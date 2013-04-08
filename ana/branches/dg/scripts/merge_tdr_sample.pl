#!/usr/bin/perl

#use strict;
use Getopt::Long;
use FileHandle;
use File::Basename;
use Shell qw(echo mkdir ls chmod grep bzip2);

# consolidate CBNT and wrapper output from TDR jobs (1 event per file). default is to combine 200 events per file.

my $inputdirname = ".";
my $outputdirname = ".";
my $insperout = 200;

GetOptions( "i=s" => \$inputdirname , "o=s" => \$outputdirname );

# do CBNT and wrapper in pairs to keep them in sync.

my $shell = Shell->new;

my @listowrappers = $shell->echo( " ${inputdirname}/*.bz2 | xargs ls -1 " );
my $nwrappers = @listowrappers;
my $nwrappersleft = $nwrappers;

$shell->mkdir( " -p $outputdirname" );

#my $nouts = $nwrappers/(1.*${insperout});
#if( ($nouts*${insperout}) < $nwrappers ) { ++$nouts; }
# figure out CBNT extension number
my $extnumber = 0;
while( $extnumber < 10 ) {
    my $cbntfirst = $listowrappers[0];
    chomp( $cbntfirst );
    $cbntfirst =~ s/EXT\d/EXT${extnumber}/;
    $cbntfirst =~ s/ftksim_wrap_raw.dat.bz2/output.aant.root/;
    # print "trying $cbntfirst\n";
    if( -e ${cbntfirst} ) {
        # print "worked!\n";
        last;
    } else {
        ++$extnumber;
    }
}
if( $extnumber > 9 ) { die "could not figure out extension number for CBNT files.\n"; }

print " working with $nwrappersleft\n";

while( $nwrappersleft > 0 ) {
    # build lists of files to merge and construct output names
    my @thesewrappers = splice( @listowrappers , 0 , $insperout );
    $nwrappersleft -= $insperout;
    if( $nwrappersleft <= 0 ) { 
        $nwrappersleft = 0; 
    }
    print "now $nwrappersleft after $insperout removed\n";
    # use first wrapper to set name
    my $firstwrapper = $thesewrappers[0];
    chomp( $firstwrapper );

    print "first wrapper is $firstwrapper\n";
    my $bwname = basename($firstwrapper);
    my $outwname = "${outputdirname}/$bwname";
    my $outcname = "${outputdirname}/$bwname";
    $outwname =~ s/ftksim/merge.ftksim/;
    $outwname =~ s/\.bz2//;
    $outcname =~ s/EXT\d/EXT${extnumber}/;
    $outcname =~ s/ftksim_wrap_raw.dat.bz2/merge.aant.root/;
    
    my @thesecbnts;
    foreach my $fwrapper (@thesewrappers) {
        chomp $fwrapper;
        my $thiscbnt = basename($fwrapper);
        $thiscbnt =~ s/EXT\d/EXT*/; #${extnumber}
        $thiscbnt =~ s/ftksim_wrap_raw.dat.bz2/output.aant.root/;
        $thiscbnt = "${inputdirname}/${thiscbnt}";
        # check that this is the only such file (no ambiguous .n extensions);
        my @listoambiguities = $shell->ls( "${thiscbnt}*" );
        if( ${@listoambiguities} > 1 ) {
            print @listoambiguities;
            print "ambiguous CBNT correspondence to file ${thiscbnt}\n";
            die;
        }
        push( @thesecbnts , $thiscbnt );
    }
    
    # print "for files\n $outwname\n and\n $outcname\n";
    # print "input cbnts:\n";
    # my $printstr = join(" ",@thesecbnts);
    # print "${printstr}\n";
    # my $ncbnts = $shell->echo( " ${printstr} | xargs ls -1 | wc" );
    # print "cbnts: declared: $thesecbnts found: $ncbnts\n";
    # print "input wrappers:\n";
    # $printstr = join(" ",@thesewrappers);
    # print "${printstr}\n";
    # my $nwraps = $shell->echo( " ${printstr} | xargs ls -1 | wc" );
    # print "wrappers: declared: $thesewrappers found: $nwraps\n";

    # merge wrapper files
    my $strwrappers = join(" ",@thesewrappers);
    my $nwrappers = $#thesewrappers;
    $shell->echo( " ${strwrappers} | xargs less | cat > $outwname" );
    my $nconcatenatedwrappers = $shell->grep( " '^L' ${outwname} | wc ");
    print "nconcatenatedwrappers: $nconcatenatedwrappers\n";
    $shell->bzip2( $outwname );

    # merge cbnt's
    my $fhmc = FileHandle::new();
    my $strcbnts = join(" ",@thesecbnts);
    my $ncbnts = $#thesecbnts;
    my $mergescript = "${outputdirname}/merge_cbnt.sh";
    open $fhmc, ">${mergescript}";
    print $fhmc "#!/bin/bash\n";
    print $fhmc "export ROOTSYS=/osg/app/atlas_app/atlas_rel/14.2.25/sw/lcg/external/root/5.18.00d/slc4_ia32_gcc34/root\n";
    print $fhmc "export LD_LIBRARY_PATH=\${ROOTSYS}/lib:\${LD_LIBRARY_PATH}\n";
    print $fhmc "echo ${strcbnts} | xargs \${ROOTSYS}/bin/hadd ${outcname} \n";
    close $fhmc;
    $shell->chmod( " u+x ${mergescript} " );
    system( ${mergescript} );
#    $shell->rm( " -f ${mergescript} " );

    if( $nwrappersleft <= 0 ) { 
        last;
    }
    
}

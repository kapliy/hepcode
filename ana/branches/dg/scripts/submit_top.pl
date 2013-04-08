#!/usr/bin/perl -w 

use lib '/share/ftkdata/boveia/perl/lib/perl5/site_perl/5.8.5/i386-linux-thread-multi/'; # for PBS::Client
use strict;
use FileHandle;
use File::Basename;
use Getopt::Long;
use PBS::Client;

my $datestring = `date --iso-8601`;
chomp $datestring;
$datestring =~ s/-//g;

my $thisusername = `whoami`;
chomp $thisusername;
my $programname = "ana_top";

my $nbits = `file ana/${programname} | awk '{print \$3}' | cut -d '-' -f 1`;
chomp( $nbits );
my $uct3_64 = ($nbits == 64 ? 1 : 0);

my @datasets;
my $tagname = "";
my $outputdir = "/share/ftkdata1/boveia/top/${datestring}_v4";
my $rundir = `echo \${PWD}`;
chomp( $rundir );

my $nsplits = 7;
my $jobtimestr = "07:59:59";

my $ana_options = "";
my $predefined_run = "";

GetOptions( "s=s" => \@datasets , "t=s" => \$tagname , "options=s" => \$ana_options ,
            "p=s" => \$predefined_run );
chomp( $tagname );

if( $predefined_run ne "" ) {
    if( $predefined_run eq "topmix" ) {
        @datasets = ();
        push( @datasets , "./datasets/topmix79_r1306_singlet.dat" );
        push( @datasets , "./datasets/topmix79_r1306_ttbar.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wbb_np1.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wbb_np2.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wenu_np0.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wenu_np1.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wenu_np2.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wenu_np3.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wenu_np4.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wenu_np5.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wmunu_np0.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wmunu_np1.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wmunu_np2.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wmunu_np3.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wmunu_np4.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wmunu_np5.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wtaunu_np0.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wtaunu_np1.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wtaunu_np2.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wtaunu_np3.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wtaunu_np4.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wtaunu_np5.dat" );
        push( @datasets , "./datasets/topmix79_r1306_ww.dat" );
        push( @datasets , "./datasets/topmix79_r1306_wz.dat" );
        push( @datasets , "./datasets/topmix79_r1306_zee_np0.dat" );
        push( @datasets , "./datasets/topmix79_r1306_zee_np1.dat" );
        push( @datasets , "./datasets/topmix79_r1306_zee_np2.dat" );
        push( @datasets , "./datasets/topmix79_r1306_zee_np3.dat" );
        push( @datasets , "./datasets/topmix79_r1306_zee_np4.dat" );
        push( @datasets , "./datasets/topmix79_r1306_zee_np5.dat" );
        push( @datasets , "./datasets/topmix79_r1306_zmm_np0.dat" );
        push( @datasets , "./datasets/topmix79_r1306_zmm_np1.dat" );
        push( @datasets , "./datasets/topmix79_r1306_zmm_np2.dat" );
        push( @datasets , "./datasets/topmix79_r1306_zmm_np3.dat" );
        push( @datasets , "./datasets/topmix79_r1306_zmm_np4.dat" );
        push( @datasets , "./datasets/topmix79_r1306_zmm_np5.dat" );
        push( @datasets , "./datasets/topmix79_r1306_ztautau_np0.dat" );
        push( @datasets , "./datasets/topmix79_r1306_ztautau_np1.dat" );
        push( @datasets , "./datasets/topmix79_r1306_ztautau_np2.dat" );
        push( @datasets , "./datasets/topmix79_r1306_ztautau_np3.dat" );
        push( @datasets , "./datasets/topmix79_r1306_ztautau_np4.dat" );
        push( @datasets , "./datasets/topmix79_r1306_ztautau_np5.dat" );
        push( @datasets , "./datasets/topmix79_r1306_zz.dat" );
        push( @datasets , "./datasets/data10_muons_20100706.dat" );
        push( @datasets , "./datasets/topmix79_108405.PythiaB_bbmu15X.dat" );
        push( @datasets , "./datasets/topmix79_109276.J0_pythia_jetjet_1muon.dat" );
        push( @datasets , "./datasets/topmix79_109277.J1_pythia_jetjet_1muon.dat" );
        push( @datasets , "./datasets/topmix79_109278.J2_pythia_jetjet_1muon.dat" );
        push( @datasets , "./datasets/topmix79_109279.J3_pythia_jetjet_1muon.dat" );
        push( @datasets , "./datasets/topmix79_109280.J4_pythia_jetjet_1muon.dat" );
        push( @datasets , "./datasets/topmix79_109281.J5_pythia_jetjet_1muon.dat" );
        push( @datasets , "./datasets/topmix79_muons_20100706.dat" );
        push( @datasets , "./datasets/topmix80_muons_20100706.dat" );
    }
}

for my $dataset (@datasets) {

    chomp( $dataset );
    die "Provide valid dataset name.\n" unless ${dataset} ne "";

    print "preparing ${programname} jobs for dataset $dataset\n";

# if( ${dataset} =~ m/data10_7TeV/ ) {
#     $nsplits = 2;
# }

    $nsplits = 16;
    my $dontuple = 1;

    my $ntblockcount = 0; $ntblockcount += `grep '^E' $dataset | wc | awk '{print \$1}'`;
    if( $nsplits > $ntblockcount ) { $nsplits = $ntblockcount; }
    
    $tagname = "";
    if( $tagname eq "" ) {
        my $dsetbase = basename( ${dataset} );
        chomp $dsetbase;
        for( my $i=1; $i<=10; ++$i ) {
            $tagname = "${dsetbase}.${datestring}.v${i}";
            if( !(-d "${outputdir}/${tagname}") ) { last; }
            if( $i==10 ) { die; }
        }
    }

    my $submitdir = "${outputdir}/${tagname}";
    chomp( $submitdir );

    if( -d ${submitdir} ) {
        print "${submitdir} already exists. skipping.\n";
        next;
    }
     #   die " ${submitdir} already exists" unless !(-d ${submitdir});
    system( "mkdir -p ${submitdir}" );

    my @top_cmds;
    my @merge_dependencies;
    my $logdir = "${submitdir}/logs/";
    if( !-d $logdir ) { system("mkdir -p $logdir"); }

    my $pbsclient = PBS::Client->new();

    my @toprootfiles;

    for( my $n=0; $n!=$nsplits; ++$n ) {
        my $inname = $dataset;
        chomp( $inname );
        my $bname = basename( $inname );
        my $batchscriptname = "$submitdir/${bname}_${n}.sh";
        my $batchlogname = "$submitdir/${bname}_${n}.log";
        my $outrootname = "$submitdir/top_${bname}_${n}";
        my $ntupleout = "";
        if( $dontuple ) {
            $ntupleout = "--out-ntuple ${submitdir}/ntuple_${n}.aen";
        }
        push( @toprootfiles , "${outrootname}.root" );
        my $bsfh = FileHandle::new();
        my $sleepseconds = (1*$n) % 30;
        open $bsfh, ">$batchscriptname";
        print $bsfh <<ENDBATCHSCRIPT;
#!/bin/bash
sleep $sleepseconds
cd $rundir
uct3_64=${uct3_64} source ./setup.sh
${programname} ${ana_options} -i $inname --force-output-filename $outrootname --noROOT --nsplits ${nsplits} --split ${n} ${ntupleout}
ENDBATCHSCRIPT
        close $bsfh;
        system( "chmod u+x ${batchscriptname}" );
        push( @top_cmds , $batchscriptname );
#       system( "qsub -l walltime=07:59:59 -N standard -j oe -o $batchlogname $batchscriptname" );
    }

# specify the top jobs
    my $top_jobspec = PBS::Client::Job->new(
        queue => "uct3",
        ofile => $logdir,
        efile => $logdir,
        name => "top",
        cput => $jobtimestr,
        wallt => $jobtimestr,
        cmd => \@top_cmds
        );
# prepare the merge
    my $mergescriptname = "${submitdir}/merge_top.sh";
    my $mergefile = "${submitdir}/${tagname}";
#    print "defining top merge job.\n";
    my $msfh = FileHandle::new();
    open $msfh, ">$mergescriptname";
    print $msfh <<ENDMERGESCRIPT;
#!/bin/bash
sleep 1
cd $rundir
uct3_64=${uct3_64} source ./setup.sh
echo @{toprootfiles} | xargs ls -1 > ${submitdir}/top.root.list
dgplot -m ${mergefile} -q -l ${submitdir}/top.root.list
ENDMERGESCRIPT
    close $msfh;
    system( "chmod u+x $mergescriptname" );
    my $merge_jobspec = PBS::Client::Job->new(
        queue => "uct3",
        ofile => "${submitdir}/merge_top.out.log",
        efile => "${submitdir}/merge_top.err.log",
        name => "top_merge",
        cput => $jobtimestr,
        wallt => $jobtimestr,
        cmd => $mergescriptname
        );

    print "submitting ${dataset}...\n";
# set dependencies:
#    $top_jobspec->pack( numQ => 32 );
    $top_jobspec->next( { end=>$merge_jobspec } );

    if( $thisusername eq "boveia" ) {
    # add an email notification when the job finishes
        my $notifyscriptname = "${submitdir}/notify_top.sh";
        my $msfh = FileHandle::new();
        open $msfh, ">$notifyscriptname";
        print $msfh <<ENDNOTIFYSCRIPT;
ssh hep \"echo \\\"${mergefile}.root complete.\\\" | mail -s \\\"top job complete\\\" boveia+notice\@me.com\"
ENDNOTIFYSCRIPT
        close $msfh;
        system( "chmod u+x $notifyscriptname" );
        my $notify_jobspec = PBS::Client::Job->new(
            queue => "uct3",
            ofile => "${submitdir}/notify_top.out.log",
            efile => "${submitdir}/notify_top.err.log",
            name => "top_notify",
            cput => $jobtimestr,
            wallt => $jobtimestr,
            cmd => $notifyscriptname
            );
        $merge_jobspec->next( { end=>$notify_jobspec } );
    }

    $pbsclient->qsub( $top_jobspec );

    print "when jobs have finished, run\n";
    print "  dgplot ${mergefile}.root\n";

    if( $thisusername eq "boveia" ) {
        my $timeestimate = "now + 1 hour";
        system( "ssh hep.uchicago.edu \"echo \\\"dgplot ${mergefile}.root\\\" | mail -s \\\"check top analysis plots of $dataset #$timeestimate #$timeestimate //automatic entry from submit_top.pl\\\" boveia+todo\@me.com \"" );
    }

} # end for each dataset    

print "done submitting.\n";


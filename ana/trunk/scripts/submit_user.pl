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
my $programname = "ana_user";

my $nbits = `file ana/${programname} | awk '{print \$3}' | cut -d '-' -f 1`;
chomp( $nbits );
my $uct3_64 = ($nbits == 64 ? 1 : 0);

my $dataset = "";
my $tagname = "";
my $outputdir = "~/";
my $rundir = `echo \${PWD}`;
chomp( $rundir );

my $nsplits = 7;
my $jobtimestr = "07:59:00";

my $ana_options = "";
my $dontuple = 0;
my $noDG = 0;
my $nevents = 0;

GetOptions( "i=s" => \$dataset , "t=s" => \$tagname , "options=s" => \$ana_options, "n=i" => \$nsplits, "xml!" => \$dontuple, "noDG!" => \$noDG, "nevts=i" => \$nevents );
chomp( $dataset );
chomp( $tagname );

die "Provide dataset name.\n" unless ${dataset} ne "";

# if( ${dataset} =~ m/data10_7TeV/ ) {
#     $nsplits = 2;
# }

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

die " ${submitdir} already exists" unless !(-d ${submitdir});
system( "mkdir -p ${submitdir}" );

print "preparing standard user study jobs.\n";

my @user_cmds;
my @merge_dependencies;
my $logdir = "${submitdir}/logs/";
if( !-d $logdir ) { system("mkdir -p $logdir"); }

my $pbsclient = PBS::Client->new();

my @userrootfiles;

for( my $n=0; $n!=$nsplits; ++$n ) {
    my $inname = $dataset;
    chomp( $inname );
    my $bname = basename( $inname );
    my $batchscriptname = "$submitdir/${bname}_${n}.sh";
    my $batchlogname = "$submitdir/${bname}_${n}.log";
    my $outrootname = "$submitdir/user_${bname}_${n}";
    my $ntupleout = "";
    if( $dontuple ) {
        $ntupleout = "--out-ntuple ${submitdir}/ntuple_${n}.aen";
    }
    my $dgorroot = "--noROOT";
    if( $noDG ) {
	$dgorroot = "--noDG";
    }
    my $neventstr = "";
    if( $nevents > 0 ) {
      $neventstr = "-n $nevents";
    }
    push( @userrootfiles , "${outrootname}.root" );
    my $bsfh = FileHandle::new();
    my $sleepseconds = (1*$n) % 30;
    open $bsfh, ">$batchscriptname";
    print $bsfh <<ENDBATCHSCRIPT;
#!/bin/bash
sleep $sleepseconds
cd $rundir
uct3_64=${uct3_64} source ./setup.sh
./smart_runner.sh ${programname} ${ana_options} -i $inname --force-output-filename $outrootname --nsplits ${nsplits} --split ${n} ${ntupleout} ${dgorroot} ${neventstr}
ENDBATCHSCRIPT
    close $bsfh;
    system( "chmod u+x ${batchscriptname}" );
    push( @user_cmds , $batchscriptname );
#    system( "qsub -l walltime=07:59:59 -N standard -j oe -o $batchlogname $batchscriptname" );
}

# specify the user jobs
my $user_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => $logdir,
    efile => $logdir,
    name => "user",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => \@user_cmds
    );
# prepare the merge
my $mergescriptname = "${submitdir}/merge_user.sh";
my $mergefile = "${submitdir}/${tagname}";
print "defining user merge job.\n";
my $msfh = FileHandle::new();
open $msfh, ">$mergescriptname";
print $msfh <<ENDMERGESCRIPT;
#!/bin/bash
sleep 1
cd $rundir
uct3_64=${uct3_64} source ./setup.sh
echo @{userrootfiles} | xargs ls -1 > ${submitdir}/user.root.list
dgplot -m ${mergefile} -q -l ${submitdir}/user.root.list
ENDMERGESCRIPT
close $msfh;
system( "chmod u+x $mergescriptname" );
my $merge_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => "${submitdir}/merge_user.out.log",
    efile => "${submitdir}/merge_user.err.log",
    name => "user_merge",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => $mergescriptname
    );

print "submitting all jobs...\n";
# set dependencies:
#    $user_jobspec->pack( numQ => 32 );

# comment out this line to remove merge job
if( !$noDG ) {
    $user_jobspec->next( { end=>$merge_jobspec } );
}

$pbsclient->qsub( $user_jobspec );

print "when jobs have finished, run\n";
print "  dgplot ${mergefile}.root\n";
print "done submitting.\n";

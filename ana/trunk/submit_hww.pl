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
my $programname = "ana_hww";

my $nbits = `file ana/${programname} | awk '{print \$3}' | cut -d '-' -f 1`;
chomp( $nbits );
my $uct3_64 = ($nbits == 64 ? 1 : 0);

my $dataset = "";
my $tagname = "";
my $outputdir = "/share/ftkdata1/${thisusername}/ana";
my $rundir = `echo \${PWD}`;
chomp( $rundir );

my $nsplits = 7;
my $jobtimestr = "07:59:59";

my $ana_options = "";
my $ntuple_directory = "";

GetOptions( "s=s" => \$dataset , "t=s" => \$tagname , "options=s" => \$ana_options, "n=i" => \$nsplits, "xml=s" => \$ntuple_directory );
chomp( $dataset );
chomp( $tagname );

die "Provide dataset name.\n" unless ${dataset} ne "";

my $dontuple = 0;
if( $ntuple_directory ne "" ) { $dontuple = 1; }

$nsplits = 4;
if( ${dataset} =~ m/data10_7TeV/ ) {
    $nsplits = 5;
}

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

print "preparing standard hww study jobs.\n";

my @hww_cmds;
my @merge_dependencies;
my $logdir = "${submitdir}/logs/";
if( !-d $logdir ) { system("mkdir -p $logdir"); }

my $pbsclient = PBS::Client->new();

my @hwwrootfiles;

for( my $n=0; $n!=$nsplits; ++$n ) {
    my $inname = $dataset;
    chomp( $inname );
    my $bname = basename( $inname );
    my $batchscriptname = "$submitdir/${bname}_${n}.sh";
    my $batchlogname = "$submitdir/${bname}_${n}.log";
    my $outrootname = "$submitdir/hww_${bname}_${n}";
    my $ntupleout = "";
    if( $dontuple ) {
        $ntupleout = "--out-ntuple ${ntuple_directory}/ntuple_${n}.aen";
    }
    push( @hwwrootfiles , "${outrootname}.root" );
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
    push( @hww_cmds , $batchscriptname );
#    system( "qsub -l walltime=07:59:59 -N standard -j oe -o $batchlogname $batchscriptname" );
}

# specify the hww jobs
my $hww_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => $logdir,
    efile => $logdir,
    name => "hww",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => \@hww_cmds
    );
# prepare the merge
my $mergescriptname = "${submitdir}/merge_hww.sh";
my $mergefile = "${submitdir}/${tagname}";
print "defining hww merge job.\n";
my $msfh = FileHandle::new();
open $msfh, ">$mergescriptname";
print $msfh <<ENDMERGESCRIPT;
#!/bin/bash
sleep 1
cd $rundir
uct3_64=${uct3_64} source ./setup.sh
echo @{hwwrootfiles} | xargs ls -1 > ${submitdir}/hww.root.list
dgplot -m ${mergefile} -q -l ${submitdir}/hww.root.list
ENDMERGESCRIPT
close $msfh;
system( "chmod u+x $mergescriptname" );
my $merge_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => "${submitdir}/merge_hww.out.log",
    efile => "${submitdir}/merge_hww.err.log",
    name => "hww_merge",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => $mergescriptname
    );

print "submitting all jobs...\n";
# set dependencies:
#    $hww_jobspec->pack( numQ => 32 );
$hww_jobspec->next( { end=>$merge_jobspec } );

if( $thisusername eq "boveia" ) {
    # add an email notification when the job finishes
    my $notifyscriptname = "${submitdir}/notify_hww.sh";
    my $msfhn = FileHandle::new();
    open $msfhn, ">$notifyscriptname";
    print $msfhn <<ENDNOTIFYSCRIPT;
ssh hep \"echo \\\"${mergefile}.root complete.\\\" | mail -s \\\"hww job complete\\\" boveia+notice\@me.com\"
ENDNOTIFYSCRIPT
    close $msfhn;
    system( "chmod u+x $notifyscriptname" );
#!/bin/bash
    my $notify_jobspec = PBS::Client::Job->new(
        queue => "uct3",
        ofile => "${submitdir}/notify_hww.out.log",
        efile => "${submitdir}/notify_hww.err.log",
        name => "hww_notify",
        cput => $jobtimestr,
        wallt => $jobtimestr,
        cmd => $notifyscriptname
        );
    $merge_jobspec->next( { end=>$notify_jobspec } );
}

$pbsclient->qsub( $hww_jobspec );

print "when jobs have finished, run\n";
print "  dgplot ${mergefile}.root\n";
print "done submitting.\n";

if( $thisusername eq "boveia" ) {
   my $timeestimate = "now + 1 hour";
        system( "ssh hep.uchicago.edu \"echo \\\"dgplot ${mergefile}.root\\\" | mail -s \\\"check ${programname} analysis plots of $dataset #$timeestimate #$timeestimate //automatic entry from submit_hww.pl\\\" boveia+todo\@me.com \"" );
}

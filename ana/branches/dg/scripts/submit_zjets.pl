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
my $programname = "ana_zjets";

my $nbits = `file ana/${programname} | awk '{print \$3}' | cut -d '-' -f 1`;
chomp( $nbits );
my $uct3_64 = ($nbits == 64 ? 1 : 0);

my $dataset = "";
my $tagname = "";
my $outputdir = "/share/ftkdata1/jwebster/WZphys/ana";
my $rundir = `echo \${PWD}`;
chomp( $rundir );

my $nsplits = 7;
my $jobtimestr = "07:59:59";

my $ana_options = "";
my $dontuple = 0;
my $doDG = 0;
my $eltrigger = 1;
my $phtrigger = 1;
my $mutrigger = 1;
my $vtxplots = 0;

GetOptions( "i=s" => \$dataset , "t=s" => \$tagname , "options=s" => \$ana_options, "n=i" => \$nsplits, "xml!" => \$dontuple, "dg!" => \$doDG, "eltrig=i" => \$eltrigger, "phtrig=i" => \$phtrigger, "mutrig=i" => \$mutrigger, "doVTX!" => \$vtxplots );
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

print "preparing standard zjets study jobs.\n";

my @zjets_cmds;
my @merge_dependencies;
my $logdir = "${submitdir}/logs/";
if( !-d $logdir ) { system("mkdir -p $logdir"); }

my $pbsclient = PBS::Client->new();

my @zjetsrootfiles;

for( my $n=0; $n!=$nsplits; ++$n ) {
    my $inname = $dataset;
    chomp( $inname );
    my $bname = basename( $inname );
    my $batchscriptname = "$submitdir/${bname}_${n}.sh";
    my $batchlogname = "$submitdir/${bname}_${n}.log";
    my $outrootname = "$submitdir/zjets_${bname}_${n}";
    my $ntupleout = "";
    if( $dontuple ) {
        $ntupleout = "--out-ntuple ${submitdir}/ntuple_${n}.aen";
    }
    my $dgorroot = "--noDG";
    if( $doDG ) {
	$dgorroot = "--noROOT";
    }
    my $vtxstring = "--noVTX";
    if( $vtxplots ) {
        $vtxstring = "";
    }
    push( @zjetsrootfiles , "${outrootname}.root" );
    my $bsfh = FileHandle::new();
    my $sleepseconds = (1*$n) % 30;
    open $bsfh, ">$batchscriptname";
    print $bsfh <<ENDBATCHSCRIPT;
#!/bin/bash
sleep $sleepseconds
cd $rundir
uct3_64=${uct3_64} source ./setup.sh
./smart_runner.sh ${programname} ${ana_options} -i $inname --force-output-filename $outrootname --nsplits ${nsplits} --split ${n} ${ntupleout} ${dgorroot} --elec-trig ${eltrigger} --phot-trig ${phtrigger} --muon-trig ${mutrigger} ${vtxstring}
ENDBATCHSCRIPT
    close $bsfh;
    system( "chmod u+x ${batchscriptname}" );
    push( @zjets_cmds , $batchscriptname );
#    system( "qsub -l walltime=07:59:59 -N standard -j oe -o $batchlogname $batchscriptname" );
}

# specify the zjets jobs
my $zjets_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => $logdir,
    efile => $logdir,
    name => "zjets",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => \@zjets_cmds
    );
# prepare the merge
my $mergescriptname = "${submitdir}/merge_zjets.sh";
my $mergefile = "${submitdir}/${tagname}";
print "defining zjets merge job.\n";
my $msfh = FileHandle::new();
open $msfh, ">$mergescriptname";
print $msfh <<ENDMERGESCRIPT;
#!/bin/bash
sleep 1
cd $rundir
uct3_64=${uct3_64} source ./setup.sh
echo @{zjetsrootfiles} | xargs ls -1 > ${submitdir}/zjets.root.list
dgplot -m ${mergefile} -q -l ${submitdir}/zjets.root.list
ENDMERGESCRIPT
close $msfh;
system( "chmod u+x $mergescriptname" );
my $merge_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => "${submitdir}/merge_zjets.out.log",
    efile => "${submitdir}/merge_zjets.err.log",
    name => "zjets_merge",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => $mergescriptname
    );

print "submitting all jobs...\n";
# set dependencies:
#    $zjets_jobspec->pack( numQ => 32 );

# comment out this line to remove merge job
if( $doDG ) {
    $zjets_jobspec->next( { end=>$merge_jobspec } );
}

if( $thisusername eq "boveia" ) {
    # add an email notification when the job finishes
    my $notifyscriptname = "${submitdir}/notify_zjets.sh";
    my $msfh = FileHandle::new();
    open $msfh, ">$notifyscriptname";
    print $msfh <<ENDNOTIFYSCRIPT;
echo "${mergefile}.root complete." | ssh hep "mail -s \"zjets job complete\" boveia+notice\@me.com"
ENDNOTIFYSCRIPT
    close $msfh;
    system( "chmod u+x $notifyscriptname" );
#!/bin/bash
    my $notify_jobspec = PBS::Client::Job->new(
        queue => "uct3",
        ofile => "${submitdir}/notify_zjets.out.log",
        efile => "${submitdir}/notify_zjets.err.log",
        name => "zjets_notify",
        cput => $jobtimestr,
        wallt => $jobtimestr,
        cmd => $notifyscriptname
        );
    $merge_jobspec->next( { end=>$notify_jobspec } );
}

$pbsclient->qsub( $zjets_jobspec );

print "when jobs have finished, run\n";
print "  dgplot ${mergefile}.root\n";
print "done submitting.\n";

if( $thisusername eq "boveia" ) {
   my $timeestimate = "now + 1 hour";
   system( " echo \"dgplot ${mergefile}.root\" | mail -s \"check w+jets analysis plots of $dataset #$timeestimate #$timeestimate //automatic entry from submit_zjets.pl\" boveia+todo\@me.com " );
}

#!/usr/bin/perl -w 

#use lib '/share/ftkdata/boveia/perl/lib/perl5/site_perl/5.8.5/i386-linux-thread-multi/'; # for PBS::Client
use strict;
use FileHandle;
use File::Basename;
use Getopt::Long;
use PBS::Client;

my $thisusername = `whoami`;
chomp( $thisusername );

# MAKE SURE YOU SET THESE VARIABLES CORRECTLY BEFORE RUNNING
# ==========================================================
# base directory for output folders
my $outputdir = "/share/ftkdata/jwebster/wzjets/ana"; 
# for email notification when jobs are complete
my $email = "jwebste2\@gmail.com"; 
my $cdfusername = "jswebster"; # cdf.uchicago.edu username
# bash commands for sourcing the root/TrigFTKAna environment
my $bashsetup = ". /home/jwebster/.bashrc; anaquick; cd /share/ftkdata1/jwebster/TrigFTKAna/CommonAnalysis; . setup.sh;";
# ==========================================================

my $datestring = `date --iso-8601`;
chomp $datestring;
$datestring =~ s/-//g;

my $programname = "ana_wzjets";

my $nbits = `file ana/${programname} | awk '{print \$3}' | cut -d '-' -f 1`;
chomp( $nbits );
my $uct3_64 = ($nbits == 64 ? 1 : 0);

my $dataset = "";
my $tagname = "";
my $rundir = `echo \${PWD}`;
chomp( $rundir );

my $nsplits = 1;
my $jobtimestr = "20:00:00";

my $ana_options = "";
my $dontuple = 0;
my $doDG = 0;
my $vtxplots = 0;
my $nevents = 0;
my $notify = 0;
my $mergeonly = 0;

GetOptions( "i=s" => \$dataset , "t=s" => \$tagname , "options=s" => \$ana_options, "n=i" => \$nsplits, "xml!" => \$dontuple, "dg!" => \$doDG, "vtx!" => \$vtxplots, "nevts=i" => \$nevents, "notify!" => \$notify, "merge!" => \$mergeonly );
chomp( $dataset );
chomp( $tagname );

die "Provide dataset name.\n" unless ${dataset} ne "";

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

if( $mergeonly==0 ) {
    die " ${submitdir} already exists" unless !(-d ${submitdir});
    system( "mkdir -p ${submitdir}" );
}

print "preparing standard wzjets study jobs.\n";

my @wzjets_cmds;
my @merge_dependencies;
my $logdir = "${submitdir}/logs/";
if( !-d $logdir ) { system("mkdir -p $logdir"); }

my $pbsclient = PBS::Client->new();

my @wzjetsrootfiles;

if( $mergeonly ) {
    print( "only merging.\n" );
} else {
    for( my $n=0; $n!=$nsplits; ++$n ) {
	my $inname = $dataset;
	chomp( $inname );
	my $bname = basename( $inname );
	my $batchscriptname = "$submitdir/${bname}_${n}.sh";
	my $batchlogname = "$submitdir/${bname}_${n}.log";
	my $outrootname = "$submitdir/wzjets_${bname}_${n}";
	my $ntupleout = "";
	if( $dontuple ) {
	    $ntupleout = "--out-ntuple ${submitdir}/ntuple_${n}.aen";
	}
	my $dgorroot = "--noDG";
	if( $doDG ) {
	    $dgorroot = "--noROOT";
	}
	my $neventstr = "";
	if( $nevents > 0 ) {
	    $neventstr = "-n $nevents";
	}
	push( @wzjetsrootfiles , "${outrootname}.root" );
	my $bsfh = FileHandle::new();
	my $sleepseconds = (1*$n) % 30;
	open( $bsfh , ">$batchscriptname" );
	print $bsfh "#!/bin/bash\n";
	print $bsfh "${bashsetup}\n";
	print $bsfh "cd ${rundir}\n";
	print $bsfh "ana/${programname} ${ana_options} -i $inname --force-output-filename $outrootname --nsplits ${nsplits} --split ${n} ${ntupleout} ${dgorroot} ${neventstr} --offline-read d3pd \n";
	close( $bsfh );
	system( "chmod u+x ${batchscriptname}" );
	push( @wzjets_cmds , $batchscriptname );
    }
}

my $wzjets_jobspec_name = "${tagname}_wzjets";
# specify the wzjets jobs
my $wzjets_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => $logdir,
    efile => $logdir,
    name => $wzjets_jobspec_name,
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => \@wzjets_cmds
    );

# prepare the merge
my $mergescriptname = "${submitdir}/merge_wzjets.sh";
my $mergefile = "${submitdir}/${tagname}";
print "defining wzjets merge job.\n";
my $msfh = FileHandle::new();
open( $msfh , ">$mergescriptname" );
if( $doDG ) {
    print $msfh "#!/bin/bash\n";
    print $msfh "${bashsetup}\n";
    print $msfh "sleep 1\n";
    print $msfh "cd $rundir\n";
    print $msfh "echo @{wzjetsrootfiles} | xargs ls -1 > ${submitdir}/wzjets.root.list\n";
    print $msfh "dgplot -m ${mergefile} -q -l ${submitdir}/wzjets.root.list\n";
} else {
    print $msfh "#!/bin/bash\n";
    print $msfh "${bashsetup}\n";
    print $msfh "sleep 1\n";
    print $msfh "cd $rundir\n";
    print $msfh ". scripts/smart_root_merge.sh ${submitdir}\n";
}
close( $msfh );
system( "chmod u+x $mergescriptname" );
my $merge_jobspec_name = "${tagname}_merge";
my $merge_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => "${logdir}/merge_wzjets.out",
    efile => "${logdir}/merge_wzjets.err",
    name => $merge_jobspec_name,
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => $mergescriptname
    );
$wzjets_jobspec->next( { end=>$merge_jobspec } );

# prepare notification job
if( $notify ) {
    # add an email notification when the job finishes
    my $notifyscriptname = "${submitdir}/notify_wzjets.sh";
    my $finished_rootfiles = 0;
    my $expected_rootfiles = 1;
    if( $mergeonly ) {
	$expected_rootfiles = 1;
    } else {
	$expected_rootfiles = $nsplits + 1;
    }
    my $job_efficiency = $finished_rootfiles/$expected_rootfiles;
    my $msfh = FileHandle::new();
    open( $msfh , ">$notifyscriptname" );
    print $msfh "#!/bin/bash\n";
    if( $mergeonly ) {
	print $msfh "nfiles=`ls ${submitdir}/merged.root | wc -l`\n";
    } else {
	print $msfh "nfiles=`ls ${submitdir}/*.root | wc -l`\n";
    }
    print $msfh "echo -e \"${submitdir} job complete\\nsuccessfully produced \$nfiles out of ${expected_rootfiles} root files\" | ssh ${cdfusername}\@cdf.uchicago.edu \"mail -s \\\"wzjets job report\\\" ${email}\"\n";
    close( $msfh );
    system( "chmod u+x $notifyscriptname" );
    my $notify_jobspec_name = "${tagname}_notify";
    my $notify_jobspec = PBS::Client::Job->new(
	queue => "uct3",
	ofile => "${logdir}/notify_wzjets.out",
	efile => "${logdir}/notify_wzjets.err",
	name => $notify_jobspec_name,
	cput => $jobtimestr,
	wallt => $jobtimestr,
	cmd => $notifyscriptname
	);
    $merge_jobspec->next( { end=>$notify_jobspec } );
}

print "submitting jobs...\n";
$pbsclient->qsub( $wzjets_jobspec );

print "when jobs have finished, run\n";
print "  root -l ${submitdir}/merged.root\n";
print "done submitting.\n";

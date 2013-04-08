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
my $programname = "ana_standard_overview";

my $nbits = `file ana/${programname} | awk '{print \$3}' | cut -d '-' -f 1`;
chomp( $nbits );
my $uct3_64 = ($nbits == 64 ? 1 : 0);

my $dataset = "";
my $tagname = "";
my $outputdir = "./results";
my $rundir = `echo \${PWD}`;
chomp( $rundir );

my $nsplits = 64;
my $jobtimestr = "07:59:59";

my $ana_options = "--absetacut 2.5 --mcut 0.7";
my $description = "no description provided";
my $dontuple = 0;

GetOptions( "s=s" => \$dataset , "t=s" => \$tagname , "options=s" => \$ana_options , "d=s" => \$description, "n=i" => \$nsplits, "xml!" => \$dontuple );
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
my $dsetbase = basename( ${dataset} );
chomp $dsetbase;

my $submitdir = "${outputdir}/${tagname}";
chomp( $submitdir );

die " ${submitdir} already exists" unless !(-d ${submitdir});
system( "mkdir -p ${submitdir}" );

my $mergescriptname = "${submitdir}/merge_overview.sh";
my $mergefile = "${submitdir}/${tagname}";

my @overview_cmds;
my @merge_dependencies;
my $logdir = "${submitdir}/logs/";
if( !-d $logdir ) { system("mkdir -p $logdir"); }

my $pbsclient = PBS::Client->new();

my @overviewrootfiles;

for( my $n=0; $n!=$nsplits; ++$n ) {
    my $inname = $dataset;
    chomp( $inname );
    my $bname = basename( $inname );
    my $batchscriptname = "$submitdir/${bname}_${n}.sh";
    my $batchlogname = "$submitdir/${bname}_${n}.log";
    my $outrootname = "$submitdir/overview_${bname}_${n}";
    my $ntupleout = "";
    if( $dontuple ) {
        $ntupleout = "--out-ntuple ${submitdir}/ntuple_${n}.aen";
    }
    push( @overviewrootfiles , "${outrootname}.root" );
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
    push( @overview_cmds , $batchscriptname );
#    system( "qsub -l walltime=07:59:59 -N standard -j oe -o $batchlogname $batchscriptname" );
}

# specify the overview jobs
my $overview_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => $logdir,
    efile => $logdir,
    name => "overview",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => \@overview_cmds
    );
# prepare the merge
print "defining overview merge job.\n";
my $msfh = FileHandle::new();
open $msfh, ">$mergescriptname";
print $msfh <<ENDMERGESCRIPT;
#!/bin/bash
sleep 1
cd $rundir
uct3_64=${uct3_64} source ./setup.sh
echo @{overviewrootfiles} | xargs ls -1 > ${submitdir}/overview.root.list
dgplot -m ${mergefile} -q -l ${submitdir}/overview.root.list
ENDMERGESCRIPT
close $msfh;
system( "chmod u+x $mergescriptname" );
my $merge_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => "${submitdir}/merge_overview.out.log",
    efile => "${submitdir}/merge_overview.err.log",
    name => "overview_merge",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => $mergescriptname
    );

print "submitting all jobs...\n";
# set dependencies:
#    $overview_jobspec->pack( numQ => 32 );
$overview_jobspec->next( { end=>$merge_jobspec } );

if( $thisusername eq "boveia" ) {
    # add an email notification when the job finishes
    my $notifyscriptname = "${submitdir}/notify_overview.sh";
    my $msfh = FileHandle::new();
    open $msfh, ">$notifyscriptname";
    print $msfh <<ENDNOTIFYSCRIPT;
ssh hep \"echo \\\"${mergefile}.root complete.\\\" | mail -s \\\"overview job complete\\\" boveia+notice\@me.com\"
ENDNOTIFYSCRIPT
    close $msfh;
    system( "chmod u+x $notifyscriptname" );
#!/bin/bash
    my $notify_jobspec = PBS::Client::Job->new(
        queue => "uct3",
        ofile => "${submitdir}/notify_overview.out.log",
        efile => "${submitdir}/notify_overview.err.log",
        name => "overview_notify",
        cput => $jobtimestr,
        wallt => $jobtimestr,
        cmd => $notifyscriptname
        );
    $merge_jobspec->next( { end=>$notify_jobspec } );
}

$pbsclient->qsub( $overview_jobspec );

print "preparing standard overview study jobs.\n";

print "when jobs have finished, run\n";
print "  dgplot ${mergefile}.root\n";
print "done submitting.\n";

system( "echo \"${description}\" > ${submitdir}/description.txt" );
system( "echo \"${ana_options}\" > ${submitdir}/options.txt" );

if( $thisusername eq "boveia" ) {
   my $timeestimate = "now + 1 hour";
   if( $dataset =~ m/1[eE]34/ ) { $timeestimate = "now + 2 hours"; }
   elsif( $dataset =~ m/3[eE]34/ ) { $timeestimate = "now + 4 hours"; }
   elsif( $dataset =~ m/20100330_flatcluster/ ) { $timeestimate = "now"; }
   system( "ssh hep.uchicago.edu \"echo \\\"dgplot ${mergefile}.root\n${description}\n//${ana_options}\\\" | mail -s \\\"check overview analysis plots of $dataset #$timeestimate #$timeestimate //automatic entry from submit_overview.pl\\\" boveia+todo\@me.com \"" );
}

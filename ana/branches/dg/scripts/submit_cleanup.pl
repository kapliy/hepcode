#!/usr/bin/perl -w 

use lib '/share/ftkdata/boveia/perl/lib/perl5/site_perl/5.8.5/i386-linux-thread-multi/'; # for PBS::Client
use strict;
use FileHandle;
use File::Basename;
use PBS::Client;
use Shell qw(echo mkdir ls chmod rm);
#use Term::ProgressBar;

my $shell = Shell->new;

my $rundir = $shell->echo("\${PWD}");
chomp( $rundir );
my $outdir = "/pnfs/uct3/users/boveia/clean.third.setO.v14training/";
# my $outdir = "/pnfs/uct3/users/boveia/clean.second.360M.v14training/";
# my $outdir = "/share/ftkdata/boveia/v14training/";
if( !-e $outdir ) { 
    $shell->mkdir( "-p $outdir" ); 
}
################################################################
my $jobtimestr = "07:59:59";
# my @inputfiles = $shell->echo( "/share/ftkdata1/boveia/samples/user09*/*.bz2 | xargs ls -1" );
# my @inputfilesA = $shell->echo( "/share/ftkdata1/boveia/samples/user09.JosephTuggle.FTKtrainingmuons.set07MWT2.20090921/*.bz2 | xargs ls -1" );
# my @inputfilesC = $shell->echo( "/share/ftkdata1/boveia/samples/user09.ConstantinosMelachrinos.FTKtrainingmuons.set09MWT2.20090921/*.bz2 | xargs ls -1" );
# my @inputfilesD = $shell->echo( "/share/ftkdata1/boveia/samples/user09.MatthewFeickert.FTKtrainingmuons.set06MWT2.20090921/*.bz2 | xargs ls -1" );
# my @inputfilesE = $shell->echo( "/share/ftkdata1/boveia/samples/user09.MonicaDunford.FTKtrainingmuons.set04MWT2.20090921/*.bz2 | xargs ls -1" );
# my @inputfilesF = $shell->echo( "/share/ftkdata1/boveia/samples/user09.ChristopherMeyer.FTKtrainingmuons.set01MWT2.20090921/*.bz2 | xargs ls -1" );
# my @inputfilesG = $shell->echo( "/share/ftkdata1/boveia/samples/user09.AntonioBoveia.FTKtrainingmuons.set02MWT2.20090924/*.bz2 | xargs ls -1" );
# my @inputfilesA = $shell->echo( "/share/ftkdata1/boveia/samples/user09.JosephTuggle.FTKtrainingmuons.set08MWT2.20090921/*.bz2 | xargs ls -1" );
# my @inputfilesB = $shell->echo( "/share/ftkdata1/boveia/samples/user09.ConstantinosMelachrinos.FTKtrainingmuons.set10MWT2.20090921/*.bz2 | xargs ls -1" );
print "retrieving list of single muon training files to clean...\n";
#my @inputfilesA = $shell->echo( "/pnfs/uct3/users/boveia/samples/second.360M.muons/*/*.bz2 | xargs ls -1" );
my @inputfilesA = $shell->echo( "/pnfs/uct3/users/boveia/v14.zeroedbeamrec/*FTKtraining*/*.bz2 | xargs ls -1" );
my @inputfiles = @inputfilesA;
# push( @inputfiles , @inputfilesB );
# push( @inputfiles , @inputfilesC );
# push( @inputfiles , @inputfilesD );
# push( @inputfiles , @inputfilesE );
# push( @inputfiles , @inputfilesF );
# push( @inputfiles , @inputfilesG );
my $ninputfiles = @inputfiles;
print "retrieved list of $ninputfiles files.\n";

my $submitdir = "/share/ftkdata/boveia/v14training/third_setOmuons.3/";
chomp( $submitdir );
if( !-e $submitdir ) { 
    $shell->mkdir( "-p $submitdir" ); 
} else { 
    print "${submitdir} already exists.\n";
    die;
}

my $pbsclient = PBS::Client->new();

my @cmds;

my $logdir1 = "${submitdir}/log/";
chomp( $logdir1 );
if( !-e $logdir1 ) { 
    $shell->mkdir( "-p $logdir1" ); 
}

print "Configuration\n";
print "================================================================\n";
print "saving output to ${outdir}\n";
print "submission scripts go to ${submitdir}\n";
print "log files go to ${logdir1}\n";


#my $nextprogressupdate = 0;
#my $progress = Term::ProgressBar->new({name => 'Job setup', count => $ninputfiles, remove => 1});
#$progress->minor(0);
my $n = 0;

foreach my $inname (@inputfiles) {
    ++$n;
    chomp( $inname );
    my $bname = basename( $inname );
    $bname =~ s/\.bz2//;
    my $bzname = basename( $inname );
    my $batchscriptname = "$submitdir/filter_$bname.sh";
    my $tmpinputname = "/scratch/boveia/dirty_${bzname}";
    my $outputname = "${outdir}/clean_${bname}.dat";
    my $tmpoutputname = "/scratch/boveia/clean_${bname}.dat";
    my $bsfh = FileHandle::new();
    my $sleepseconds = 60 - ((1*$n)%60);
    if( $sleepseconds < 0 ) { $sleepseconds = 1; }
    open $bsfh, ">$batchscriptname";
    print $bsfh <<ENDBATCHSCRIPT;
#!/bin/bash
sleep $sleepseconds
cd ${rundir}
source ./setup.sh
source ~/scripts/setup_dcap.sh
if [ -e $outputname ]; then
        echo "deleting existing ${outputname}...\n"
        rm -f ${outputname}
fi
dccp ${inname} ${tmpinputname}
cleanup_training_wrapper -i ${tmpinputname} -o $tmpoutputname -b 10001 --trackbar 10001
source ~/scripts/setup_dcap.sh
dccp ${tmpoutputname} ${outputname}
rm -f ${tmpoutputname}
rm -f ${tmpinputname}
ENDBATCHSCRIPT
close $bsfh;
    $shell->chmod( "u+x $batchscriptname" );
    push( @cmds , ${batchscriptname} );
#    $nextprogressupdate = $progress->update($n)
#      if $n >= $nextprogressupdate;
}
# specify the jobs
my $jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => $logdir1,
    efile => $logdir1,
    name => "filter",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => \@cmds
    );

print "submitting all jobs...\n";
$jobspec->pack( numQ => 600 );
$pbsclient->qsub( $jobspec );

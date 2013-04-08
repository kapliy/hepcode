#!/usr/bin/perl -w 

use lib '/share/ftkdata/boveia/perl/lib/perl5/site_perl/5.8.5/i386-linux-thread-multi/'; # for PBS::Client
use strict;
use FileHandle;
use File::Basename;
use PBS::Client;
use Shell qw(echo mkdir ls chmod);
use Getopt::Long;

my $shell = Shell->new;

my $thisusername = `whoami`;
chomp $thisusername;

my $rundir = $shell->echo("\${PWD}");
chomp( $rundir );
# my $split_list_command = "samples/whzero_aug05_anton_tracks/split_*.dat";
# my $tagname = "wh_nom_aug05.20090823.v1";
# my $split_list_command = "samples/whzero_aug05_anton_tracks_shifted/split_*.dat";
# my $tagname = "wh_nom_aug05_shifted.20090823.v1";
# my $split_list_command = "samples/whzero_aug07_anton_tracks_shifted/split_*.dat";
# my $tagname = "wh_nom_aug07_shifted.20090823.v1";
# my $split_list_command = "samples/whzero_aug07_anton_tracks/split_*.dat";
# my $tagname = "wh_nom_aug07.20090823.v1";
# my $split_list_command = "samples/whzero_hwndiff9/split_*.dat";
# my $tagname = "wh_hwndiff9.20090824.v1";
# my $split_list_command = "samples/splitarch_2miss_whzero_nom_20090907/split_*.dat";
# my $tagname = "splitarch_2miss_whzero_nom.20090923.v1";
# my $split_list_command = "samples/tsp_anton_13P5_20090921/split_*.dat";
# my $tagname = "tsp_anton_13P5_20090921.20090924.v1";
# my $split_list_command = "samples/tsp_anton_7P5_20090921/split_*.dat";
# my $tagname = "tsp_anton_7P5_20090921.20090924.v1";
# my $split_list_command = "samples/splitarch_whbbzero_nom_20090907/split_*.dat";
# my $tagname = "splitarch_whzero_nom.20090907.v1";
# my $split_list_command = "samples/whzero_anton_tracks/split_*.dat";
# my $tagname = "wh_hwdiff3.20090824.v1";
# my $split_list_command = "samples/whzero_anton_tracks/split_*.dat";
# my $tagname = "wh_nom.20090828.v1";
# my $split_list_command = "samples/whzero_aug28_cm/split_*.dat";
# my $tagname = "wh_aug28_cm.20090828.v4";
# my $split_list_command = "samples/whzero_hwstudies_8hw0/split_*.dat";
# my $tagname = "whzero_hwstudies_8hw0.v3";
# my $split_list_bb_command = "samples/WH120bb1E34/split_*.dat";
# my $split_list_uu_command = "samples/WH120uu1E34/split_*.dat";
# my $split_list_bb_command = "samples/20091122_test_wh3e34/split_*bb.dat";
# my $split_list_uu_command = "samples/20091122_test_wh3e34/split_*uu.dat";
# my $tagname = "WH1203E34v14test.20091122.v3";
my $datestring = `date --iso-8601`;
chomp $datestring;
$datestring =~ s/-//g;

my $programname = "ana_b_tagging";

my $nbits = `file ana/${programname} | awk '{print \$3}' | cut -d '-' -f 1`;
chomp( $nbits );
my $uct3_64 = ($nbits == 64 ? 1 : 0);

my $lumi = "3e34";
my $sample = "";
my $tagname = "";
my $anaoptions = "--absetacut 1.2 --mcut 0.1 --jetdr 0.3 --jetdz0prune 2.0"; # change |dz0| < 5 (march cut) to 2 mm (tp cut)
GetOptions( "s=s" => \$sample ,
            "t=s" => \$tagname );
die "Specify sample with -s" unless ( $sample ne "" );
if( $tagname eq "" ) {
    for( my $i=0; $i<30; ++$i ) { 
        my $bsample = basename($sample);
        $tagname = "tag_${bsample}_${datestring}_v${i}";
        if( !(-d "/share/ftkdata/boveia/tagging/$tagname") ) { last; }
    }
}
################################################################
my $doTraining = 1;
my $doTagging = 1;
my $nsplits = 180;
my $jobtimestr = "07:59:59";

my $submitdir = "/share/ftkdata/boveia/tagging/${tagname}";
chomp( $submitdir );
if( !-e $submitdir ) { 
    $shell->mkdir( "-p $submitdir" ); 
} else { 
    print "${submitdir} already exists.\n";
    if( $doTraining != 0 ) { 
        die;
    }
    print "overwriting tagging files.\n";
}
my $training_file = "${tagname}";
my $trainlist = "training_files.list";
my $trainrootlist = "training_root_files.list";
my $tagrootlist = "tagging_root_files.list";

my $ana_options_training = "--train-tagger --read-offline-si-clusters false --read-offline-si-channels false";
my $ana_options_tagging = "--trained-tagger-file ${submitdir}/${tagname} --read-offline-si-clusters false --read-offline-si-channels false";

my $pbsclient = PBS::Client->new();

my @train_cmds;
my @tag_cmds;
my @merge_dependencies;
my @outmerge_dependencies;

my $logdir1 = "${submitdir}/training.log/";
chomp( $logdir1 );
if( $doTraining == 1 ) {
    print "defining b tagging training jobs.\n";
    if( !-e $logdir1 ) { 
        $shell->mkdir( "-p $logdir1" ); 
    }
    # my $n = 0;
    # foreach my $inname (@splits_training) {
    #     ++$n;
    for( my $n=0; $n!=$nsplits; ++$n ) {
        my $inname = $sample;
        chomp( $inname );
        my $bname = basename( $inname );
        $bname = "${bname}_${n}";
        my $batchscriptname = "$submitdir/training_$bname.sh";
        my $outrootname = "$submitdir/training_$bname.root";
        my $outtaggername = "$submitdir/$bname.tagger";
        my $bsfh = FileHandle::new();
        my $sleepseconds = 60 - ((1*$n)%60);
        if( $sleepseconds < 0 ) { $sleepseconds = 1; }
        open $bsfh, ">$batchscriptname";
        print $bsfh <<ENDBATCHSCRIPT;
#!/bin/bash
sleep $sleepseconds
cd $rundir
#uct3_64=${uct3_64} source ../setup_standalone.sh
uct3_64=${uct3_64} source ./setup.sh
${programname} -i $inname ${ana_options_training} --trained-tagger-file $outtaggername --force-output-filename $outrootname --noROOT --nsplits ${nsplits} --split ${n} ${anaoptions}
ENDBATCHSCRIPT
# ${programname} -i $inname ${ana_options_training} --trained-tagger-file $outtaggername --force-output-filename $outrootname --noROOT ${anaoptions}
close $bsfh;
        $shell->chmod( "u+x $batchscriptname" );
        push( @train_cmds , ${batchscriptname} );
    }
}
# specify the training jobs
#my $jobid = $shell->qsub("-N tagana_train -j oe -o $batchlogname $batchscriptname");
my $training_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => $logdir1,
    efile => $logdir1,
    name => "tagana_train",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => \@train_cmds
    );

# submit merge jobs
my $mergescriptname = "$submitdir/merge_training.sh";
if( $doTraining == 1 ) {
    print "defining b tagging training merge job.\n";
    my $msfh = FileHandle::new();
    open $msfh, ">$mergescriptname";
    print $msfh <<ENDMERGESCRIPT;
#!/bin/bash
sleep 1
cd $rundir
#uct3_64=${uct3_64} source ../setup_standalone.sh
uct3_64=${uct3_64} source ./setup.sh
echo ${submitdir}/*.tagger | xargs ls -1 > ${submitdir}/${trainlist}
echo ${submitdir}/training*.root | xargs ls -1 > ${submitdir}/${trainrootlist}
merge_trained_tagger -l ${submitdir}/${trainlist} -o ${submitdir}/${tagname} --noparam
dgplot -m ${submitdir}/training -q -l ${submitdir}/${trainrootlist}
ENDMERGESCRIPT
close $msfh;
    $shell->chmod( "u+x $mergescriptname" );
}
# specify the training merge job
my $training_merge_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => "${submitdir}/merge_training.out.log",
    efile => "${submitdir}/merge_training.err.log",
    name => "tagana_train_merge",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => $mergescriptname
    );

# submit tagging jobs
my $logdir2 = "${submitdir}/tagging.log/";
chomp( $logdir2 );
print "defining b tagging jobs.\n";
if( $doTagging == 1 ) {
    if( !-e $logdir2 ) { 
        $shell->mkdir( "-p $logdir2" ); 
    }
    # my $nt = 0;
    # foreach my $intname (@splits_tagging) {
    #     ++$nt;
    for( my $nt=0; $nt!=$nsplits; ++$nt ) {
        my $intname = $sample;
        chomp( $intname );
        my $btname = basename( $intname );
        $btname = "${btname}_${nt}";
        my $batchscripttname = "$submitdir/tagging_$btname.sh";
        my $outroottname = "$submitdir/tagging_$btname.root";
        my $bsfh = FileHandle::new();
        my $sleepseconds = (1*$nt) % 30;
        open $bsfh, ">$batchscripttname";
        print $bsfh <<ENDBATCHSCRIPTT;
#!/bin/bash
sleep $sleepseconds
cd $rundir
#uct3_64=${uct3_64} source ../setup_standalone.sh
uct3_64=${uct3_64} source ./setup.sh
${programname} -i $intname ${ana_options_tagging} --force-output-filename $outroottname --noROOT --nsplits ${nsplits} --split ${nt} ${anaoptions}
ENDBATCHSCRIPTT
#${programname} -i $intname ${ana_options_tagging} --force-output-filename $outroottname --noROOT ${anaoptions}
close $bsfh;
        $shell->chmod( "u+x $batchscripttname" );
        push( @tag_cmds , $batchscripttname );
    }
}
# specify the tagging jobs
#my $jobid = $shell->qsub("-N tagana_train -j oe -o $batchlogname $batchscriptname");
my $tagging_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => $logdir2,
    efile => $logdir2,
    name => "tagana_tag",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => \@tag_cmds
    );

# submit outmerge jobs
my $outmergescriptname = "$submitdir/merge_tagging.sh";
if( $doTagging == 1 ) {
    print "defining b tagging merge job.\n";
    my $outmerge_dependency_string = join(":",@outmerge_dependencies);
    my $omsfh = FileHandle::new();
    open $omsfh, ">$outmergescriptname";
    print $omsfh <<ENDOUTMERGESCRIPT;
#!/bin/bash
sleep 1
cd $rundir
#uct3_64=${uct3_64} source ../setup_standalone.sh
uct3_64=${uct3_64} source ./setup.sh
echo ${submitdir}/tagging*.root | xargs ls -1 > ${submitdir}/${tagrootlist}
dgplot -m ${submitdir}/tagging -q -l ${submitdir}/${tagrootlist}
ENDOUTMERGESCRIPT
close $omsfh;
    $shell->chmod( "u+x $outmergescriptname" );
}
# specify the training merge job
my $tagging_merge_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => "${submitdir}/merge_tagging.out.log",
    efile => "${submitdir}/merge_tagging.err.log",
    name => "tagana_tag_merge",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => $outmergescriptname
    );
#my $jobid_outmerge = $shell->qsub("-N tagana_merge -W depend=afterany:${outmerge_dependency_string} -j oe -o $submitdir/merge_tagging.log $outmergescriptname");

my $notify_jobspec;
if( $thisusername eq "boveia" ) {
        # add an email notification when the job finishes
        my $notifyscriptname = "${submitdir}/notify_tagging.sh";
        my $msfh = FileHandle::new();
        open $msfh, ">$notifyscriptname";
        print $msfh <<ENDNOTIFYSCRIPT;
ssh hep \"echo \\\"tagging ${submitdir} complete.\\\" | mail -s \\\"tagging job complete\\\" boveia+notice\@me.com\"
ENDNOTIFYSCRIPT
        close $msfh;
        system( "chmod u+x $notifyscriptname" );
        $notify_jobspec = PBS::Client::Job->new(
            queue => "uct3",
            ofile => "${submitdir}/notify_top.out.log",
            efile => "${submitdir}/notify_top.err.log",
            name => "top_notify",
            cput => $jobtimestr,
            wallt => $jobtimestr,
            cmd => $notifyscriptname
            );
}


print "submitting all jobs...\n";
if( $doTraining==1 && $doTagging==1 ) {
    $training_jobspec->pack( numQ => 70 );
    $tagging_jobspec->pack( numQ => 70 );
    $training_jobspec->next( { end=>$training_merge_jobspec } );
    $training_merge_jobspec->next( { end=>$tagging_jobspec } );
    $tagging_jobspec->next( { end=>$tagging_merge_jobspec } );
    if( $thisusername eq "boveia" ) {
        $tagging_merge_jobspec->next( { end=>$notify_jobspec } );
    }
    $pbsclient->qsub( $training_jobspec );
    print "when finished, run\n";
    print "  dgplot ${submitdir}/training.root\n\n";
    print "  dgplot ${submitdir}/tagging.root\n\n";
    if( $thisusername eq "boveia" ) {
        my $timeestimate = "now + 2 hour";
        system( "ssh hep.uchicago.edu \"echo \\\"dgplot ${submitdir}/tagging.root\\\" | mail -s \\\"check tagger analysis plots of $sample #$timeestimate #$timeestimate //automatic entry from submit_tagging.pl\\\" boveia+todo\@me.com \"" );
    }
} elsif( $doTraining==1 && $doTagging==0 ) {
    $training_jobspec->pack( numQ => 30 );
    $training_jobspec->next( { end=>$training_merge_jobspec } );
    # $training_merge_jobspec->prev( { end=>$training_jobspec } );
    if( $thisusername eq "boveia" ) {
        $training_merge_jobspec->next( { end=>$notify_jobspec } );
    }
    $pbsclient->qsub( $training_jobspec );
    print "when finished, run\n";
    print "  dgplot ${submitdir}/training.root\n\n";
    if( $thisusername eq "boveia" ) {
        my $timeestimate = "now + 2 hour";
        system( "ssh hep.uchicago.edu \"echo \\\"dgplot ${submitdir}/training.root\\\" | mail -s \\\"check tagger analysis plots of $sample #$timeestimate #$timeestimate //automatic entry from submit_tagging.pl\\\" boveia+todo\@me.com \"" );
    }
} elsif( $doTraining==0 && $doTagging==1 ) {
    $tagging_jobspec->pack( numQ => 30 );
    $tagging_jobspec->next( { end=>$tagging_merge_jobspec } );
    if( $thisusername eq "boveia" ) {
        $tagging_merge_jobspec->next( { end=>$notify_jobspec } );
    }
    $pbsclient->qsub( $tagging_jobspec );
    print "when finished, run\n";
    print "  dgplot ${submitdir}/tagging.root\n\n";
    if( $thisusername eq "boveia" ) {
        my $timeestimate = "now + 2 hour";
        system( "ssh hep.uchicago.edu \"echo \\\"dgplot ${submitdir}/tagging.root\\\" | mail -s \\\"check tagger analysis plots of $sample #$timeestimate #$timeestimate //automatic entry from submit_tagging.pl\\\" boveia+todo\@me.com \"" );
    }
}


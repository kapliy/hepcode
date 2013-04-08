#!/usr/bin/perl -w

use lib '/share/ftkdata/boveia/perl/lib/perl5/site_perl/5.8.5/i386-linux-thread-multi/'; # for PBS::Client
use strict;
use FileHandle;
use File::Basename;
use PBS::Client;

my $classname = "TreeSingleTopD3PD";
my $datadefcode = "DSGTOP";
my @datasets = `ls ./datasets/SGTOP/*.dat`;
my @treenames = ( "TopInputsTree" , "InfoTree" );
my @submitcmds;

foreach my $dataset (@datasets) {
    chomp $dataset;
    my $dname = basename($dataset);
    $dname =~ s/\.dat$//;
    my $shellscript = "./make_ttree_${dname}_makeclass.sh";
    my $rootscript = "./make_ttree_${dname}_makeclass.C";
    my $fh = FileHandle::new();
    open $fh, ">${rootscript}";
    # get root filename to read
    my $rootfilename = `grep "^${datadefcode}" ${dataset}| awk '{print \$2}' | head -n 1`;
    chomp $rootfilename;
    if( $rootfilename =~ m/^\/pnfs/ ) {
        $rootfilename = "dcache:${rootfilename}";
    }
    print $fh "{\n";
    print $fh "TFile::Open(\"${rootfilename}\");\n";
    foreach my $treename (@treenames) {
        my $tname = $treename;
        $tname =~ s/Tree$//;
        my $thisclassname = "${classname}${tname}_${dname}";
        print $fh "${treename}->MakeClass(\"${thisclassname}\");\n";
    }
    print $fh "}\n";
    close $fh;
    my $fhs = FileHandle::new();
    open $fhs, ">${shellscript}";
    print $fhs "#!/bin/bash\n";
    print $fhs "cd /share/ftkdata1/boveia/testana/TrigFTKAna/\n";
    print $fhs "source ./setup.sh\n";
    print $fhs "root -b -q ${rootscript}\n";
    close $fhs;
    push @submitcmds , "$shellscript";
}
system( "chmod u+x make_ttree*makeclass.sh" );

# my $logdir = "./";
# my
my $jobspec = PBS::Client::Job->new(
    queue => "uct3",
    # ofile => $logdir,
    # efile => $logdir,
    name => "makeclass",
    # cput => $jobtimestr,
    # wallt => $jobtimestr,
    cmd => \@submitcmds
    );

my $pbsclient = PBS::Client->new();
$pbsclient->qsub( $jobspec );



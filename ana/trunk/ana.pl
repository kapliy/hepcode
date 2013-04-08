#!/usr/bin/perl -w 

# from bash, 
#   eval $(perl -I/share/t3data/ftkdata/ftkdata/boveia/perl/lib/perl5 -Mlocal::lib=/share/t3data/ftkdata/ftkdata/boveia/perl)
# to set up local (UC Tier3) perl modules such as PBS::Client.

use strict;
use warnings;
use FileHandle;
use File::Basename;
use File::Slurp;
use Getopt::Long qw(GetOptionsFromArray);
use PBS::Client;

my $rundir = `echo \${PWD}`;
chomp( $rundir );
# make sure to use paths like /share/t3data/ftkdata/ftkdata1 instead
# of the symlinks /share/ftkdata1
$rundir =~ s/\/share\/ftkdata/\/share\/t3data\/ftkdata\/ftkdata/;

# if this is not a Tier 3 edge node, ssh into one before submitting
my $thisnode = `hostname`;
if( $thisnode =~ m/uct3\-c/ ) {
    print "this is a Tier 3 worker node. attempt to ssh to edge node for submission.\n";
    print "if this fails, run your submission command on the edge node directly.\n";
    my $cmdline = "@ARGV";#join(@ARGV,' ');
    print "command line is: @ARGV\n";
    # system( "ssh uct3-edge3.uchicago.edu \"hostname; eval \$(perl -I/share/t3data/ftkdata/ftkdata/boveia/perl/lib/perl5 -Mlocal::lib=/share/t3data/ftkdata/ftkdata/boveia/perl); cd ${rundir}; ./ana.pl $cmdline\"" );
    system( "ssh -A uct3-edge3.uchicago.edu \"hostname; export PATH=/usr/local/bin:\${PATH}; cd ${rundir}; ./ana.pl $cmdline\"" );
    exit;
}


my $datestring = `date --iso-8601`;
chomp $datestring;
$datestring =~ s/-//g;

my $thisusername = `whoami`;
chomp $thisusername;
my $programname = "ana_lowmh";

my $smartrunner = "./smart_runner.sh"; # path to Anton's smart_runner.sh

my $tagname = "";
my $outputdirbase = "/share/t3data/ftkdata/ftkdata1/${thisusername}/ana_lowmh";

my $nsplits = 24;
my $jobtimestr = "07:59:59";

my $ana_options = "";
my $ntuple_directory = "";

my $single_dataset = "";
my $dataset_list_filename = "";
#ana_standard_overview -i ~/datasets/WRAPPER/data/data10_minbias_r1647_p306_run155697.in
#$ana_options = "--study-si-channels true --study-si-clusters true --offline-read wrap";
$nsplits = 60;

my @datasets = ();
GetOptionsFromArray( \@ARGV , "p=s" => \$programname , "i=s" => \@datasets , "f=s" => \$dataset_list_filename , "t=s" => \$tagname , "options=s" => \$ana_options, "n=i" => \$nsplits, "xml=s" => \$ntuple_directory , "odir=s" => \$outputdirbase );

chomp( $dataset_list_filename );
chomp( $tagname );

if( ! ($programname =~ m/^ana\_/ ) ) {
    $programname = "ana_$programname";
}
my $prognames = `ls -1 ./ana/ana_* | grep -v '\.c'`;
die "Provide a valid program name.\n" unless ( $prognames =~ m/${programname}/ );

if( $programname =~ m/standard_overview/ ) {
    $ana_options = "${ana_options} --study-si-channels true --study-si-clusters true --offline-read wrap";
}

my $nbits = `file ana/${programname} | awk '{print \$3}' | cut -d '-' -f 1`;
chomp( $nbits );
my $uct3_64 = ($nbits == 64 ? 1 : 0);

my $dontuple = 0;
if( $ntuple_directory ne "" ) { $dontuple = 1; }

# construct and check list of datasets
my $nsingledat = scalar(@datasets);
die "Provide one or more dataset definition files (-i filename) or a file contain a list of these (-f listname)" unless ( (${nsingledat} > 0) || (${dataset_list_filename} ne "") );
#if( $single_dataset ne "" ) { push( @datasets , $single_dataset ); }
# validate singly-specified datasets
foreach my $tmpd (@datasets) {
    chomp $tmpd;
    if( !-e $tmpd ) { die "error: $tmpd does not exist.\n"; }
}
# check file datasets
if( ${dataset_list_filename} ne "" ) {
    my @tmplist = `cat ${dataset_list_filename}`;
    foreach my $tmp (@tmplist) {
        chomp $tmp;
        if( $tmp =~ m/^\#/ ) { next; } # skip commented-out datasets
        if( !-e $tmp ) { print "warning: $tmp does not exist.\n"; }
        push( @datasets , $tmp );
    }
}

# find unique dirname to store output from all datasets
if( $tagname eq "" ) { $tagname = "$datestring"; }
my $outputdir = "${outputdirbase}/${tagname}";
for( my $i=1; $i<=20; ++$i ) {
    if( $i>1 ) { $outputdir = "${outputdirbase}/${tagname}.v${i}"; }
    if( !(-d "${outputdir}") ) { last; }
    if( $i==20 ) { die "20 submissions with this tagname already exist. Choose another.\n"; }
}

my $pbsclient = PBS::Client->new();

my $dgplottext = "";
my $listofoutput = "";
my $outputregname = "${outputdir}/register.txt";
my $outputretryname = "${outputdir}/retry.txt";

my @data_egamma_merge_dependencies;
my @data_muons_merge_dependencies;
my @anarootfiles_egamma;
my @anarootfiles_muons;

foreach my $dataset (@datasets ) {
    chomp $dataset;
    my $tmpdname = ${dataset};
    $tmpdname =~ s/\/home\/${thisusername}\/datasets//;
    my $submitdir = "${outputdir}/$tmpdname";
    chomp( $submitdir );
    die " ${submitdir} already exists" unless !(-d ${submitdir});
    system( "mkdir -p ${submitdir}" );
    #print "preparing ${programname} on ${dataset}.\n";

    my @ana_cmds;
    my @merge_dependencies;
    my $logdir = "${submitdir}/logs/";
    if( !-d $logdir ) { system("mkdir -p $logdir"); }

    my @anarootfiles;
    my $thisnsplits = $nsplits;
    if( $dataset =~ m/McAtNlo_JIMMY_H.*_WpWm_lnulnu.WZphys.101222b/ ) {
        $thisnsplits = 2;
    } elsif( $dataset =~ m/periodH/ ) {
        $thisnsplits = 40;
    }

    for( my $n=0; $n!=$thisnsplits; ++$n ) {
        my $inname = $dataset;
        chomp( $inname );
        my $bname = basename( $inname );
        my $batchscriptname = "$submitdir/${bname}_${n}.sh";
        my $batchlogname = "$submitdir/${bname}_${n}.log";
        my $outrootname = "$submitdir/${bname}_${n}";
        my $ntupleout = "";
        if( $dontuple ) {
            $ntupleout = "--out-ntuple ${ntuple_directory}/ntuple_${n}.aen";
        }
        push( @anarootfiles , "${outrootname}.root" );
        my $bsfh = FileHandle::new();
        my $sleepseconds = (1*$n) % 30;
        open $bsfh, ">$batchscriptname";
        print $bsfh <<ENDBATCHSCRIPT;
#!/bin/bash
sleep $sleepseconds
cd $rundir
uct3_64=${uct3_64} source ./setup.sh
echo \$HOME
export HOME=/home/${thisusername}
${smartrunner} ${programname} ${ana_options} -i $inname --force-output-filename $outrootname --noROOT --nsplits ${thisnsplits} --split ${n} ${ntupleout}
ENDBATCHSCRIPT
        close $bsfh;
        system( "chmod u+x ${batchscriptname}" );
        push( @ana_cmds , $batchscriptname );
   #    system( "qsub -l walltime=07:59:59 -N standard -j oe -o $batchlogname $batchscriptname" );
    }

# specify the ana jobs
my $ana_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => $logdir,
    efile => $logdir,
    name => "${programname}",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => \@ana_cmds
    );
# prepare the merge
    my $mergescriptname = "${submitdir}/merge_ana.sh";
    my $mergefile = "${submitdir}/${tagname}";
    my $firstsampledesc = $anarootfiles[0];
    $firstsampledesc =~ s/\.root$/\.xml/;
    my $mergedesc = "${submitdir}/${tagname}.xml";
    # print "defining ${programname} on ${dataset} merge job.\n";
    my $msfh = FileHandle::new();
    open $msfh, ">$mergescriptname";
    print $msfh <<ENDMERGESCRIPT;
#!/bin/bash
sleep 1
cd $rundir
uct3_64=${uct3_64} source ./setup.sh
echo @{anarootfiles} | xargs ls -1 > ${submitdir}/${programname}.root.list
NEXPECTED=`echo @{anarootfiles}|wc|awk '{print \$2}'`
NFOUND=`echo @{anarootfiles}|xargs ls -1 2>&1 | grep -v 'No such file or directory' | wc | awk '{print \$1}'`
if [ \$NEXPECTED == \$NFOUND ]; then
  dgplot -m ${mergefile} -q -l ${submitdir}/${programname}.root.list
  if [ -e ${mergefile}.root ]; then
    echo "${dataset} success" >> ${outputregname}
  else
    echo "${dataset} FAILURE(merge)" >> ${outputregname}
  fi
else
  MISSING=`echo @{anarootfiles}|xargs ls -1 2>&1 | grep 'No such file or directory' | awk -F':' '{printf(\" %s\",\$2);}'`
  echo "${dataset} FAILURE(missing \$MISSING)" >> ${outputregname}
  RETRIES=`echo \$MISSING | sed 's/\.root/\.sh/g'`
  echo "\$RETRIES" >> ${outputretryname}
fi
ENDMERGESCRIPT
close $msfh;
system( "chmod u+x $mergescriptname" );
my $merge_jobspec = PBS::Client::Job->new(
    queue => "uct3",
    ofile => "${submitdir}/merge_${programname}.out.log",
    efile => "${submitdir}/merge_${programname}.err.log",
    name => "${programname}_merge",
    cput => $jobtimestr,
    wallt => $jobtimestr,
    cmd => $mergescriptname
    );

    print "submitting ${dataset}...\n";
#    $ana_jobspec->pack( numQ => 32 );
    $ana_jobspec->next( { end=>$merge_jobspec } );

    my $merge_later = 0;
    if( $dataset =~ m/data11/ ) {
        if( $dataset =~ m/egamma/i ) {
            push( @data_egamma_merge_dependencies , $merge_jobspec );
            push( @anarootfiles_egamma , "${mergefile}.root" );
            $merge_later = 1;
            # print "merge of electron data will include ${dataset}\n";
        } elsif( $dataset =~ m/muons/i ) {
            push( @data_muons_merge_dependencies , $merge_jobspec );
            push( @anarootfiles_muons , "${mergefile}.root" );
            $merge_later = 1;
            # print "merge of muon data will include ${dataset}\n";
        }
    }

# if( $thisusername eq "boveia" ) {
#     # add an email notification when the job finishes
#     my $notifyscriptname = "${submitdir}/notify_${programname}.sh";
#     my $msfhn = FileHandle::new();
#     open $msfhn, ">$notifyscriptname";
#     print $msfhn <<ENDNOTIFYSCRIPT;
# ssh hep \"echo \\\"${mergefile}.root complete.\\\" | mail -s \\\"${programname} job complete\\\" boveia+notice\@me.com\"
# ENDNOTIFYSCRIPT
#     close $msfhn;
#     system( "chmod u+x $notifyscriptname" );
# #!/bin/bash
#     my $notify_jobspec = PBS::Client::Job->new(
#         queue => "uct3",
#         ofile => "${submitdir}/notify_${programname}.out.log",
#         efile => "${submitdir}/notify_${programname}.err.log",
#         name => "${programname}_notify",
#         cput => $jobtimestr,
#         wallt => $jobtimestr,
#         cmd => $notifyscriptname
#         );
#     $merge_jobspec->next( { end=>$notify_jobspec } );
# }

    # if( $merge_later == 0 ) {
        $pbsclient->qsub( $ana_jobspec );
    # }
     $dgplottext .= "\ndgplot ${mergefile}.root";
     $listofoutput .= "\n${dataset} in ${submitdir} with ${mergefile}.root";

    # pause 
    sleep 2;
}

# submit final data merges.
if( scalar(@data_egamma_merge_dependencies) > 0 ) {
# prepare the merge
    my $submitdir = "${outputdir}/data11_egamma";
    system( "mkdir -p ${submitdir}" );
    my $mergescriptname = "${submitdir}/merge_ana.sh";
    my $mergefile = "${submitdir}/${tagname}";
    my $msfh = FileHandle::new();
    open $msfh, ">$mergescriptname";
    print $msfh <<ENDMERGESCRIPTDE;
#!/bin/bash
sleep 1
cd $rundir
uct3_64=${uct3_64} source ./setup.sh
echo @{anarootfiles_egamma} | xargs ls -1 > ${submitdir}/${programname}.root.list
NEXPECTED=`echo @{anarootfiles_egamma}|wc|awk '{print \$2}'`
NFOUND=`echo @{anarootfiles_egamma}|xargs ls -1 2>&1 | grep -v 'No such file or directory' | wc | awk '{print \$1}'`
if [ \$NEXPECTED == \$NFOUND ]; then
  dgplot -m ${mergefile} -q -l ${submitdir}/${programname}.root.list
  if [ -e ${mergefile}.root ]; then
    echo "data11 egamma success" >> ${outputregname}
  else
    echo "data11 egamma FAILURE(merge)" >> ${outputregname}
  fi
else
  MISSING=`echo @{anarootfiles_egamma}|xargs ls -1 2>&1 | grep 'No such file or directory' | awk -F':' '{printf(\" %s\",\$2);}'`
  echo "data11 egamma FAILURE(missing \$MISSING)" >> ${outputregname}
fi
ENDMERGESCRIPTDE
    close $msfh;
    system( "chmod u+x $mergescriptname" );
    
    # my $egamma_merge_jobspec = PBS::Client::Job->new(
    #     queue => "uct3",
    #     ofile => "${submitdir}/merge_${programname}.out.log",
    #     efile => "${submitdir}/merge_${programname}.err.log",
    #     name => "${programname}_egamma_merge",
    #     cput => $jobtimestr,
    #     wallt => $jobtimestr,
    #     cmd => $mergescriptname
    #     );

    #print "submitting data11 egamma...\n";

    # foreach my $dep (@data_egamma_merge_dependencies) {
    #     $egamma_merge_jobspec->prev( { end=>$dep } );
    # }
    # $pbsclient->qsub( $egamma_merge_jobspec );
    #$dgplottext .= "\ndgplot ${mergefile}.root";
    $listofoutput .= "\ndata11 egamma merge\n ${mergescriptname}";
    $listofoutput .= "\ndata11 egamma in ${submitdir} with ${mergefile}.root";
}

if( scalar(@data_muons_merge_dependencies) > 0 ) {
# prepare the merge
    my $submitdir = "${outputdir}/data11_muons";
    system( "mkdir -p ${submitdir}" );
    my $mergescriptname = "${submitdir}/merge_ana.sh";
    my $mergefile = "${submitdir}/${tagname}";
    my $msfh = FileHandle::new();
    open $msfh, ">$mergescriptname";
    print $msfh <<ENDMERGESCRIPTDE;
#!/bin/bash
sleep 1
cd $rundir
uct3_64=${uct3_64} source ./setup.sh
echo @{anarootfiles_muons} | xargs ls -1 > ${submitdir}/${programname}.root.list
NEXPECTED=`echo @{anarootfiles_muons}|wc|awk '{print \$2}'`
NFOUND=`echo @{anarootfiles_muons}|xargs ls -1 2>&1 | grep -v 'No such file or directory' | wc | awk '{print \$1}'`
if [ \$NEXPECTED == \$NFOUND ]; then
  dgplot -m ${mergefile} -q -l ${submitdir}/${programname}.root.list
  if [ -e ${mergefile}.root ]; then
    echo "data11 muons success" >> ${outputregname}
  else
    echo "data11 muons FAILURE(merge)" >> ${outputregname}
  fi
else
  MISSING=`echo @{anarootfiles_muons}|xargs ls -1 2>&1 | grep 'No such file or directory' | awk -F':' '{printf(\" %s\",\$2);}'`
  echo "data11 muons FAILURE(missing \$MISSING)" >> ${outputregname}
fi
ENDMERGESCRIPTDE
    close $msfh;
    system( "chmod u+x $mergescriptname" );
    
    # my $muons_merge_jobspec = PBS::Client::Job->new(
    #     queue => "uct3",
    #     ofile => "${submitdir}/merge_${programname}.out.log",
    #     efile => "${submitdir}/merge_${programname}.err.log",
    #     name => "${programname}_muons_merge",
    #     cput => $jobtimestr,
    #     wallt => $jobtimestr,
    #     cmd => $mergescriptname
    #     );

    # print "submitting data11 muons...\n";

    # foreach my $dep (@data_muons_merge_dependencies) {
    #     $muons_merge_jobspec->prev( { end=>$dep } );
    # }
    # $pbsclient->qsub( $muons_merge_jobspec );
    #$dgplottext .= "\ndgplot ${mergefile}.root";
    $listofoutput .= "\ndata11 muons merge\n ${mergescriptname}";
    $listofoutput .= "\ndata11 muons in ${submitdir} with ${mergefile}.root";
}

print "all datasets submitted.\n";
print "================================================================\n";

my $fhoutputdesc = FileHandle::new();
my $outputdescname = "${outputdir}/description.txt";
open $fhoutputdesc, ">$outputdescname";
print $fhoutputdesc "TrigFTKAna ${programname} run submitted $datestring to ${outputdir}:\n";
print $fhoutputdesc "----------------------------------------------------------------\n";
print $fhoutputdesc "command line options: ${ana_options}\n";
print $fhoutputdesc "AnaEventNtuples?: $dontuple\n";
print $fhoutputdesc "----------------------------------------------------------------\n";
print $fhoutputdesc "list of input datasets:TrigFTKAna ${programname} run submitted $datestring to ${outputdir}:\n";
print $fhoutputdesc "$listofoutput\n";
print $fhoutputdesc "----------------------------------------------------------------\n";
print $fhoutputdesc "dgplot command lines to view:\n";
print $fhoutputdesc "   $dgplottext\n";
print $fhoutputdesc "dgplot command line to view all:\n";
my $tmpdgplot = $dgplottext;
$tmpdgplot =~ s/dgplot//g;
$tmpdgplot =~ s/\n/ /g;
print $fhoutputdesc "   dgplot $tmpdgplot\n";
# my @dgtotallist = ( "#!/bin/bash\n" );
#$tmpdgplot =~ s/ /\n /g;
my @dgtotallist = split( ' ' , $tmpdgplot );
my $dgtotallistname = "${outputdir}/all.list";
write_file( $dgtotallistname , @dgtotallist );
# print $fhoutputdesc "\n or source\n     $dgcommandscriptname\n";
# print $fhoutputdesc "----------------------------------------------------------------\n";
close $fhoutputdesc;


print "when jobs have finished, \n";
print "   less ${outputdescname}\n";
print "for a description of the submission and commands to access the output.\n";
print "\n or run \n";
print " dgplot -l $dgtotallistname\nto load everything.\n";

if( $thisusername eq "boveia" ) {
   my $timeestimate = "now + 1 hour";
   # system( "ssh hep.uchicago.edu \"echo \\\"cat $outputdescname\\\" | mail -s \\\"check ${programname} analysis plots #$timeestimate #$timeestimate //automatic entry from ana.pl\\\" boveia+todo\@me.com \"" );
       my $outstring = $outputdescname;
        $outstring =~ s/\n/\\n/g;
        system( "ssh hep.uchicago.edu \"echo ${outstring} | mail -s \\\"check ${programname} analysis plots #$timeestimate #$timeestimate //automatic entry from ana.pl\\\" boveia+todo\@me.com \"" );
}

print "done submitting.\n";

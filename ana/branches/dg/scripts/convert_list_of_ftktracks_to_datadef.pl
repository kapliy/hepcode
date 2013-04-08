#!/usr/bin/perl -w

use Getopt::Long;
use FileHandle;
use File::Basename;

my $input_list = "";
my $output_def = "dataset.dat";
my $include_cbnt = 0;
my $include_wrap = 0;
my $include_roads = 0;
my $include_btag = 0;
my $input_sample_dir ="";
my $nperfile = 0;

GetOptions( "i=s" => \${input_list} , "o=s" => \${output_def} ,
            "n=i" => \$nperfile ,
            "cbnt=i" => \$include_cbnt , "wrap=i" => \$include_wrap ,
            "road=i" => \$include_roads , "btag=i" => \$include_btag ,
            "s=s" => \$input_sample_dir );

die "Specify input list with -i\n" unless ${input_list} ne "";
die "Specify number of events per file -n\n" unless $nperfile != 0;
die "Specify output dataset filename with -o\n" unless ${output_def} ne "";
die "Specify sample dir with -s\n" unless (${input_sample_dir} ne "") || ($include_cbnt==0 && $include_wrap==0);
die "Sample directory ${input_sample_dir} does not exist" unless (-d ${input_sample_dir});

my $fhout = FileHandle::new();
open $fh, ">${output_def}";

my @ftktracks_filenames = `cat ${input_list}`;

my $wrap_last_dset = "";
my $cbnt_last_dset = "";
my $btag_last_dset = "";
my $cbnt_ext = -1;
my $wrap_ext = -1;
my $btag_ext = -1;

my $nevents = 0;

print $fh "V 1\n";

foreach my $ftktracks_filename (@ftktracks_filenames) {
    chomp $ftktracks_filename;
    # number of events in the file
    print $fh "N $nperfile\n";
    $nevents += $nperfile;
    # FTK merged output tracks
    print $fh "T ${ftktracks_filename} M\n";
    # compute some filename stuff
    my $setname = basename( ${ftktracks_filename} );
    $setname =~ s/^ftktracks_//;
    my $fileindex = "";
    if( $setname =~ m/rset(\d*)\.(\d*)\.EXT\d\._(\d*)\.ftksim_wrap_raw\.root/ ) {
        $setname =~ s/rset(\d*)\.(\d*)\.EXT\d\._(\d*)\.ftksim_wrap_raw\.root/rset$1\.$2/;
        $fileindex = $3;
    } elsif( $setname =~ m/FTKtrainingmuons\.set(\d*)/ ) {
        $setname =~ s/FTKtrainingmuons\.set(\d*)([A-Za-z0-9]*)\.([A-Za-z0-9]*)\.EXT\d\._(\d*)\.ftksim_wrap_raw\.root/FTKtrainingmuons\.set$1$2\.$3/;
        $fileindex = $4;
    }
    #print "$setname\n";
    my $setdir = "${input_sample_dir}/${setname}";
    #print "$setdir\n";
    # wrapper file if requested
    if( $include_wrap ) {
        # figure out extension number and test for file existence.
        my $wrap_filename = "";
        if( $setdir ne ${wrap_last_dset} || ${wrap_ext}<0 || ${wrap_ext}>=10 ) {
            for( ${wrap_ext} = 0; ${wrap_ext}<=10; ++${wrap_ext} ) {
                $wrap_filename = "${setdir}/${setname}.EXT${wrap_ext}._${fileindex}.ftksim_wrap_raw.dat.bz2";
                if( -e ${wrap_filename} ) { last; }
            }
            if( ${wrap_ext}<0 || ${wrap_ext}>=10 ) {
                print "could not figure out wrapper file extension for ${ftktracks_filename}\n";
            } else {
                ${wrap_last_dset} = ${setdir};
            }
        }
        $wrap_filename = "${setdir}/${setname}.EXT${wrap_ext}._${fileindex}.ftksim_wrap_raw.dat.bz2";
        if( -e ${wrap_filename} ) {
            print $fh "W ${wrap_filename}\n";
        } else {
            print "warning: could not find wrapper file ${wrap_filename} for ${ftktracks_filename}\n";
        }
    }
    # FTK road file if requested
    if( $include_roads ) {
        # figure out extension number and test for file existence.
        my $road_filename = "${ftktracks_filename}";
        $road_filename =~ s/tracks_merge\/8/merged\/REGION/;
        $road_filename =~ s/ftktracks_/ftkroads\_/;
        for( my $iregion=0; $iregion!=8; ++$iregion ) {
            my ${road_region_filename} = ${road_filename};
            ${road_region_filename} =~ s/REGION/${iregion}/;
            if( -e ${road_region_filename} ) {
                print $fh "R ${road_region_filename} ${iregion}\n";
            } else {
                print "warning: could not find region ${iregion} road file ${road_region_filename} for ${ftktracks_filename}\n";
            }
        }
    }
    # CBNTs if requested
    if( $include_cbnt ) {
        # figure out extension number and test for file existence.
        my $cbnt_filename = "";
        if( $setdir ne ${cbnt_last_dset} || ${cbnt_ext}<0 || ${cbnt_ext}>=10 ) {
            for( ${cbnt_ext} = 0; ${cbnt_ext}<=10; ++${cbnt_ext} ) {
                $cbnt_filename = "${setdir}/${setname}.EXT${cbnt_ext}._${fileindex}.output.aant.root";
                if( -e ${cbnt_filename} ) { last; }
            }
            if( ${cbnt_ext}<0 || ${cbnt_ext}>=10 ) {
                print "could not figure out cbntper file extension for ${ftktracks_filename}\n";
            } else {
                ${cbnt_last_dset} = ${setdir};
            }
        }
        $cbnt_filename = "${setdir}/${setname}.EXT${cbnt_ext}._${fileindex}.output.aant.root";
        if( -e ${cbnt_filename} ) {
            print $fh "C14 ${cbnt_filename}\n";
        } else {
            print "warning: could not find cbnt file ${cbnt_filename} for ${ftktracks_filename}\n";
        }
    }
    # JetTagNtuples if requested
    if( $include_btag ) {
        # figure out extension number and test for file existence.
        my $btag_filename = "";
        if( $setdir ne ${btag_last_dset} || ${btag_ext}<0 || ${btag_ext}>=10 ) {
            for( ${btag_ext} = 0; ${btag_ext}<=10; ++${btag_ext} ) {
                $btag_filename = "${setdir}/${setname}.EXT${btag_ext}._${fileindex}.btag.pool.root";
                if( -e ${btag_filename} ) { last; }
            }
            if( ${btag_ext}<0 || ${btag_ext}>=10 ) {
                print "could not figure out btag file extension for ${ftktracks_filename}\n";
            } else {
                ${btag_last_dset} = ${setdir};
            }
        }
        $btag_filename = "${setdir}/${setname}.EXT${btag_ext}._${fileindex}.btag.pool.root";
        if( -e ${btag_filename} ) {
            print $fh "B ${btag_filename}\n";
        } else {
            print "warning: could not find btag file ${btag_filename} for ${ftktracks_filename}\n";
        }
    }
    print $fh "E\n";
#    last;
}

close $fh;

print "done after ${nevents} events.\n";


#!/usr/bin/perl -w

# parse ilumicalc html output and extract run numbers, luminosities, and other data.
# writes this information to the terminal.

# from bash, 
#   eval $(perl -I/share/ftkdata/boveia/perl/lib/perl5 -Mlocal::lib=/share/ftkdata/boveia/perl)
# to set up local (UC Tier3) perl modules such as PBS::Client.

use strict;
use warnings;
use FileHandle;
use HTML::TableExtract;
use File::Slurp;
use Scalar::Util qw(looks_like_number);  
use Getopt::Long qw(GetOptionsFromArray);

my $ilumicalchtml;
GetOptionsFromArray( \@ARGV , "i=s" => \$ilumicalchtml );

die "${ilumicalchtml} : input file does not exist.\n" unless ( -e "$ilumicalchtml" );

# load the entire file into a single string
my $htmldata = read_file("$ilumicalchtml");

# construct space-separated list of regular expressions which will match to the table's column header strings
my $tabform = HTML::TableExtract->new( headers => [qw(Run\s*Number Number\s*of\s*Good\s*LBs Number\s*of\s*Bad\s*LBs Luminosity\s*Delivered Luminosity\s*Livefraction\s*Corrected Luminosity\s*Prescale\s*Corrected Lumi-Weighted\s*Live\s*Fraction Lumi-Weighted\s*Prescale)] );
$tabform->parse( $htmldata );

# data11_7TeV.periodAllYear_HEAD_All_Good.xml
# period B     3 runs [178044, 178109] 
# period D    22 runs [179725, 180481] 
# period E     5 runs [180614, 180776] 
# period F    15 runs [182013, 182519] 
# period G    28 runs [182726, 183462] 
# period H    13 runs [183544, 184169] 
# period I    25 runs [185353, 186493] 
# period J     9 runs [186516, 186755] 
# period K    15 runs [186873, 187815] 


my %lumibyperiod = ();
my %goodrunsbyperiod = ();
my %lumibygoodrun = ();
my @periods = ( "2010" , 
                "2011B" , 
                "2011D1" , 
                "2011D2" , 
                "2011D3" , 
                "2011D4" , 
                "2011D5" , 
                "2011D6" , 
                "2011D7" , 
                "2011E1" , 
                "2011F1" , 
                "2011F2" , 
                "2011F3" , 
                "2011G1" ,
                "2011G2" ,
                "2011G3" ,
                "2011G4" ,
                "2011G5" ,
                "2011G6" ,
                "2011H1" ,
                "2011H2" ,
                "2011H3" ,
                "2011H4" ,
                "2011I1" ,
                "2011I2" ,
                "2011I3" ,
                "2011I4" ,
                "2011J1" ,
                "2011J2" ,
                "2011K1" ,
                "2011K2" ,
                "2011K3" ,
                "2011K4" ,
                "2011K5" ,
                "2011K6" ,
                "unknown" );
foreach my $per (@periods) { $lumibyperiod{ $per } = 0; }
my $totallumi = 0;


# parse each table and each row in each table.
foreach my $itable ($tabform->tables) {
    foreach my $srow ($itable->rows) {
        my @row = @$srow;
        if( scalar(@row)==0 ) { next; }
        my $rowtxt = join(",",@row);
        # print "$rowtxt\n";
        my $runstr = $row[0]; $runstr =~ s/\s.*//;
        if( !looks_like_number( ($runstr) ) ) { next; }
        my $runnumber = 0 + $runstr;
        my $goodlbn = 0 + $row[1];
        my $badlbn = 0 + $row[2];
        my $delivlumi = 1.*$row[3];
        my $livelumi = 1.*$row[4];
        my $prescalelumi = 1.*$row[5];
        my $liveeff = 1.*$row[6];
        my $averageprescale = 1.*$row[7];
        # print "$runnumber $goodlbn $badlbn $delivlumi $livelumi $prescalelumi $liveeff $averageprescale\n";
        my $period = which_period( $runnumber );
        $lumibyperiod{ $period } += $prescalelumi;
        if( ! exists $goodrunsbyperiod{ $period } ) {
            $goodrunsbyperiod{ $period } = [ $runnumber ];
        } else {
            push @{ $goodrunsbyperiod{ $period } } , $runnumber;
        }
        if( ! exists $lumibygoodrun{ $runnumber } ) {
            $lumibygoodrun{ $runnumber } = 0;
        } 
        $lumibygoodrun{ $runnumber } += $prescalelumi;
        $totallumi += $prescalelumi;
    }
}

# display results
print "Lumnosity by Period\n";
print "Period Lumi\n";
foreach my $per (@periods) { 
    my $lum = $lumibyperiod{$per};
    print "$per $lum\n";
}
print "Total $totallumi\n";

print "\nList of Good Runs\n";
print "Period RunNumber Lumi\n";
foreach my $per (@periods) { 
    if( ! exists $goodrunsbyperiod{$per} ) { next; }
    my @runs = @{ $goodrunsbyperiod{$per} };
    foreach my $run (@runs) {
        my $lum = 0 + $lumibygoodrun{ $run };
        print "$per $run $lum\n";
    }
}



sub which_period {
    # determine period by run number. For B through E1 and for G1 this
    # is based on a check of the runs present in the corresponding
    # physics container. for periods F1-F3 it is ad-hoc use of the run
    # numbers in a presentation by DQ Helen Hayward on 31 May 2011.
    my ($runnum) = @_;
    if( !looks_like_number( ($runnum) ) ) { return "unknown"; }
    if( $runnum < 178000 ) { return "2010"; }
    if( $runnum <= 178109 ) { return "2011B"; }
    if( $runnum <= 179739 ) { return "2011D1"; }
    if( $runnum <= 179804 ) { return "2011D2"; }
    if( $runnum <= 180144 ) { return "2011D3"; }
    if( $runnum <= 180212 ) { return "2011D4"; }
    if( $runnum <= 180242 ) { return "2011D5"; }
    if( $runnum <= 180448 ) { return "2011D6"; }
    if( $runnum <= 180481 ) { return "2011D7"; }
    if( $runnum <= 180710 ) { return "2011E1"; }
    if( $runnum <= 182160 ) { return "2011F1"; }
    if( $runnum <= 182486 ) { return "2011F2"; }
    if( $runnum <= 182519 ) { return "2011F3"; }
    if( $runnum == 182726 ) { return "2011G1"; }
    if( $runnum >= 182747 && $runnum <= 182886 ) { return "2011G2"; }
    if( $runnum >= 182997 && $runnum <= 183021 ) { return "2011G3"; }
    if( $runnum >= 183038 && $runnum <= 183130 ) { return "2011G4"; }
    if( $runnum >= 183216 && $runnum <= 183347 ) { return "2011G5"; }
    if( $runnum >= 183391 && $runnum <= 183462 ) { return "2011G6"; }
    if( $runnum >= 183544 && $runnum <= 183602 ) { return "2011H1"; }
    if( $runnum == 183780 ) { return "2011H2"; }
    if( $runnum >= 183963 && $runnum <= 184072 ) { return "2011H3"; }
    if( $runnum >= 184074 && $runnum <= 184169 ) { return "2011H4"; }
    if( $runnum >= 185353 && $runnum <= 185649 ) { return "2011I1"; }
    if( $runnum >= 185731 && $runnum <= 185976 ) { return "2011I2"; }
    if( $runnum >= 185998 && $runnum <= 186182 ) { return "2011I3"; }
    if( $runnum >= 186216 && $runnum <= 186493 ) { return "2011I4"; }
    if( $runnum >= 186516 && $runnum <= 186673 ) { return "2011J1"; }
    if( $runnum >= 186721 && $runnum <= 186755 ) { return "2011J2"; }
    if( $runnum >= 186873 && $runnum <= 186934 ) { return "2011K1"; }
    if( $runnum >= 186965 && $runnum <= 187219 ) { return "2011K2"; }
    if( $runnum >= 187453 && $runnum <= 187552 ) { return "2011K3"; }
    if( $runnum == 187763 ) { return "2011K4"; }
    if( $runnum >= 187811 && $runnum <= 187812  ) { return "2011K5"; }
    if( $runnum == 187815 ) { return "2011K6"; }
    return "unknown";
}


#!/usr/bin/perl -w

use strict;
use File::Slurp;
use File::Basename;

# retrieve list of samples.
my $dirnames = `ls -1d /share/ftkdata1/boveia/samples/v1_26/*r23??_r2300*/ /share/ftkdata1/boveia/samples/v1_26/*data11*/`;
# remove trailing dataset decimal tag and collapse to unique sample names
$dirnames =~ s/\.[0-9]+\//*\//g;
$dirnames =~ s/\.[0-9]+_r[0-9]+\//*\//g;
$dirnames =~ s/\_tid.+/*/g;
$dirnames =~ s/\n/ /g;
my @sampledirs = uniq( split(/ +/,$dirnames) );

#print "$sampledirs[0]\n";

# hash of luminosities for each data period
my $dataprop;
# 2011 luminosities corresponding to v28 WZjets GRL and OflLumi-7TeV-002 with ive fraction trigger L1_EM14
$dataprop->{ "wz2011B" } = { "lumi" => 11.38612491 };
$dataprop->{ "wz2011D1" } = { "lumi" => 9.5570489 };
$dataprop->{ "wz2011D2" } = { "lumi" => 8.7340545 };
$dataprop->{ "wz2011D3" } = { "lumi" => 29.73906914 };
$dataprop->{ "wz2011D4" } = { "lumi" => 31.9316473 };
$dataprop->{ "wz2011D5" } = { "lumi" => 27.4133804 };
$dataprop->{ "wz2011D6" } = { "lumi" => 25.5108394 };
$dataprop->{ "wz2011D7" } = { "lumi" => 21.3402185 };
$dataprop->{ "wz2011E1" } = { "lumi" => 42.2520343 };
$dataprop->{ "wz2011F1" } = { "lumi" => 0.33022749 };
$dataprop->{ "wz2011F2" } = { "lumi" => 110.06963 };
$dataprop->{ "wz2011F3" } = { "lumi" => 12.5952214 };
$dataprop->{ "wz2011G1" } = { "lumi" => 4.3504048 };
$dataprop->{ "wz2011G2" } = { "lumi" => 98.38403022 };
$dataprop->{ "wz2011G3" } = { "lumi" => 63.91007874 };
$dataprop->{ "wz2011G4" } = { "lumi" => 105.204624 };
$dataprop->{ "wz2011G5" } = { "lumi" => 92.2705864 };
$dataprop->{ "wz2011G6" } = { "lumi" => 100.1821518 };
$dataprop->{ "wz2011H1" } = { "lumi" => 48.4681055 };
$dataprop->{ "wz2011H2" } = { "lumi" => 43.2627417 };
$dataprop->{ "wz2011H3" } = { "lumi" => 53.4282478 };
$dataprop->{ "wz2011H4" } = { "lumi" => 95.0977534 };
$dataprop->{ "wz2011I1" } = { "lumi" => 14.73595955 };
$dataprop->{ "wz2011I2" } = { "lumi" => 29.9666137 };
$dataprop->{ "wz2011I3" } = { "lumi" => 132.1216605 };
$dataprop->{ "wz2011I4" } = { "lumi" => 128.01530682 };
$dataprop->{ "wz2011J1" } = { "lumi" => 66.3574909 };
$dataprop->{ "wz2011J2" } = { "lumi" => 145.8739784 };
$dataprop->{ "wz2011K1" } = { "lumi" => 177.4337195 };
$dataprop->{ "wz2011K2" } = { "lumi" => 187.6571642 };
$dataprop->{ "wz2011K3" } = { "lumi" => 31.315234 };
$dataprop->{ "wz2011K4" } = { "lumi" => 34.2927773 };
$dataprop->{ "wz2011K5" } = { "lumi" => 44.649689 };
$dataprop->{ "wz2011K6" } = { "lumi" => 24.6567974 };

# 2011 luminosities corresponding to v28 gamma+jet GRL and OflLumi-7TeV-002 with ive fraction trigger L1_EM14
$dataprop->{ "gj2011B" } = { "lumi" => 11.44569811 };
$dataprop->{ "gj2011D" } = { "lumi" => 154.98475644 };
$dataprop->{ "gj2011E" } = { "lumi" => 42.2520343 };
$dataprop->{ "gj2011F" } = { "lumi" => 126.3998366 };
$dataprop->{ "gj2011G" } = { "lumi" => 494.67958736 };
$dataprop->{ "gj2011H" } = { "lumi" => 242.876937 };
$dataprop->{ "gj2011I" } = { "lumi" => 309.808903831 };
$dataprop->{ "gj2011J" } = { "lumi" => 213.4580074 };
$dataprop->{ "gj2011K" } = { "lumi" => 517.3055996 };

# build hash of mc sample cross sections, filter efficiencies, and k factors for HSG3 samples
my $mcprop;
# cross sections from https://svnweb.cern.ch/trac/atlasgrp/browser/Physics/StandardModel/ElectroWeak/Analyses/Summer2011/Common/mc10b_p574_info.txt
#ttbar
$mcprop->{ "105200" } = { "name" => "ttbar" , "xsec" => 164.57 , "eff" => 0.5562 , "kf" => 1. };
#single top
$mcprop->{ "108340" } = { "name" => "tchan->e" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 7.1522 };
$mcprop->{ "108341" } = { "name" => "tchan->mu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 7.1767 };
$mcprop->{ "108342" } = { "name" => "tchan->tau" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 7.1277 };
$mcprop->{ "108343" } = { "name" => "schan->e" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.46856 };
$mcprop->{ "108344" } = { "name" => "schan->mu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.46837 };
$mcprop->{ "108345" } = { "name" => "schan->tau" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.46978 };
$mcprop->{ "108346" } = { "name" => "Wt" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 13.102 };
# ww
$mcprop->{ "105921" } = { "name" => "qq->WpWm->eenunu" , "xsec" => 0.524 , "eff" => 0.5562 , "kf" => 1.};
$mcprop->{ "105922" } = { "name" => "qq->WpWm->emununu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105923" } = { "name" => "qq->WpWm->etaununu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105924" } = { "name" => "qq->WpWm->mumununu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105925" } = { "name" => "qq->WpWm->muenunu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105926" } = { "name" => "qq->WpWm->mutaununu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105927" } = { "name" => "qq->WpWm->tautaununu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105928" } = { "name" => "qq->WpWm->tauenunu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105929" } = { "name" => "qq->WpWm->taumununu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "106011" } = { "name" => "gg->WpWm->eenunu" , "kf" => 1.0 , "eff" => 0.9885 , "xsec" => 0.0155};
$mcprop->{ "106012" } = { "name" => "gg->WpWm->emununu" , "kf" => 1.0 , "eff" => 0.9893 , "xsec" => 0.0155};
$mcprop->{ "106013" } = { "name" => "gg->WpWm->etaununu" , "kf" => 1.0 , "eff" => 0.9231 , "xsec" => 0.0155};
$mcprop->{ "106014" } = { "name" => "gg->WpWm->mumununu" , "kf" => 1.0 , "eff" => 0.9897 , "xsec" => 0.0155};
$mcprop->{ "106015" } = { "name" => "gg->WpWm->muenunu" , "kf" => 1.0 , "eff" => 0.9888 , "xsec" => 0.0155};
$mcprop->{ "106016" } = { "name" => "gg->WpWm->mutaununu" , "kf" => 1.0 , "eff" => 0.9281 , "xsec" => 0.0155};
$mcprop->{ "106017" } = { "name" => "gg->WpWm->tautaununu" , "kf" => 1.0 , "eff" => 0.3268 , "xsec" => 0.0155};
$mcprop->{ "106018" } = { "name" => "gg->WpWm->tauenunu" , "kf" => 1.0 , "eff" => 0.9236 , "xsec" => 0.0155};
$mcprop->{ "106019" } = { "name" => "gg->WpWm->taumununu" , "kf" => 1.0 , "eff" => 0.9272 , "xsec" => 0.0155};
$mcprop->{ "105985" } = { "name" => "WW" , "kf" => 1.52 , "eff" => 0.38863 , "xsec" => 29.592};
    # wz
$mcprop->{ "105971" } = { "name" => "ZW-->lnull" , "kf" => 1.13 , "eff" => 1.0 , "xsec" => 0.08605};
$mcprop->{ "106024" } = { "name" => "ZW+->taunull" , "kf" => 1.09 , "eff" => 1.0 , "xsec" => 0.07962};
$mcprop->{ "106027" } = { "name" => "ZW-->taunull" , "kf" => 1.13 , "eff" => 1.0 , "xsec" => 0.04302};
$mcprop->{ "105940" } = { "name" => "ZW+->lnuqq" , "kf" => 1.09 , "eff" => 1.0 , "xsec" => 1.6889 };
$mcprop->{ "105970" } = { "name" => "ZW-->lnuqq" , "kf" => 1.13 , "eff" => 1.0 , "xsec" => 0.91264};
$mcprop->{ "105942" } = { "name" => "ZW+->qqll" , "kf" => 1.09 , "eff" => 1.0 , "xsec" => 0.49836};
$mcprop->{ "105972" } = { "name" => "ZW-->qqll" , "kf" => 1.13 , "eff" => 1.0 , "xsec" => 0.2693 };
$mcprop->{ "106025" } = { "name" => "ZW+->lnutautau" , "kf" => 1.09 , "eff" => 1.0 , "xsec" => 0.07962};
$mcprop->{ "106028" } = { "name" => "ZW-->lnutautau" , "kf" => 1.13 , "eff" => 1.0 , "xsec" => 0.04302};
$mcprop->{ "106026" } = { "name" => "ZW+->taunutautau" , "kf" => 1.09 , "eff" => 1.0 , "xsec" => 0.03981};
$mcprop->{ "106029" } = { "name" => "ZW-->taunutautau" , "kf" => 1.13 , "eff" => 1.0 , "xsec" => 0.02151};
$mcprop->{ "113190" } = { "name" => "ZW+->qqtautau" , "kf" => 1.09 , "eff" => 1.0 , "xsec" => 0.24918};
$mcprop->{ "113191" } = { "name" => "ZW-->qqtautau" , "kf" => 1.13 , "eff" => 1.0 , "xsec" => 0.13465};
    # zz
$mcprop->{ "105941" } = { "name" => "ZW+->lnull" , "kf" => 1.09 , "eff" => 1.0 , "xsec" => 0.15924};
$mcprop->{ "105986" } = { "name" => "ZZ" , "kf" => 1.29 , "eff" => 0.21152 , "xsec" => 4.62};
$mcprop->{ "105931" } = { "name" => "ZZ_llll" , "kf" => 1.11 , "eff" => 1.0 , "xsec" => 0.02436};
$mcprop->{ "109292" } = { "name" => "zz4l_3MLFElecMu" , "kf" => 1.5 , "eff" => 0.6235 , "xsec" => 0.07494};
$mcprop->{ "107650" } = { "name" => "ZeeNp0_pt20" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 664.1};
$mcprop->{ "107651" } = { "name" => "ZeeNp1_pt20" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 132.462};
$mcprop->{ "107652" } = { "name" => "ZeeNp2_pt20" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 40.226};
$mcprop->{ "107653" } = { "name" => "ZeeNp3_pt20" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 11.138};
$mcprop->{ "107654" } = { "name" => "ZeeNp4_pt20" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 3.12818};
$mcprop->{ "107655" } = { "name" => "ZeeNp5_pt20" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 0.753425};
$mcprop->{ "107660" } = { "name" => "ZmumuNp0_pt20" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 663.79};
$mcprop->{ "107661" } = { "name" => "ZmumuNp1_pt20" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 132.95};
$mcprop->{ "107662" } = { "name" => "ZmumuNp2_pt20" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 40.375};
$mcprop->{ "107663" } = { "name" => "ZmumuNp3_pt20" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 11.161};
$mcprop->{ "107664" } = { "name" => "ZmumuNp4_pt20" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 2.8987};
$mcprop->{ "107665" } = { "name" => "ZmumuNp5_pt20" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 0.75662};
$mcprop->{ "109380" } = { "name" => "ZeebbNp0_3Leptons" , "kf" => 1.26 , "eff" => 0.09 , "xsec" => 6.37};
$mcprop->{ "109381" } = { "name" => "ZeebbNp1_3Leptons" , "kf" => 1.26 , "eff" => 0.14 , "xsec" => 2.39};
$mcprop->{ "109382" } = { "name" => "ZeebbNp2_3Leptons" , "kf" => 1.26 , "eff" => 0.16 , "xsec" => 0.859};
$mcprop->{ "109383" } = { "name" => "ZeebbNp3_3Leptons" , "kf" => 1.26 , "eff" => 0.19 , "xsec" => 0.379};
$mcprop->{ "109385" } = { "name" => "ZmumubbNp0_3Leptons" , "kf" => 1.26 , "eff" => 0.09 , "xsec" => 6.34};
$mcprop->{ "109386" } = { "name" => "ZmumubbNp1_3Leptons" , "kf" => 1.26 , "eff" => 0.14 , "xsec" => 2.45};
$mcprop->{ "109387" } = { "name" => "ZmumubbNp2_3Leptons" , "kf" => 1.26 , "eff" => 0.16 , "xsec" => 0.858};
$mcprop->{ "109388" } = { "name" => "ZmumubbNp3_3Leptons" , "kf" => 1.26 , "eff" => 0.2 , "xsec" => 0.376};

#Z+jets
$mcprop->{ "107650" } = { "name" => "ZeeNp0" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 664.10      };
$mcprop->{ "107651" } = { "name" => "ZeeNp1" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 132.462     };
$mcprop->{ "107652" } = { "name" => "ZeeNp2" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 40.226      };
$mcprop->{ "107653" } = { "name" => "ZeeNp3" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 11.138      };
$mcprop->{ "107654" } = { "name" => "ZeeNp4" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 3.12818     };
$mcprop->{ "107655" } = { "name" => "ZeeNp5" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 0.753425     };
$mcprop->{ "107660" } = { "name" => "ZmumuNp0" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 663.79      };
$mcprop->{ "107661" } = { "name" => "ZmumuNp1" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 132.95      };
$mcprop->{ "107662" } = { "name" => "ZmumuNp2" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 40.375      };
$mcprop->{ "107663" } = { "name" => "ZmumuNp3" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 11.161      };
$mcprop->{ "107664" } = { "name" => "ZmumuNp4" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 2.8987      };
$mcprop->{ "107665" } = { "name" => "ZmumuNp5" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 0.75662      };
$mcprop->{ "116250" } = { "name" => "ZeeNp0_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 3051.62        };
$mcprop->{ "116251" } = { "name" => "ZeeNp1_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 87.87          };
$mcprop->{ "116252" } = { "name" => "ZeeNp2_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 41.10          };
$mcprop->{ "116253" } = { "name" => "ZeeNp3_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 8.46           };
$mcprop->{ "116254" } = { "name" => "ZeeNp4_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 1.84           };
$mcprop->{ "116255" } = { "name" => "ZeeNp5_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 0.46           };
$mcprop->{ "116260" } = { "name" => "ZmmNp0_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 3051.62        };
$mcprop->{ "116261" } = { "name" => "ZmmNp1_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 87.87          };
$mcprop->{ "116262" } = { "name" => "ZmmNp2_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 40.95          };
$mcprop->{ "116263" } = { "name" => "ZmmNp3_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 8.41           };
$mcprop->{ "116264" } = { "name" => "ZmmNp4_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 1.85           };
$mcprop->{ "116265" } = { "name" => "ZmmNp5_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 0.46           };
$mcprop->{ "106052" } = { "name" => "Ztautau" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 856.967     };
$mcprop->{ "107055" } = { "name" => "DYLowMtautau_M10" , "kf" => 1.15 , "eff" => 1.0 , "xsec" => 3454.0      };
$mcprop->{ "107055" } = { "name" => "DYLowMtautau_M10" , "kf" => 1.15 , "eff" => 1.0 , "xsec" => 3454.0      };
#Z+bb (cross-sections and k-factors from top group twiki)
#https://twiki.cern.ch/twiki/bin/view/AtlasProtected/TopMC10For2011Data#Alpgen_Z_jets_Unfiltered_Sample
$mcprop->{ "109300" } = { "name" => "ZeebbNp0_nofilter" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 6.57};
$mcprop->{ "109301" } = { "name" => "ZeebbNp1_nofilter" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 2.48};
$mcprop->{ "109302" } = { "name" => "ZeebbNp2_nofilter" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 0.89};
$mcprop->{ "109303" } = { "name" => "ZeebbNp3_nofilter" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 0.39};
$mcprop->{ "109305" } = { "name" => "ZmmbbNp0_nofilter" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 6.56};
$mcprop->{ "109306" } = { "name" => "ZmmbbNp1_nofilter" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 2.47};
$mcprop->{ "109307" } = { "name" => "ZmmbbNp2_nofilter" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 0.89};
$mcprop->{ "109308" } = { "name" => "ZmmbbNp3_nofilter" , "kf" => 1.26 , "eff" => 1.0 , "xsec" => 0.39};
#W+jets
#<e600 for Np0,1; and e760 for Np>=2>
$mcprop->{ "107680" } = { "name" => "WenuNp0" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 6870.5      };
$mcprop->{ "107680" } = { "name" => "WenuNp0" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 6870.5      };
$mcprop->{ "107681" } = { "name" => "WenuNp1" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 1293.0      };
$mcprop->{ "107681" } = { "name" => "WenuNp1" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 1293.0      };
$mcprop->{ "107682" } = { "name" => "WenuNp2" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 376.60      };
$mcprop->{ "107683" } = { "name" => "WenuNp3" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 101.29      };
$mcprop->{ "107684" } = { "name" => "WenuNp4" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 25.246      };
$mcprop->{ "107685" } = { "name" => "WenuNp5" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 7.1239      };
$mcprop->{ "107690" } = { "name" => "WmunuNp0" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 6871.1     };
$mcprop->{ "107690" } = { "name" => "WmunuNp0" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 6871.1     };
$mcprop->{ "107691" } = { "name" => "WmunuNp1" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 1294.7     };
$mcprop->{ "107691" } = { "name" => "WmunuNp1" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 1294.7     };
$mcprop->{ "107692" } = { "name" => "WmunuNp2" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 376.08     };
$mcprop->{ "107693" } = { "name" => "WmunuNp3" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 100.72     };
$mcprop->{ "107694" } = { "name" => "WmunuNp4" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 25.993     };
$mcprop->{ "107695" } = { "name" => "WmunuNp5" , "kf" => 1.21 , "eff" => 1.0 , "xsec" => 7.1300     };
$mcprop->{ "106022" } = { "name" => "Wtaunu_1Lepton" , "kf" => 1.17 , "eff" => 0.8765778 , "xsec" => 8916.33*0.35 };
    #W/Z+Gamma
$mcprop->{ "106001" } = { "name" => "gammaWp(e)" , "kf" => 1.75 , "eff" => 1.0 , "xsec" => 28.0           };
$mcprop->{ "106002" } = { "name" => "gammaWp(mu)" , "kf" => 1.75 , "eff" => 1.0 , "xsec" => 28.0           };
$mcprop->{ "106003" } = { "name" => "gammaWp(tau)" , "kf" => 1.75 , "eff" => 1.0 , "xsec" => 25.4           };
$mcprop->{ "108288" } = { "name" => "gammaWm(e)" , "kf" => 1.83 , "eff" => 1.0 , "xsec" => 18.53          };
$mcprop->{ "108289" } = { "name" => "gammaWm(mu)" , "kf" => 1.83 , "eff" => 1.0 , "xsec" => 18.62          };
$mcprop->{ "108290" } = { "name" => "gammaWm(tau)" , "kf" => 1.83 , "eff" => 1.0 , "xsec" => 16.8           };
$mcprop->{ "108323" } = { "name" => "gammaZ(ee)" , "kf" => 1.41 , "eff" => 1.0 , "xsec" => 9.63           };
$mcprop->{ "108324" } = { "name" => "gammaZ(mumu)" , "kf" => 1.41 , "eff" => 1.0 , "xsec" => 9.63           };
$mcprop->{ "108325" } = { "name" => "gammaZ(tautau)" , "kf" => 1.41 , "eff" => 0.15 , "xsec" => 9.41           };
    # Dijets
$mcprop->{ "105757" } = { "name" => "bbcc_mu10mu10X" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 2830.3      };
$mcprop->{ "105758" } = { "name" => "bbcc_mu10e10X" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 4017.1      };
$mcprop->{ "105759" } = { "name" => "bbcc_e10e10X" , "kf" => 1.0 , "eff" => 3.1913E-03 , "xsec" => 530580.      };

# xsecs and kfactors from https://svnweb.cern.ch/trac/atlasgrp/browser/Physics/StandardModel/ElectroWeak/Analyses/Summer2011/Common/mc10b_p591_info.txt
# overriding some of the above values
#ttbar
$mcprop->{ "105200" } = { "name" => "ttbar" , "xsec" => 164.57 , "eff" => 0.55551 , "kf" => 1. };
#single top
$mcprop->{ "108340" } = { "name" => "tchan->e" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 6.93 };
$mcprop->{ "108341" } = { "name" => "tchan->mu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 6.93 };
$mcprop->{ "108342" } = { "name" => "tchan->tau" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 6.93 };
$mcprop->{ "108343" } = { "name" => "schan->e" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.5 };
$mcprop->{ "108344" } = { "name" => "schan->mu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.5 };
$mcprop->{ "108345" } = { "name" => "schan->tau" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.5 };
$mcprop->{ "108346" } = { "name" => "Wt" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 15.6 };
# ww
$mcprop->{ "105921" } = { "name" => "qq->WpWm->eenunu" , "xsec" => 0.524 , "eff" => 1.0 , "kf" => 1.};
$mcprop->{ "105922" } = { "name" => "qq->WpWm->emununu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105923" } = { "name" => "qq->WpWm->etaununu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105924" } = { "name" => "qq->WpWm->mumununu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105925" } = { "name" => "qq->WpWm->muenunu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105926" } = { "name" => "qq->WpWm->mutaununu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105927" } = { "name" => "qq->WpWm->tautaununu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105928" } = { "name" => "qq->WpWm->tauenunu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "105929" } = { "name" => "qq->WpWm->taumununu" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.524};
$mcprop->{ "106011" } = { "name" => "gg->WpWm->eenunu" , "kf" => 1.0 , "eff" => 0.98951 , "xsec" => 0.0155};
$mcprop->{ "106012" } = { "name" => "gg->WpWm->emununu" , "kf" => 1.0 , "eff" => 0.98990 , "xsec" => 0.0155};
$mcprop->{ "106013" } = { "name" => "gg->WpWm->etaununu" , "kf" => 1.0 , "eff" => 0.92319 , "xsec" => 0.0155};
$mcprop->{ "106014" } = { "name" => "gg->WpWm->mumununu" , "kf" => 1.0 , "eff" => 0.98902 , "xsec" => 0.0155};
$mcprop->{ "106015" } = { "name" => "gg->WpWm->muenunu" , "kf" => 1.0 , "eff" => 0.98687 , "xsec" => 0.0155};
$mcprop->{ "106016" } = { "name" => "gg->WpWm->mutaununu" , "kf" => 1.0 , "eff" => 0.92876 , "xsec" => 0.0155};
$mcprop->{ "106017" } = { "name" => "gg->WpWm->tautaununu" , "kf" => 1.0 , "eff" => 0.32688 , "xsec" => 0.0155};
$mcprop->{ "106018" } = { "name" => "gg->WpWm->tauenunu" , "kf" => 1.0 , "eff" => 0.92191 , "xsec" => 0.0155};
$mcprop->{ "106019" } = { "name" => "gg->WpWm->taumununu" , "kf" => 1.0 , "eff" => 0.92894 , "xsec" => 0.0155};
$mcprop->{ "105985" } = { "name" => "WW" , "kf" => 1.52 , "eff" => 0.38863 , "xsec" => 29.592};
    # wz
$mcprop->{ "105971" } = { "name" => "ZW-->lnull" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.08605};
$mcprop->{ "106024" } = { "name" => "ZW+->taunull" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.07962};
$mcprop->{ "106027" } = { "name" => "ZW-->taunull" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.04302};
$mcprop->{ "105940" } = { "name" => "ZW+->lnuqq" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 1.6889 };
$mcprop->{ "105970" } = { "name" => "ZW-->lnuqq" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.91264};
$mcprop->{ "105942" } = { "name" => "ZW+->qqll" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.49836};
$mcprop->{ "105972" } = { "name" => "ZW-->qqll" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.2693 };
$mcprop->{ "106025" } = { "name" => "ZW+->lnutautau" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.07962};
$mcprop->{ "106028" } = { "name" => "ZW-->lnutautau" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.04302};
$mcprop->{ "106026" } = { "name" => "ZW+->taunutautau" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.03981};
$mcprop->{ "106029" } = { "name" => "ZW-->taunutautau" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.02151};
$mcprop->{ "113190" } = { "name" => "ZW+->qqtautau" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.24918};
$mcprop->{ "113191" } = { "name" => "ZW-->qqtautau" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.13465};
    # zz
$mcprop->{ "105941" } = { "name" => "ZW+->lnull" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 0.15924};
$mcprop->{ "105986" } = { "name" => "ZZ" , "kf" => 1.2 , "eff" => 1.0 , "xsec" => 0.977 };
$mcprop->{ "107650" } = { "name" => "ZeeNp0_pt20" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 668.32};
$mcprop->{ "107651" } = { "name" => "ZeeNp1_pt20" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 134.36};
$mcprop->{ "107652" } = { "name" => "ZeeNp2_pt20" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 40.54};
$mcprop->{ "107653" } = { "name" => "ZeeNp3_pt20" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 11.16};
$mcprop->{ "107654" } = { "name" => "ZeeNp4_pt20" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 2.88};
$mcprop->{ "107655" } = { "name" => "ZeeNp5_pt20" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 0.83};
$mcprop->{ "107660" } = { "name" => "ZmumuNp0_pt20" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 668.68};
$mcprop->{ "107661" } = { "name" => "ZmumuNp1_pt20" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 134.14};
$mcprop->{ "107662" } = { "name" => "ZmumuNp2_pt20" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 40.33};
$mcprop->{ "107663" } = { "name" => "ZmumuNp3_pt20" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 11.19};
$mcprop->{ "107664" } = { "name" => "ZmumuNp4_pt20" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 2.75};
$mcprop->{ "107665" } = { "name" => "ZmumuNp5_pt20" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 0.77};
#Z+jets
$mcprop->{ "107650" } = { "name" => "ZeeNp0" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 668.32     };
$mcprop->{ "107651" } = { "name" => "ZeeNp1" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 134.36     };
$mcprop->{ "107652" } = { "name" => "ZeeNp2" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 40.54      };
$mcprop->{ "107653" } = { "name" => "ZeeNp3" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 11.16      };
$mcprop->{ "107654" } = { "name" => "ZeeNp4" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 2.88     };
$mcprop->{ "107655" } = { "name" => "ZeeNp5" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 0.83     };
$mcprop->{ "107660" } = { "name" => "ZmumuNp0" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 668.68      };
$mcprop->{ "107661" } = { "name" => "ZmumuNp1" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 134.14      };
$mcprop->{ "107662" } = { "name" => "ZmumuNp2" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 40.33      };
$mcprop->{ "107663" } = { "name" => "ZmumuNp3" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 11.19      };
$mcprop->{ "107664" } = { "name" => "ZmumuNp4" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 2.75      };
$mcprop->{ "107665" } = { "name" => "ZmumuNp5" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 0.77      };
$mcprop->{ "116250" } = { "name" => "ZeeNp0_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 3055.20        };
$mcprop->{ "116251" } = { "name" => "ZeeNp1_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 84.92          };
$mcprop->{ "116252" } = { "name" => "ZeeNp2_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 41.40          };
$mcprop->{ "116253" } = { "name" => "ZeeNp3_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 8.38           };
$mcprop->{ "116254" } = { "name" => "ZeeNp4_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 1.85           };
$mcprop->{ "116255" } = { "name" => "ZeeNp5_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 0.46           };
$mcprop->{ "116260" } = { "name" => "ZmmNp0_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 3054.90        };
$mcprop->{ "116261" } = { "name" => "ZmmNp1_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 84.87          };
$mcprop->{ "116262" } = { "name" => "ZmmNp2_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 41.45          };
$mcprop->{ "116263" } = { "name" => "ZmmNp3_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 8.38           };
$mcprop->{ "116264" } = { "name" => "ZmmNp4_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 1.85           };
$mcprop->{ "116265" } = { "name" => "ZmmNp5_M10to40_pt20" , "kf" => 1.22 , "eff" => 1.0 , "xsec" => 0.46           };
$mcprop->{ "106052" } = { "name" => "Ztautau" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 989     };
$mcprop->{ "107055" } = { "name" => "DYLowMtautau_M10" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 3971      };
$mcprop->{ "107055" } = { "name" => "DYLowMtautau_M10" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 3971      };
#Z+bb 
$mcprop->{ "109300" } = { "name" => "ZeebbNp0_nofilter" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 6.57};
$mcprop->{ "109301" } = { "name" => "ZeebbNp1_nofilter" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 2.48};
$mcprop->{ "109302" } = { "name" => "ZeebbNp2_nofilter" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 0.89};
$mcprop->{ "109303" } = { "name" => "ZeebbNp3_nofilter" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 0.39};
$mcprop->{ "109305" } = { "name" => "ZmmbbNp0_nofilter" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 6.56};
$mcprop->{ "109306" } = { "name" => "ZmmbbNp1_nofilter" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 2.47};
$mcprop->{ "109307" } = { "name" => "ZmmbbNp2_nofilter" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 0.89};
$mcprop->{ "109308" } = { "name" => "ZmmbbNp3_nofilter" , "kf" => 1.25 , "eff" => 1.0 , "xsec" => 0.39};
#W+jets
#<e600 for Np0,1; and e760 for Np>=2>
$mcprop->{ "107680" } = { "name" => "WenuNp0" , "kf" => 1.2 , "eff" => 1.0 , "xsec" => 6921.60      };
$mcprop->{ "107681" } = { "name" => "WenuNp1" , "kf" => 1.2 , "eff" => 1.0 , "xsec" => 1304.30      };
$mcprop->{ "107682" } = { "name" => "WenuNp2" , "kf" => 1.2 , "eff" => 1.0 , "xsec" => 378.29      };
$mcprop->{ "107683" } = { "name" => "WenuNp3" , "kf" => 1.2 , "eff" => 1.0 , "xsec" => 101.43      };
$mcprop->{ "107684" } = { "name" => "WenuNp4" , "kf" => 1.2 , "eff" => 1.0 , "xsec" => 25.87      };
$mcprop->{ "107685" } = { "name" => "WenuNp5" , "kf" => 1.2 , "eff" => 1.0 , "xsec" => 7.0      };
$mcprop->{ "107690" } = { "name" => "WmunuNp0" , "kf" => 1.2 , "eff" => 1.0 , "xsec" => 6919.60     };
$mcprop->{ "107691" } = { "name" => "WmunuNp1" , "kf" => 1.2 , "eff" => 1.0 , "xsec" => 1304.20     };
$mcprop->{ "107692" } = { "name" => "WmunuNp2" , "kf" => 1.2 , "eff" => 1.0 , "xsec" => 377.83     };
$mcprop->{ "107693" } = { "name" => "WmunuNp3" , "kf" => 1.2 , "eff" => 1.0 , "xsec" => 101.88     };
$mcprop->{ "107694" } = { "name" => "WmunuNp4" , "kf" => 1.2 , "eff" => 1.0 , "xsec" => 25.75     };
$mcprop->{ "107695" } = { "name" => "WmunuNp5" , "kf" => 1.2 , "eff" => 1.0 , "xsec" => 6.92     };
$mcprop->{ "106022" } = { "name" => "Wtaunu_1Lepton" , "kf" => 1.17 , "eff" => 0.3068 , "xsec" => 8916.33 };
    #W/Z+Gamma
$mcprop->{ "106001" } = { "name" => "gammaWp(e)" , "kf" => 1.75 , "eff" => 1.0 , "xsec" => 28.0           };
$mcprop->{ "106002" } = { "name" => "gammaWp(mu)" , "kf" => 1.75 , "eff" => 1.0 , "xsec" => 28.0           };
$mcprop->{ "106003" } = { "name" => "gammaWp(tau)" , "kf" => 1.75 , "eff" => 1.0 , "xsec" => 25.4           };
$mcprop->{ "108288" } = { "name" => "gammaWm(e)" , "kf" => 1.83 , "eff" => 1.0 , "xsec" => 18.53          };
$mcprop->{ "108289" } = { "name" => "gammaWm(mu)" , "kf" => 1.83 , "eff" => 1.0 , "xsec" => 18.62          };
$mcprop->{ "108290" } = { "name" => "gammaWm(tau)" , "kf" => 1.83 , "eff" => 1.0 , "xsec" => 16.8           };
$mcprop->{ "108323" } = { "name" => "gammaZ(ee)" , "kf" => 1.41 , "eff" => 1.0 , "xsec" => 9.63           };
$mcprop->{ "108324" } = { "name" => "gammaZ(mumu)" , "kf" => 1.41 , "eff" => 1.0 , "xsec" => 9.63           };
$mcprop->{ "108325" } = { "name" => "gammaZ(tautau)" , "kf" => 1.41 , "eff" => 0.15 , "xsec" => 9.41           };
    # Dijets
$mcprop->{ "105757" } = { "name" => "bbcc_mu10mu10X" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 2830      };
$mcprop->{ "105758" } = { "name" => "bbcc_mu10e10X" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 4017      };
$mcprop->{ "105759" } = { "name" => "bbcc_e10e10X" , "kf" => 1.0 , "eff" => 1.0 , "xsec" => 1693      };

# MC@NLO higgs
$mcprop->{ "109070" } = { "name" => "H->WW->ll_110" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109962" } = { "name" => "H->WW->ltau_110" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109963" } = { "name" => "H->WW->tautau_110" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116630" } = { "name" => "H->WW->ll_115" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116631" } = { "name" => "H->WW->ltau_115" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116632" } = { "name" => "H->WW->tautau_115" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109071" } = { "name" => "H->WW->ll_120" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109964" } = { "name" => "H->WW->ltau_120" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109965" } = { "name" => "H->WW->tautau_120" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116633" } = { "name" => "H->WW->ll_125" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116634" } = { "name" => "H->WW->ltau_125" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116635" } = { "name" => "H->WW->tautau_125" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "106534" } = { "name" => "H->WW->ll_130" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109966" } = { "name" => "H->WW->ltau_130" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109967" } = { "name" => "H->WW->tautau_130" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116636" } = { "name" => "H->WW->ll_135" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116637" } = { "name" => "H->WW->ltau_135" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116638" } = { "name" => "H->WW->tautau_135" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109072" } = { "name" => "H->WW->ll_140" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109968" } = { "name" => "H->WW->ltau_140" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109969" } = { "name" => "H->WW->tautau_140" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116639" } = { "name" => "H->WW->ll_145" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116640" } = { "name" => "H->WW->ltau_145" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116641" } = { "name" => "H->WW->tautau_145" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109073" } = { "name" => "H->WW->ll_150" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109970" } = { "name" => "H->WW->ltau_150" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109971" } = { "name" => "H->WW->tautau_150" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116642" } = { "name" => "H->WW->ll_155" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116643" } = { "name" => "H->WW->ltau_155" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116644" } = { "name" => "H->WW->tautau_155" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109074" } = { "name" => "H->WW->ll_160" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109960" } = { "name" => "H->WW->ltau_160" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109961" } = { "name" => "H->WW->tautau_160" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109136" } = { "name" => "H->WW->ll_165" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109972" } = { "name" => "H->WW->ltau_165" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109973" } = { "name" => "H->WW->tautau_165" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "106533" } = { "name" => "H->WW->ll_170" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109974" } = { "name" => "H->WW->ltau_170" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109975" } = { "name" => "H->WW->tautau_170" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116645" } = { "name" => "H->WW->ll_175" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116646" } = { "name" => "H->WW->ltau_175" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116647" } = { "name" => "H->WW->tautau_175" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109075" } = { "name" => "H->WW->ll_180" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109976" } = { "name" => "H->WW->ltau_180" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109977" } = { "name" => "H->WW->tautau_180" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116648" } = { "name" => "H->WW->ll_185" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116649" } = { "name" => "H->WW->ltau_185" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116650" } = { "name" => "H->WW->tautau_185" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109076" } = { "name" => "H->WW->ll_190" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109978" } = { "name" => "H->WW->ltau_190" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109979" } = { "name" => "H->WW->tautau_190" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116651" } = { "name" => "H->WW->ll_195" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116652" } = { "name" => "H->WW->ltau_195" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116653" } = { "name" => "H->WW->tautau_195" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "106535" } = { "name" => "H->WW->ll_200" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109980" } = { "name" => "H->WW->ltau_200" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109981" } = { "name" => "H->WW->tautau_200" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109077" } = { "name" => "H->WW->ll_220" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109982" } = { "name" => "H->WW->ltau_220" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109983" } = { "name" => "H->WW->tautau_220" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109078" } = { "name" => "H->WW->ll_240" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109984" } = { "name" => "H->WW->ltau_240" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109985" } = { "name" => "H->WW->tautau_240" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109079" } = { "name" => "H->WW->ll_260" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109986" } = { "name" => "H->WW->ltau_260" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109987" } = { "name" => "H->WW->tautau_260" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109080" } = { "name" => "H->WW->ll_280" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109988" } = { "name" => "H->WW->ltau_280" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109989" } = { "name" => "H->WW->tautau_280" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109081" } = { "name" => "H->WW->ll_300" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109990" } = { "name" => "H->WW->ltau_300" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "109991" } = { "name" => "H->WW->tautau_300" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116519" } = { "name" => "H->WW->ll_320" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116520" } = { "name" => "H->WW->ltau_320" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116521" } = { "name" => "H->WW->tautau_320" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116522" } = { "name" => "H->WW->ll_340" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116523" } = { "name" => "H->WW->ltau_340" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116524" } = { "name" => "H->WW->tautau_340" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116525" } = { "name" => "H->WW->ll_360" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116526" } = { "name" => "H->WW->ltau_360" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116527" } = { "name" => "H->WW->tautau_360" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116528" } = { "name" => "H->WW->ll_380" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116529" } = { "name" => "H->WW->ltau_380" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116530" } = { "name" => "H->WW->tautau_380" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116510" } = { "name" => "H->WW->ll_400" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116511" } = { "name" => "H->WW->ltau_400" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116512" } = { "name" => "H->WW->tautau_400" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116531" } = { "name" => "H->WW->ll_420" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116532" } = { "name" => "H->WW->ltau_420" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116533" } = { "name" => "H->WW->tautau_420" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116534" } = { "name" => "H->WW->ll_440" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116535" } = { "name" => "H->WW->ltau_440" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116536" } = { "name" => "H->WW->tautau_440" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116537" } = { "name" => "H->WW->ll_460" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116538" } = { "name" => "H->WW->ltau_460" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116539" } = { "name" => "H->WW->tautau_460" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116540" } = { "name" => "H->WW->ll_480" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116541" } = { "name" => "H->WW->ltau_480" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116542" } = { "name" => "H->WW->tautau_480" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116513" } = { "name" => "H->WW->ll_500" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116514" } = { "name" => "H->WW->ltau_500" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116515" } = { "name" => "H->WW->tautau_500" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116543" } = { "name" => "H->WW->ll_520" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116544" } = { "name" => "H->WW->ltau_520" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116545" } = { "name" => "H->WW->tautau_520" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116546" } = { "name" => "H->WW->ll_540" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116547" } = { "name" => "H->WW->ltau_540" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116548" } = { "name" => "H->WW->tautau_540" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116549" } = { "name" => "H->WW->ll_560" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116550" } = { "name" => "H->WW->ltau_560" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116551" } = { "name" => "H->WW->tautau_560" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116552" } = { "name" => "H->WW->ll_580" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116553" } = { "name" => "H->WW->ltau_580" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116554" } = { "name" => "H->WW->tautau_580" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116516" } = { "name" => "H->WW->ll_600" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116517" } = { "name" => "H->WW->ltau_600" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116518" } = { "name" => "H->WW->tautau_600" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
# gg H
$mcprop->{ "116700" } = { "name" => "ggH->WW->leplep_110" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116701" } = { "name" => "ggH->WW->leplep_115" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116702" } = { "name" => "ggH->WW->leplep_120" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116703" } = { "name" => "ggH->WW->leplep_125" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116613" } = { "name" => "ggH->WW->leplep_130" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116704" } = { "name" => "ggH->WW->leplep_135" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116705" } = { "name" => "ggH->WW->leplep_140" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116706" } = { "name" => "ggH->WW->leplep_145" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116707" } = { "name" => "ggH->WW->leplep_150" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116708" } = { "name" => "ggH->WW->leplep_155" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116614" } = { "name" => "ggH->WW->leplep_160" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116709" } = { "name" => "ggH->WW->leplep_165" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116710" } = { "name" => "ggH->WW->leplep_170" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116711" } = { "name" => "ggH->WW->leplep_175" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116712" } = { "name" => "ggH->WW->leplep_180" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116713" } = { "name" => "ggH->WW->leplep_185" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116714" } = { "name" => "ggH->WW->leplep_190" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116715" } = { "name" => "ggH->WW->leplep_195" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116716" } = { "name" => "ggH->WW->leplep_200" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116717" } = { "name" => "ggH->WW->leplep_205" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116718" } = { "name" => "ggH->WW->leplep_210" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116719" } = { "name" => "ggH->WW->leplep_220" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116720" } = { "name" => "ggH->WW->leplep_240" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116721" } = { "name" => "ggH->WW->leplep_260" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116722" } = { "name" => "ggH->WW->leplep_280" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116723" } = { "name" => "ggH->WW->leplep_300" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116724" } = { "name" => "ggH->WW->leplep_320" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116725" } = { "name" => "ggH->WW->leplep_340" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116726" } = { "name" => "ggH->WW->leplep_360" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116727" } = { "name" => "ggH->WW->leplep_380" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116619" } = { "name" => "ggH->WW->leplep_400" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116728" } = { "name" => "ggH->WW->leplep_420" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116729" } = { "name" => "ggH->WW->leplep_440" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116730" } = { "name" => "ggH->WW->leplep_460" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116731" } = { "name" => "ggH->WW->leplep_480" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116732" } = { "name" => "ggH->WW->leplep_500" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116733" } = { "name" => "ggH->WW->leplep_520" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116734" } = { "name" => "ggH->WW->leplep_540" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116735" } = { "name" => "ggH->WW->leplep_560" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116736" } = { "name" => "ggH->WW->leplep_580" , "kf" => 1. , "eff" => 1. , "xsec" => 0.};
$mcprop->{ "116737" } = { "name" => "ggH->WW->leplep_600" ,  "kf" => 1. , "eff" => 1. , "xsec" => 0.};
# cross section numbers from peter
$mcprop->{ "105930" } = { "name" => "ZZ_llqq" , "kf" => 1. , "eff" => 1. , "xsec" => 0. };
$mcprop->{ "105932" } = { "name" => "ZZ_llnunu" , "kf" => 1. , "eff" => 1. , "xsec" => 0. };
$mcprop->{ "106044" } = { "name" => "Wmunu_no_filter" , "kf" => 1. , "eff" => 1. , "xsec" => 10461. }; # disagrees with HSG3 numbers, overridden below
$mcprop->{ "106047" } = { "name" => "Zmumu_no_filter" , "kf" => 1. , "eff" => 1. , "xsec" => 989.0 }; # disagrees with HSG3 numbers, overridden below
$mcprop->{ "106059" } = { "name" => "B_ccmu15X" , "kf" => 1. , "eff" => 1. , "xsec" => 73000. }; # approximate, from peter onyisi
$mcprop->{ "106405" } = { "name" => "B_ccmu15X" , "kf" => 1. , "eff" => 1. , "xsec" => 28000. }; # approximate, from peter onyisi
# from HSG3 overriding the above
$mcprop->{ "106043" } = { "name" => "Wenu_no_filter" , "kf" => 1.17 , "eff" => 1. , "xsec" => 8894.09 };
$mcprop->{ "106044" } = { "name" => "Wmunu_no_filter" , "kf" => 1.17 , "eff" => 1. , "xsec" => 8894.06 };
$mcprop->{ "106046" } = { "name" => "Zee_no_filter" , "kf" => 1.15 , "eff" => 1. , "xsec" => 851.019 };
$mcprop->{ "106047" } = { "name" => "Zmumu_no_filter" , "kf" => 1.15 , "eff" => 1. , "xsec" => 851.011 };
$mcprop->{ "105200" } = { "name" => "ttbar" , "xsec" => 164.57 , "eff" => 0.5562 , "kf" => 1. };
$mcprop->{ "105921" } = { "name" => "qq->WpWm->enuenu" , "xsec" => 0.524 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105922" } = { "name" => "qq->WpWm->enumunu" , "xsec" => 0.524 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105923" } = { "name" => "qq->WpWm->enutaunu" , "xsec" => 0.524 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105924" } = { "name" => "qq->WpWm->munumunu" , "xsec" => 0.524 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105925" } = { "name" => "qq->WpWm->munuenu" , "xsec" => 0.524 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105926" } = { "name" => "qq->WpWm->munutaunu" , "xsec" => 0.524 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105927" } = { "name" => "qq->WpWm->taunutaunu" , "xsec" => 0.524 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105928" } = { "name" => "qq->WpWm->taunuenu" , "xsec" => 0.524 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105929" } = { "name" => "qq->WpWm->taunumunu" , "xsec" => 0.524 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105930" } = { "name" => "qq->ZZ->llqq" , "xsec" => 0.52354 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105931" } = { "name" => "qq->ZZ->llll" , "xsec" => 0.02468 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105932" } = { "name" => "qq->ZZ->llnunu" , "xsec" => 0.15033 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105940" } = { "name" => "qq->WpZ->lnuqq" , "xsec" => 1.6889 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105941" } = { "name" => "qq->WpZ->lnull" , "xsec" => 0.15924 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105970" } = { "name" => "qq->WmZ->lnuqq" , "xsec" => 0.91264 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105971" } = { "name" => "qq->WmZ->lnull" , "xsec" => 0.08605 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "105972" } = { "name" => "qq->WmZ->qqll" , "xsec" => 0.2693 , "eff" => 1. , "kf" => 1. };
$mcprop->{ "106022" } = { "name" => "PythiaWtaunu_1Lepton" , "xsec" => 8916.33 , "eff" => 1. , "kf" => 1.17 };
$mcprop->{ "106024" } = { "name" => "WpZ_taunull" , "xsec" => 0.07962, "eff" => 1. , "kf" => 1. };
$mcprop->{ "106025" } = { "name" => "WpZ_lnutautau" , "xsec" => 0.07962, "eff" => 1. , "kf" => 1. };
$mcprop->{ "106026" } = { "name" => "WpZ_taunutautau" , "xsec" => 0.03981, "eff" => 1. , "kf" => 1. };
$mcprop->{ "106027" } = { "name" => "WmZ_taunull" , "xsec" => 0.04302, "eff" => 1. , "kf" => 1. };
$mcprop->{ "106029" } = { "name" => "WmZ_taunutautau" , "xsec" => 0.02151, "eff" => 1. , "kf" => 1. };
$mcprop->{ "106052" } = { "name" => "Ztautau" , "xsec" => 856.967, "eff" => 1. , "kf" => 1.15 };
$mcprop->{ "108319" } = { "name" => "DrellYan_mumu" , "xsec" => 1245.7, "eff" => 1. , "kf" => 1. };
$mcprop->{ "108320" } = { "name" => "DrellYan_ee" , "xsec" => 1242.2, "eff" => 1. , "kf" => 1. };
$mcprop->{ "108326" } = { "name" => "B_bbe15X" , "xsec" => 0., "eff" => 1. , "kf" => 1. };
$mcprop->{ "108327" } = { "name" => "B_cce15X" , "xsec" => 0., "eff" => 1. , "kf" => 1. };
$mcprop->{ "108340" } = { "name" => "st_tchan_enu" , "xsec" => 6.34, "eff" => 1. , "kf" => 1. };
$mcprop->{ "108341" } = { "name" => "st_tchan_munu_" , "xsec" => 6.34, "eff" => 1. , "kf" => 1. };
$mcprop->{ "108342" } = { "name" => "st_tchan_taunu_" , "xsec" => 6.34, "eff" => 1. , "kf" => 1. };
$mcprop->{ "108343" } = { "name" => "st_schan_enu_" , "xsec" => 0.43, "eff" => 1. , "kf" => 1. };
$mcprop->{ "108344" } = { "name" => "st_schan_munu_" , "xsec" => 0.43, "eff" => 1. , "kf" => 1. };
$mcprop->{ "108345" } = { "name" => "st_schan_taunu_" , "xsec" => 0.43, "eff" => 1. , "kf" => 1. };
$mcprop->{ "108405" } = { "name" => "B_bbmu15X" , "xsec" => 0., "eff" => 1. , "kf" => 1. };
$mcprop->{ "109070" } = { "name" => "H110_WpWm_lnulnu" , "xsec" => 0., "eff" => 1. , "kf" => 1. };
$mcprop->{ "109071" } = { "name" => "H120_WpWm_lnulnu" , "xsec" => 0.0771, "eff" => 1. , "kf" => 1. };
$mcprop->{ "109072" } = { "name" => "H140_WpWm_lnulnu" , "xsec" => 0.18778, "eff" => 1. , "kf" => 1. };
$mcprop->{ "109073" } = { "name" => "H150_WpWm_lnulnu" , "xsec" => 0.21974, "eff" => 1. , "kf" => 1. };
$mcprop->{ "109074" } = { "name" => "H160_WpWm_lnulnu" , "xsec" => 0.24411, "eff" => 1. , "kf" => 1. };
# unknown
# $mcprop->{ "106059" } = { "name" => "B_bbe15X" , "kf" => 1. , "eff" => 1. , "xsec" => 0. };
# $mcprop->{ "106405" } = { "name" => "B_cce15X" , "kf" => 1. , "eff" => 1. , "xsec" => 0. };


foreach my $sampledir (@sampledirs) {
    chomp $sampledir;
    # remove trailing slash
    $sampledir =~ s/\/$//;
    my $defname = basename( $sampledir );
    #print "$sampledir\n";
    $defname =~ s/\*//g;
    my $bname = $defname;
    # $defname =~ s/\.[0-9]+$//;
    my $tmpname = $defname;
    $defname ="${tmpname}.in";
    $defname =~ s/^user\..*\.mc10/mc10/;
    $defname =~ s/^user\..*\.data11/data11/;
    my @listoffiles = `echo $sampledir/*.root* | xargs ls -1`;
    my @def = ();
    push( @def , "V 1\n" );
    push( @def , "# add metadata\n" );
    if( $defname =~ m/data/ ) {
        my $pname = "";
        if( $bname =~ m/period([A-Za-z][0-9]*)\./i ) { 
            $pname = $1;
        }
        if( $pname eq "" ) {
            print "unknown data period: $bname \n skipping...\n";
            next;
        }
        my $lumipname = $pname;
        if( $bname =~ m/Egamma/ ) { 
            push( @def , "Fs name period${pname}_egamma\n" );
            push( @def , "Ft egamma\n" );
            $lumipname = "wz2011$pname";
        } elsif( $bname =~ m/Muons/ ) { 
            push( @def , "Fs name period${pname}_muons\n" );
            push( @def , "Ft muons\n" );
            $lumipname = "wz2011$pname";
        } elsif( $bname =~ m/JetTauEtmiss/ ) {
            push( @def , "Fs name period${pname}_jettauetmiss\n" );
            push( @def , "Ft jettauetmiss\n" );
            $lumipname = "gj2011$pname";
        }
        push( @def , "Fs legend data\n" );
        push( @def , "Fb mc false\n" );
        push( @def , "Ft data\n" );
        push( @def , "Ft period$pname\n"  );
        # get luminosity
        if( ! exists $dataprop->{ $lumipname } ) {
            print " warning: data properties for $pname not defined\n";
            next;
        }
        if( ! exists $dataprop->{ $lumipname }->{ "lumi" } ) {
            print " warning: lumi not defined for data period $pname\n";
            next;
        }
        my $lumi = $dataprop->{ $lumipname }->{ "lumi" };
        push( @def , "Fn lumi $lumi\n" );
    } else {
        # get run number
        my $run = "";
        if( $defname =~ m/mc10_7TeV\.([0-9]+)\./ ) {
            $run = "$1";
        }
        my $xsec = $mcprop->{ $run }->{ "xsec" };
        if( ! exists $mcprop->{ $run } ) {
            print " warning: run $run dataset $defname not defined \n";
            next;
        }
        my $mcname = $mcprop->{ $run }->{ "name" };
        push( @def , "Fs name $mcname\n" );
        if( ! exists $mcprop->{ $run }->{ "xsec" } ) {
            print " warning: run $run dataset $defname cross section not defined \n";
            next;
        }
        push( @def , "Fs legend $bname\n" );
        push( @def , "Fb mc true\n" );
        if( $bname =~ m/r2302\_r2300/ ) {
            push( @def , "Ft mc10b\n" );     
        }
        push( @def , "Fn xsec ${xsec}\n" );
        my $kfactor = $mcprop->{ $run }->{ "kf" };
        my $filtereff = $mcprop->{ $run }->{ "eff" };
        my $weight = $kfactor*$filtereff;
        push( @def , "# kfactor: $kfactor filtereff: $filtereff\n" );
        push( @def , "Fn weight ${weight}\n" );
        push( @def , "Fs absolute_normalization\n" );
    }

    push( @def , "# peter ntuples (v1_26)\n" );
    foreach my $filename (@listoffiles) {
        chomp $filename;
        push( @def , "DPETER $filename\n" );
        push( @def , "E\n" );
    }
    write_file( $defname , @def );
}


sub uniq {
    my %h;
    return grep { !$h{$_}++ } @_
}


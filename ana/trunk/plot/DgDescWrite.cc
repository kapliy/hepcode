#include <DataGraphics/DataGraphics.hh>
#include <DataGraphics/DgXMLSampleDescription.hh>
#include <string>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <boost/foreach.hpp>

using namespace std;

int
main( void )
{
  cout << " building sample description files." << endl;

  // looks for text files named "*.in" (in the current directory) that contain lines like:
  //     "Fb mc true"  <= this is a mc file
  //     "F xsec 12345" <= xsec in pb
  //     "F lumi 12345" <= luminosity of sampel in /pb
  //     "F

  std::set<std::string> streams;
  streams.insert( "egamma" );
  streams.insert( "muons" );

  std::set<std::string> periods;
  periods.insert( "B" );
  periods.insert( "D1" );
  periods.insert( "D2" );
  periods.insert( "D3" );
  periods.insert( "D4" );
  periods.insert( "D5" );
  periods.insert( "D6" );
  periods.insert( "D7" );
  periods.insert( "E" );
  periods.insert( "F" );

  map<string,float> lumi;
  lumi["egamma.periodB"] = 1;

  {
    BOOST_FOREACH( const string& stream , streams ) {
      BOOST_FOREACH( const string& period , periods ) {
        DgXMLSampleDesciption d( "data11_period"+period+"_"+stream , "data "+stream+" period "+period , "period "+period+" "+stream+" data" );
        d.set_tag( "data" );
        d.set_tag( stream );
        d.set_tag( period );
        d.set_luminosity( lumi[stream+"."+period] );
        d.save ("./data11_7TeV.period"+period+".physics_Egamma.merge.AOD.repro08.ntuple.v1_21.in
      }
    }
  }

  {
    DgXMLSampleDesciption d( "mc10_MC@NLO_WW_ee" , "data "+stream+" period "+period , "period "+period+" "+stream+" data" );

mc10_7TeV.105921.McAtNlo_JIMMY_WpWm_enuenu.merge.AOD.e707_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105922.McAtNlo_JIMMY_WpWm_enumunu.merge.AOD.e707_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105923.McAtNlo_JIMMY_WpWm_enutaunu.merge.AOD.e707_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105924.McAtNlo_JIMMY_WpWm_munumunu.merge.AOD.e707_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105925.McAtNlo_JIMMY_WpWm_munuenu.merge.AOD.e707_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105926.McAtNlo_JIMMY_WpWm_munutaunu.merge.AOD.e707_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105927.McAtNlo_JIMMY_WpWm_taunutaunu.merge.AOD.e707_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105928.McAtNlo_JIMMY_WpWm_taunuenu.merge.AOD.e707_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105929.McAtNlo_JIMMY_WpWm_taunumunu.merge.AOD.e707_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105930.McAtNlo_JIMMY_ZZ_llqq.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105931.McAtNlo_JIMMY_ZZ_llll.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105932.McAtNlo_JIMMY_ZZ_llnunu.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105940.McAtNlo_JIMMY_WpZ_lnuqq.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105941.McAtNlo_JIMMY_WpZ_lnull.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105970.McAtNlo_JIMMY_WmZ_lnuqq.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105971.McAtNlo_JIMMY_WmZ_lnull.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.105972.McAtNlo_JIMMY_WmZ_qqll.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.106022.PythiaWtaunu_1Lepton.merge.AOD.e574_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.106024.McAtNlo_JIMMY_WpZ_taunull.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.106025.McAtNlo_JIMMY_WpZ_lnutautau.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.106026.McAtNlo_JIMMY_WpZ_taunutautau.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.106027.McAtNlo_JIMMY_WmZ_taunull.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.106029.McAtNlo_JIMMY_WmZ_taunutautau.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.106043.PythiaWenu_no_filter.merge.AOD.e574_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.106044.PythiaWmunu_no_filter.merge.AOD.e574_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.106046.PythiaZee_no_filter.merge.AOD.e574_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.106047.PythiaZmumu_no_filter.merge.AOD.e574_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.106052.PythiaZtautau.merge.AOD.e574_s934_s946_r2263_r2260.ntuple.v1_21.in
mc10_7TeV.108326.PythiaB_bbe15X.merge.AOD.e574_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.108327.PythiaB_cce15X.merge.AOD.e574_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.108340.st_tchan_enu_McAtNlo_Jimmy.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.108341.st_tchan_munu_McAtNlo_Jimmy.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.108342.st_tchan_taunu_McAtNlo_Jimmy.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.108343.st_schan_enu_McAtNlo_Jimmy.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.108344.st_schan_munu_McAtNlo_Jimmy.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.108345.st_schan_taunu_McAtNlo_Jimmy.merge.AOD.e598_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.108405.PythiaB_bbmu15X.merge.AOD.e574_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.109070.McAtNlo_JIMMY_H110_WpWm_lnulnu.merge.AOD.e707_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.109071.McAtNlo_JIMMY_H120_WpWm_lnulnu.merge.AOD.e707_s933_s946_r2215_r2260.ntuple.v1_21.in
mc10_7TeV.109072.McAtNlo_JIMMY_H140_WpWm_lnulnu.merge.AOD.e707_s933_s946_r2215_r2260.ntuple.v1_21.in
    
  }


}

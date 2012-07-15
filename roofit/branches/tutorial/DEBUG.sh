#!/bin/bash
source bashmap.sh

input=/share/t3data3/antonk/ana/ana_v29G_07102012_DtoM_jerjes_wptw_stacoCB_all # includes bugfix for KC systematics (tight was actully doing default); new jerjes studies (rawjet vs caljet); new pileup histo; fixed lauren's bug average_mu was rounded off to int; V29G from tier3 xrootd (copied); removed overlap removal for met propagation;    wpt weight applied to histograms; require born level mu/nu status AND require parent match -> very rarely get two muons per event!!!

#./stack2.py --input /share/t3data3/antonk/ana/ana_v29G_07102012_DtoM_jerjes_wptw_stacoCB_all -b --charge 0 --lvar met --lbin 50,0,120 --preNN "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --preNQ "ptiso40/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2" --var met --bin 200,0,200 -o TEST -t aiso_met0to120 --cut mcw*puw*effw*trigw*wptw -m qcdfit --bgsig 5 --bgewk 5 --bgqcd 3

./stack2.py --input /share/t3data3/antonk/ana/ana_v29G_07102012_DtoM_jerjes_wptw_stacoCB_all -b --charge 0 --lvar met --lbin 50,0,120 --preNN "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --preNQ "ptiso40/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2" --var 'fabs(l_eta)' --bin 10,0,2.5 --hsource lepton_absetav -o TEST -t aiso_met0to120 --cut mcw*puw*effw*trigw*wptw -m qcdfit --bgsig 5 --bgewk 5 --bgqcd 3

./stack2.py --input /share/t3data3/antonk/ana/ana_v29G_07102012_DtoM_jerjes_wptw_stacoCB_all -b --charge 1 --lvar met --lbin 50,0,120 --preNN "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --preNQ "ptiso40/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2" --var 'fabs(l_eta)' --bin 10,0,2.5 --hsource lepton_absetav -o TEST -t aiso_met0to120 --cut mcw*puw*effw*trigw*wptw -m qcdfit --bgsig 5 --bgewk 5 --bgqcd 3



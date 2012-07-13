#!/bin/bash
source bashmap.sh

input=/share/t3data3/antonk/ana/ana_v29G_07102012_DtoM_jerjes_wptw_stacoCB_all # includes bugfix for KC systematics (tight was actully doing default); new jerjes studies (rawjet vs caljet); new pileup histo; fixed lauren's bug average_mu was rounded off to int; V29G from tier3 xrootd (copied); removed overlap removal for met propagation;    wpt weight applied to histograms; require born level mu/nu status AND require parent match -> very rarely get two muons per event!!!

bgqcd=0

i=0
#gput tag_preNN ${i}  "--pre \"${zpre_jordan}\" --cut \"${cut}\""
((i++))
preNN='ptiso20/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2'
preNQ='ptiso20/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2'
preFN='ptiso20/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2'
preFQ='ptiso20/l_pt>0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2'


for bgsig in 0 1 4 5; do
    ./stack2.py --input ${input} -b --lvar "met" --lbin "200,0,100" --charge 2 --var "met" --bin "200,0,200" -o TEST -t X --preNN="${preNN}" --preNQ="${preNQ}" --preFN="${preFN}" --preFQ="${preFQ}" --cut "mcw*puw*effw*trigw*wptw" -m qcdfit --bgsig ${bgsig} --bgewk 5 --bgqcd ${bgqcd}
    for i in {0..10}; do
	continue
	#./stack2.py --input ${input} -b --lvar "met" --lbin "200,0,100" --charge 2 --var "met" --bin "200,0,200" -o TEST -t X_ETA_${i} --preNN="${preNN}" --preNQ="${preNQ}" --preFN="${preFN}" --preFQ="${preFQ}" --cut "mcw*puw*effw*trigw*wptw" -m qcdfit --bgsig ${bgsig} --bgewk 5 --bgqcd ${bgqcd} --extra ${i} &> LOG.${i} &
    done
done
wait

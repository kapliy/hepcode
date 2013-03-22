#!/bin/bash

fin1=/share/t3data3/antonk/ana/ana_v29J_03172013_paper_stacoCB_all/pt20/  # nominal (MG)
fin2=/share/t3data3/antonk/ana/ana_v29J_03172013_paper_mu18_stacoCB_all/pt20/ # variation (muid)

# profile of match distrance
if [ 0 -eq 1 ]; then
    fin=${fin1}
    for p in D L; do
	for q in 0 1; do
	    for nt in w z; do
		./max_trigmatch.py -m 0 -f ${fin}/data_period${p}/root_data_period${p}.root -t data${p} -n ${nt} -q ${q} &
		if [ "$nt" == "z" ]; then
		    ./max_trigmatch.py -m 0 -f ${fin}/data_period${p}/root_data_period${p}.root -t data${p} -n ${nt} -q ${q} --fail &
		fi
	    done
	done
    done
    echo "wait..."
    wait
fi

# dump match
if [ 0 -eq 1 ]; then
    fin=${fin1}
    nt=w
    for p in D L; do
	for q in 0 1; do
	    /home/antonk/roofit/max_trigmatch.py -m 1 -f ${fin}/data_period${p}/root_data_period${p}.root -t MG_data${p} -n ${nt} -q ${q} &
	done
    done
    echo "wait for MG..."
    wait
    fin=${fin2}
    for p in D L; do
	for q in 0 1; do
	    /home/antonk/roofit/max_trigmatch.py -m 1 -f ${fin}/data_period${p}/root_data_period${p}.root -t MU_data${p} -n ${nt} -q ${q} &
	done
    done
    echo "wait for MU..."
    wait
fi

# dump z events in bin 10 z-, where 
if [ 0 -eq 1 ]; then
    fin=${fin1}
    nt=z
    for p in L; do
	for q in 1; do
	    /home/antonk/roofit/max_trigmatch.py --bin ' && lN_eta<-1.95 && lN_eta>-2.18' -m 1 -f ${fin}/data_period${p}/root_data_period${p}.root -t tagfail_period${p} -n ${nt} --fail -q ${q} &
	done
    done
    echo "wait for MG..."
    wait
fi

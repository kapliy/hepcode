#!/bin/bash
source bashmap.sh

# Z tag and probe studies

# input
common="--input /share/t3data3/antonk/ana/ana_v29I_11302012_edboard_stacoCB_all"
# default cut
cut="mcw*puw*wzptw*vxw*ls1w*ls2w*effw*isow*trigw"

###############################################################################
# Parse command line
###############################################################################

# Z tag and probe
if [ "1" == "1" ]; then
    i=0
    # iso relative
    BEF="lX_idhits==1 && fabs(lP_z0)<10. && lX_pt>20.0 && fabs(lX_eta)<2.4 && lY_idhits==1 && fabs(lN_z0)<10. && lY_pt>20.0 && fabs(lY_eta)<2.4 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso20/lX_pt<0.1"
    AFT="${PRE} && lY_ptiso20/lY_pt<0.1"
    gput tags ${i} ptrel20  "--prebef \"${BEF}\" --preaft \"${BEF} && lY_ptiso20/lY_pt<0.1\""
    ((i++))
    # trigger
    BEF="lX_idhits==1 && fabs(lP_z0)<10. && lX_pt>20.0 && fabs(lX_eta)<2.4 && lY_idhits==1 && fabs(lN_z0)<10. && lY_pt>20.0 && fabs(lY_eta)<2.4 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso20/lX_pt<0.1 && lY_ptiso20/lY_pt<0.1"
    gput tags ${i} trigEF  "--prebef \"${BEF}\" --preaft \"${BEF} && (lY_trigEF<0.2 || lY_trigEFb<0.2)\""
    ((i++))
    # iso absolute
    BEF="lX_idhits==1 && fabs(lP_z0)<10. && lX_pt>20.0 && fabs(lX_eta)<2.4 && lY_idhits==1 && fabs(lN_z0)<10. && lY_pt>20.0 && fabs(lY_eta)<2.4 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso40<2.0 && lX_etiso40<2.0"
    #gput tags ${i} etiso40  "--prebef \"${BEF}\" --preaft \"${BEF} && lY_etiso40<2.0\""
    ((i++))
    #gput tags ${i} ptiso40  "--prebef \"${BEF}\" --preaft \"${BEF} && lY_ptiso40<2.0\""
    ((i++))
    # MCP (on isolated muons)
    BEF='lX_idhits==1 && fabs(lP_z0)<10. && lX_pt>20.0 && fabs(lX_eta)<2.4 && fabs(lN_z0)<10. && lY_pt>20.0 && fabs(lY_eta)<2.4 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso20/lX_pt<0.1 && lY_ptiso20/lY_pt<0.1'
    gput tags ${i} mcphits  "--prebef \"${BEF}\" --preaft \"${BEF} && lY_idhits==1\""
    ((i++))
    # run all jobs
    for itag in `gkeys tags`; do  # cuts
	tag=`ggeta tags $itag`
	opts=`ggetb tags $itag`
	for bgsig in 0 3; do  # Pythia or Alpgen MC
	    for m in 101 103; do  # Use BG subtraction?
		eval ./stack2.py ${common} ${opts} --bgsig ${bgsig} --cut ${cut} -m${m} --ntuple z -b --var 'lY_pt'  --bin '32,10,140' -t ${tag}_pt_MC${bgsig}&
		eval ./stack2.py ${common} ${opts} --bgsig ${bgsig} --cut ${cut} -m${m} --ntuple z -b --var 'lY_eta' --bin '30,-2.5,2.5' -t ${tag}_eta_MC${bgsig} &
	    done
	done
	wait
    done
    wait
    echo DONE
fi

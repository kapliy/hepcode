#!/bin/bash
# RooKeysPdf studies:
data=data_data
mc=mc_zmumu_mc_zmumu

for glb in scale shift; do
xtra=""
if [ "$glb" == "shift" ]; then xtra=1; fi

tg=$mc
reg=BB
tag=${glb}_truth_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag $xtra

tg=$mc
reg=CC
tag=${glb}_truth_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag $xtra

tg=$mc
reg=AA
tag=${glb}_truth_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag $xtra

tg=$data
reg=BB
tag=${glb}_data_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag $xtra

tg=$data
reg=Baa
tag=${glb}_data_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag $xtra

tg=$data
reg=Bcc
tag=${glb}_data_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag $xtra

tg=$data
reg=CC
tag=${glb}_data_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag $xtra

tg=$data
reg=AA
tag=${glb}_data_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag $xtra

done

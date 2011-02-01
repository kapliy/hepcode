# RooKeysPdf studies:
data=data_data
mc=mc_zmumu_mc_zmumu
glb=scale

tg=$mc
reg=BB
tag=${glb}_truth_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag

tg=$mc
reg=CC
tag=${glb}_truth_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag

tg=$mc
reg=AA
tag=${glb}_truth_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag

tg=$data
reg=BB
tag=${glb}_data_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag

tg=$data
reg=CC
tag=${glb}_data_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag

tg=$data
reg=AA
tag=${glb}_data_$reg
python keysfit.py root_all.root $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tg.root/dg/dg/st_z_final/$reg/graph_lpt_P_N $tag

#!/bin/bash

# gaus and egge yield consistent results!
# but looks out for the non pos-def egge error matrices.

# truth on full dataset, egge fit
if [ "0" -eq "1" ]; then
    for func in egge; do
	for reg in AA BB CC BA BC AB CB; do
	    ./zpeak.py -b --res 3 --${func} --data mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 100000 -t mc_${reg}_${func}
	done
    done
fi;
# truth on full dataset, gaussian core fit
if [ "0" -eq "1" ]; then
    func=gaus
    for reg in AA BB CC BA BC AB CB; do
	./zpeak.py -b --res 3 --${func} --data mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 100000 -t mc_${reg}_${func} --min 88 --max 94
    done
fi;


# AA BB CC [COMBINED], egge fit
if [ "0" -eq "1" ]; then
    func=egge
    reg=AA
    ./zpeak.py -b --res 3 --${func} --mz0 91.01 --R 0.990 --eR 0.0052 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func}
    reg=BB
    ./zpeak.py -b --res 3 --${func} --mz0 90.87 --R 1.005 --eR 0.0036 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func}
    reg=CC
    ./zpeak.py -b --res 3 --${func} --mz0 91.04 --R 1.013 --eR 0.0051 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func}
fi;

# AA BB CC [COMBINED], gaussian core fit
if [ "0" -eq "1" ]; then
    func=gaus
    reg=AA
    ./zpeak.py -b --res 3 --${func} --mz0 91.06 --R 0.990 --eR 0.0052 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func} --min 86 --max 96
    reg=BB
    ./zpeak.py -b --res 3 --${func} --mz0 90.86 --R 1.005 --eR 0.0036 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func} --min 86 --max 96
    reg=CC
    ./zpeak.py -b --res 3 --${func} --mz0 91.04 --R 1.013 --eR 0.0051 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func} --min 86 --max 96
fi;


#BARREL-ENDCAP CROSS:
if [ "1" -eq "1" ]; then
    func=egge
    reg=BA
    ./zpeak.py -b --res 3 --${func} --mz0 90.963 --kp 1.00114 --ekp 0.00191 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func}
    reg=BC
    ./zpeak.py -b --res 3 --${func} --mz0 90.950 --kp 1.00114 --ekp 0.00191 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func}
    reg=AB
    ./zpeak.py -b --res 3 --${func} --mz0 90.914 --km 0.996 --ekm 0.002 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func}
    reg=CB
    ./zpeak.py -b --res 3 --${func} --mz0 90.902 --km 0.996 --ekm 0.002 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func}
fi

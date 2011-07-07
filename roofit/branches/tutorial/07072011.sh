out=asym07072011

#various data scales and MC smears
if [ "1" -eq "0" ]; then
    # my scales
    for f in WP WP_sc00 WP_sc01 WP_sc10 WP_sc11; do
	./stack.py -b -m11 -t $f --hmc WP/st_w_final/00_wmt/asym_abseta --hdata $f/st_w_final/00_wmt/asym_abseta --rebin 4 --antondb $out
    done
    # MC shifts (pythia)
    for f in WP_sm{0..5}; do
	./stack.py -b -m11 -t $f --hmc $f/st_w_final/00_wmt/asym_abseta --hdata WP/st_w_final/00_wmt/asym_abseta --rebin 4 --antondb $out
    done
    # MC shift (Alpgen-jimmy)
    for f in WP; do
	./stack.py -b -m11 -t jimmy_$f --hmc $f/st_w_final/00_wmt/asym_abseta --hdata WP/st_w_final/00_wmt/asym_abseta --rebin 4 --antondb $out --bgsig 3
    done
    # MC shift (MC@NLO) - needs a hack
    for f in WP; do
	./stack.py -b -m11 -t mcnlo_$f --hmc $f/st_w_final/00_wmt/asym_abseta --hdata WP/st_w_final/00_wmt/asym_abseta --rebin 4 --antondb $out --bgsig 1
    done
fi


# generate pictures
if [ "1" -eq "0" ]; then
    ./stack.py -b -m1 -t eta --hmc WJ/st_w_final/00_wmt/asym_eta --hdata WJ/st_w_final/00_wmt/asym_eta --rebin 2
    ./stack.py -b -m1 -t met --hmc WJ/st_w_final/00_wmt/met --hdata WJ/st_w_final/00_wmt/met --rebin 2
    ./stack.py -b -m1 -t w_mt --hmc WJ/st_w_final/00_wmt/w_mt --hdata WJ/st_w_final/00_wmt/w_mt --rebin 2
    ./stack.py -b -q 0 -m1 -t w_mt_POS --hmc WJ/st_w_final/00_wmt/w_mt --hdata WJ/st_w_final/00_wmt/w_mt --rebin 2
    ./stack.py -b -q 1 -m1 -t w_mt_NEG --hmc WJ/st_w_final/00_wmt/w_mt --hdata WJ/st_w_final/00_wmt/w_mt --rebin 2
fi;

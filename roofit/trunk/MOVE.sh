folders="data_20110425D data_20110425B data_20110425 data_2010repro data"

for f in $folders; do
    dout=OUT.${f}
    n=`find . -maxdepth 1 -type f \( -name \*${f}\*png -o -name \*${f}\*rtxt \) | wc -l`
    echo $dout - $n
    if [ "${n}" -gt "0" ]; then
	mkdir -p ${dout}/
	find . -maxdepth 1 -type f \( -name \*${f}\*png -o -name \*${f}\*rtxt \) -exec mv {} $dout/ \;
	cd $dout
	mkdir -p SHF
	find . -maxdepth 1 -type f -name SHF_\*${f}\* -exec mv {} SHF/ \;
	mkdir -p KLU
	find . -maxdepth 1 -type f -name KLU_\*${f}\* -exec mv {} KLU/ \;
	mkdir -p AKLU
	find . -maxdepth 1 -type f -name AKLU_\*${f}\* -exec mv {} AKLU/ \;
	cd -
    fi;
done

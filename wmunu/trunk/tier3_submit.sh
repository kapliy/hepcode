anadir=/home/antonk/analysis
outdir=/home/antonk/output
DATADIR=/pnfs/uct3/data/users/antonk/ANALYSIS/ICHEP_DATA
MCDIR=/pnfs/uct3/data/users/antonk/ANALYSIS/ICHEP_MC

id=0
for dsampledir in $DATADIR; do
    dsample=periodABC
    echo "Submitting job $id: $dsample"
    mkdir -p $outdir/$dsample
    qsub -v anadir=${anadir},inpdir="${dsampledir}/*",outdir=${outdir}/$dsample,extras="--data" -N looper${id} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
    ((id++))
done

for dsampledir in `find $MCDIR -maxdepth 1 -mindepth 1 -type d | sort`; do
    dsample=`basename $dsampledir`
    echo "Submitting job $id: $dsample"
    mkdir -p $outdir/$dsample
    qsub -v anadir=${anadir},inpdir=${dsampledir},outdir=${outdir}/$dsample,extras="" -N looper${id} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
    ((id++))
done

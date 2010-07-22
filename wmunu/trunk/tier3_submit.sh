outdir=/home/antonk/output
anadir=/home/antonk/analysis
MCDIR="/pnfs/uct3/data/users/antonk/ANALYSIS/JULY_MC"
DATADIR="/pnfs/uct3/data/users/antonk/ANALYSIS/JULY_DATA"
TRIGGER="L1_MU6"
id=0

#data [period by period]
for dsampledir in `find $DATADIR -maxdepth 1 -mindepth 1 -type d -name \*period\* | sort`; do
    dsample=`basename $dsampledir`
    echo "Submitting job $id: $dsample"
    mkdir -p $outdir/$dsample
    qsub -v anadir=${anadir},inpfiles="${dsampledir}/*root*",outdir=${outdir}/$dsample,extras="--trigger ${TRIGGER} --data" -N looper${id}${dsample} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
    ((id++))
done

#mc
for dsampledir in `find $MCDIR -maxdepth 1 -mindepth 1 -type d | sort`; do
    dsample=`basename $dsampledir`
    echo "Submitting job $id: $dsample"
    mkdir -p $outdir/$dsample
    qsub -v anadir=${anadir},inpfiles="${dsampledir}/*root*",outdir=${outdir}/$dsample,extras="--trigger ${TRIGGER}" -N looper${id}${dsample} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
    ((id++))
    qsub -v anadir=${anadir},inpfiles="${dsampledir}/*root*",outdir=${outdir}/tcuts/$dsample,extras="--trigger ${TRIGGER} --truthcuts" -N looper${id}${dsample}TC -o $outdir/$dsample/log.stdout.tcuts -e $outdir/$dsample/log.stderr.tcuts pbs.sh
    ((id++))
done

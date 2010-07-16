anadir=/home/antonk/analysis
MCDIR="/pnfs/uct3/data/users/antonk/ANALYSIS/ICHEP_MC"
id=0

# period ABC only with L1_MU0
outdir=/home/antonk/outputABC
DATAFILES="/pnfs/uct3/data/users/antonk/ANALYSIS/ICHEP_DATA/*period[ABC]*/*root*"
#data
dsample=periodABC
echo "Submitting job $id: $dsample"
mkdir -p $outdir/$dsample
qsub -v anadir=${anadir},inpfiles="${DATAFILES}",outdir=${outdir}/$dsample,extras="--data --trigger L1_MU0" -N looper${id} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
((id++))
#mc
for dsampledir in `find $MCDIR -maxdepth 1 -mindepth 1 -type d | sort`; do
    dsample=`basename $dsampledir`
    echo "Submitting job $id: $dsample"
    mkdir -p $outdir/$dsample
    qsub -v anadir=${anadir},inpfiles="${dsampledir}/*root*",outdir=${outdir}/$dsample,extras="--trigger L1_MU0" -N looper${id} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
    ((id++))
    qsub -v anadir=${anadir},inpfiles="${dsampledir}/*root*",outdir=${outdir}/tcuts/$dsample,extras="--trigger L1_MU0 --truthcuts" -N looper${id} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
    ((id++))
done

# all data
outdir=/home/antonk/output
DATAFILES="/pnfs/uct3/data/users/antonk/ANALYSIS/ICHEP_DATA/*period*/*root*"
#data
dsample=periodALLD2
echo "Submitting job $id: $dsample"
mkdir -p $outdir/$dsample
qsub -v anadir=${anadir},inpfiles="${DATAFILES}",outdir=${outdir}/$dsample,extras="--data" -N looper${id} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
((id++))
#mc
for dsampledir in `find $MCDIR -maxdepth 1 -mindepth 1 -type d | sort`; do
    dsample=`basename $dsampledir`
    echo "Submitting job $id: $dsample"
    mkdir -p $outdir/$dsample
    qsub -v anadir=${anadir},inpfiles="${dsampledir}/*root*",outdir=${outdir}/$dsample,extras="" -N looper${id} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
    ((id++))
    qsub -v anadir=${anadir},inpfiles="${dsampledir}/*root*",outdir=${outdir}/tcuts/$dsample,extras="--truthcuts" -N looper${id} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
    ((id++))
done

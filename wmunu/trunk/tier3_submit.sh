anadir=/home/antonk/analysis
outdir=/home/antonk/output
DATADIR="/pnfs/uct3/data/users/antonk/ANALYSIS/ICHEP_DATA/*period*/*root*"
DATADIRABC="/pnfs/uct3/data/users/antonk/ANALYSIS/ICHEP_DATA/*period[ABC]*/*root*"
MCDIR="/pnfs/uct3/data/users/antonk/ANALYSIS/ICHEP_MC"

id=0
dsampledir="$DATADIRABC"
dsample=periodABC
echo "Submitting job $id: $dsample"
mkdir -p $outdir/$dsample
qsub -v anadir=${anadir},inpdir="${dsampledir}",outdir=${outdir}/$dsample,extras="--data" -N looper${id} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
((id++))
exit -1

dsampledir=$DATADIR
dsample=periodABCD1
echo "Submitting job $id: $dsample"
mkdir -p $outdir/$dsample
#qsub -v anadir=${anadir},inpdir="${dsampledir}/*",outdir=${outdir}/$dsample,extras="--data" -N looper${id} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
((id++))
#exit -1

for dsampledir in `find $MCDIR -maxdepth 1 -mindepth 1 -type d | sort`; do
    dsample=`basename $dsampledir`
    echo "Submitting job $id: $dsample"
    mkdir -p $outdir/$dsample
    #qsub -v anadir=${anadir},inpdir="${dsampledir}/*root*",outdir=${outdir}/$dsample,extras="" -N looper${id} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
    ((id++))
done

outdir=/home/antonk/pdfoutput
anadir=/home/antonk/analysis
id=0

#data [period by period]
for dsample in `seq 0 40`; do
    echo "Submitting job $id: $dsample"
    mkdir -p $outdir/$dsample
    qsub -v anadir=${anadir},outdir=${outdir}/$dsample,extras="-n300000 -p ${dsample}" -N pdf${id} -o $outdir/$dsample/log.stdout -e $outdir/$dsample/log.stderr pbs.sh
    ((id++))
done

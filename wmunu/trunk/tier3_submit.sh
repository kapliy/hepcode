anadir=/home/antonk/analysis
outdir=/home/antonk/logs
runs="106044 105861 106022 106047 106052  109276 109277 109278 109279 109280 109281"
info="Wmunu  ttbar  Wtaunu Zmumu  Ztautau J0     J1     J2     J3     J4     J5"
echo $info

for rnum in `echo $runs`; do
    echo "Submitting job $rnum"
    qsub -v anadir=${anadir},outdir=${outdir},rnum=${rnum} -N looper${rnum} -o $outdir/$rnum/log$id.stdout -e $outdir/$rnum/log$id.stderr pbs.sh
done;

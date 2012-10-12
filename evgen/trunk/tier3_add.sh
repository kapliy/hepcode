for dsample in D3PD_HERWIG FLAT_HERWIG FLAT_PYTHIA; do
    f=root_$dsample.root
    rm -f $f
    hadd  -f3 $f output/$dsample/*root*
done


function list_to_sample () {
    echo V 1
    echo Fb mc 0
    echo Ft mc11c
    for ff in `cat $1 | sort`; do
        echo DPETER $ff
        echo E
    done
}

INFILE=$1
OUTFILE=$2

list_to_sample ${INFILE} > ${OUTFILE}


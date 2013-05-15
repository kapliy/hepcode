#!/bin/bash

# A bunch of helper arrays and functions to work with eta binning
# abseta binning is indexed starting with bin=1 to bin=11

# PRE STRING
function W_getpreA() {
    ibin=$1
    python -c "from binning import absetabins; print '&& l_eta>%.2f && l_eta<%.2f'%(absetabins[${ibin}-1],absetabins[${ibin}])"
}
function W_getpreC() {
    ibin=$1
    python -c "from binning import absetabins; print '&& l_eta<-%.2f && l_eta>-%.2f'%(absetabins[${ibin}-1],absetabins[${ibin}])"
}
function Z_getpreA() {
    ibin=$1
    iq=$2
    if [ "$2" == "P" -o "$2" == "N" ]; then
	python -c "from binning import absetabins; print '&& l%s_eta>%.2f && l%s_eta<%.2f'%(\"${iq}\",absetabins[${ibin}-1],\"${iq}\",absetabins[${ibin}])"
    else
	echo "ERROR: Z binning functions need two arguments: ibin and charge (P or N)"
	exit 1
    fi
}
function Z_getpreC() {
    ibin=$1
    iq=$2
    if [ "$2" == "P" -o "$2" == "N" ]; then
	python -c "from binning import absetabins; print '&& l%s_eta<-%.2f && l%s_eta>-%.2f'%(\"${iq}\",absetabins[${ibin}-1],\"${iq}\",absetabins[${ibin}])"
    else
	echo "ERROR: Z binning functions need two arguments: ibin and charge (P or N)"
	exit 1
    fi
}
function W_getpre() {
    ibin=$1
    iside=$2
    if [ "${iside}" == "A" ]; then W_getpreA ${ibin}; fi
    if [ "${iside}" == "C" ]; then W_getpreC ${ibin}; fi
}
function Z_getpre() {
    ibin=$1
    iq=$2
    iside=$3
    if [ "${iside}" == "A" ]; then Z_getpreA ${ibin} ${iq}; fi
    if [ "${iside}" == "C" ]; then Z_getpreC ${ibin} ${iq}; fi
}

# BINNING RANGE STRING
function V_getbinsA() {
    ibin=$1
    python -c "from binning import absetabins; print '40,%.2f,%.2f'%(absetabins[${ibin}-1],absetabins[${ibin}])"
}
function V_getbinsC() {
    ibin=$1
    python -c "from binning import absetabins; print '40,-%.2f,-%.2f'%(absetabins[${ibin}],absetabins[${ibin}-1])"
}
function V_getbins() {
    ibin=$1
    iside=$2
    if [ "${iside}" == "A" ]; then V_getbinsA ${ibin}; fi
    if [ "${iside}" == "C" ]; then V_getbinsC ${ibin}; fi
}

# TEST
if [ 0 -eq 1 ]; then
    echo "Checking getpre"
    W_getpre 10 A
    W_getpre 10 C
    Z_getpre 10 P A
    Z_getpre 10 N C
    echo "Checking getbins"
    V_getbins 10 A
fi

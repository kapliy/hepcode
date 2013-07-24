#!/bin/bash

# Smart job runner to kill hung jobs
# Instead of running:
#> PROGRAM ARGS
# you can run
#> ./smart_runner.sh PROGRAM ARGS
# It will automatically monitor your program and will restart it
# if it detects an infinite loop.

PROGRAM=$@
if [ -z "${PROGRAM}" ]; then 
    echo "Usage: $0 commands"
    exit 0
fi;
pattern='R__unzip|ReadKeys|ReadBuffer'
ntry=1 # how many times should we try?
slp=10 # how often to check for process status?
maxsleepcycles=420 # 4200 seconds = 70 minutes

function monitor () {
    _pid=$1
    _log=$2
    _err=$3
    _try=$4
    echo "BEGIN MONITORING pid=${_pid} attempt=${_try}"
    z=0
    while ps -p ${_pid} > /dev/null; do
	report=`ps -p ${_pid} -o cputime=,rss=,vsz=`
	cat ${_log} | egrep -q "${pattern}" && {
	    echo "FAILED PROCESS: ${_pid} attempt=${_try} - detected stdout error"
	    ps -p ${_pid} > /dev/null && kill -SIGKILL ${_pid}
	    sleep 0.5
	    return 1
	}
	cat ${_err} | egrep -q "${pattern}" && {
	    echo "FAILED PROCESS: ${_pid} attempt=${_try} - detected stderr error"
	    ps -p ${_pid} > /dev/null && kill -SIGKILL ${_pid}
	    sleep 0.5
	    return 1
	}
        # job running for X cycles of sleep.
	if [ "${z}" -gt "${maxsleepcycles}" ]; then
	    echo "FAILED PROCESS: ${_pid} attempt=${_try} - running too long"
	    ps -p ${_pid} > /dev/null && kill -SIGKILL ${_pid}
	    sleep 0.5
	    return 1
	fi
        # is cpu usage zero over the past cycle?
	echo "Check # ${z}: ${report}   date=`date`"
	((z++))
	sleep $slp
    done
    # one last check
    cat ${_log} | egrep -q "${pattern}" && {
	echo "FAILED PROCESS: ${_pid} attempt=${_try} - detected dcache error"
	ps -p ${_pid} > /dev/null && kill -SIGKILL ${_pid}
	sleep 0.5
	return 1
    }
    cat ${_err} | egrep -q "${pattern}" && {
	echo "FAILED PROCESS: ${_pid} attempt=${_try} - detected dcache error"
	ps -p ${_pid} > /dev/null && kill -SIGKILL ${_pid}
	sleep 0.5
	return 1
    }
    echo "END MONITORING pid=${_pid} attempt=${_try}"
    return 0
}

function dump () {
    _log=$1
    _err=$2
    _try=$3
    echo "=========================="
    echo "STDOUT attempt=${_try}:"
    echo "--------------------------"
    cat ${_log} 1>&1 2>&1
    echo "--------------------------"
    echo "=========================="
    echo "STDERR attempt=${_try}:"
    echo "--------------------------"
    cat ${_err} 1>&2 2>&2
    echo "--------------------------"
}

blog=`mktemp`;
log="${blog}.OU"; touch $log
err="${blog}.ER"; touch $err
echo "HOSTNAME:"
hostname
echo "Temp files:"
echo "log = ${log}"
echo "err = ${err}"
echo "Command:"
echo ${PROGRAM}

success=0
for i in `seq 1 ${ntry}`; do
    ${PROGRAM} 1>${log} 2>${err} &
    PID=$!
    disown  # to prevent annoying kill messages
    monitor ${PID} ${log} ${err} ${i}
    MONRES=$?
    if [ "${MONRES}" -eq "0" ]; then
	success=1
	break; 
    fi;
    if [ "${ntry}" -gt "1" ]; then
	dump ${log} ${err} ${i}
    fi
    sleep 0.5
done

if [ "${success}" == "1" ]; then
    dump ${log} ${err} ${i}
    echo "SUCCESS!"
    rm -f ${log} ${err} ${blog}
    exit 0;
else
    dump ${log} ${err} ${i}  # optional
    echo "FAILURE!"
    rm -f ${log} ${err} ${blog}
    exit 1;
fi;

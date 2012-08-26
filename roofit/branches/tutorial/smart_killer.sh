#!/bin/bash

# Smart job runner to catch pnfs crashes
# Instead of running:
#> PROGRAM ARGS
# you can run
#> ./smart_runner.sh PROGRAM ARGS
# It will automatically monitor your program and will restart it
# if it detects an infinite pnfs loop.

PROGRAM=$@
if [ -z "${PROGRAM}" ]; then 
    echo "Usage: $0 commands"
    exit 0
fi;
pattern='Last IO operation timeout'
patternh='HOME directory not set'
patternm='std::bad_alloc'
patternseg='\*\*\* Break \*\*\* segmentation violation'
patternboost='boost::filesystem::create_directory: Permission denied'
slp=10 # how often to check for process status?
ntry=7 # how many times should we try?
maxsleepcycles=150 # (in a row)

function monitor () {
    _pid=$1
    _log=$2
    _try=$3
    echo "BEGIN MONITORING pid=${_pid} attempt=${_try}"
    z=0
    ncycles=0
    while ps -p ${_pid} > /dev/null; do
	report=`ps -p ${_pid} -o cputime=,rss=,vsz=`
	# check for dcache errors
	tail -n2 ${_log} | grep -q "${pattern}" && {
	    echo "FAILED PROCESS: ${_pid} attempt=${_try} - detected dcache error"
	    # try to kill it gently first (to give time to flush stdout)
	    kill -SIGTERM ${_pid}
	    sleep 5
	    # if it is still running, send a SIGKILL
	    ps -p ${_pid} > /dev/null && kill -SIGKILL ${_pid}
	    sleep 0.5
	    return 1
	}
        # job was in suspend for X cycles of sleep. something is wrong. retry.
	if [ "${ncycles}" -gt "${maxsleepcycles}" ]; then
	    echo "FAILED PROCESS: ${_pid} attempt=${_try} - is asleep"
	    # try to kill it gently first (to give time to flush stdout)
	    kill -SIGTERM ${_pid}
	    sleep 5
	    # if it is still running, send a SIGKILL
	    ps -p ${_pid} > /dev/null && kill -SIGKILL ${_pid}
	    sleep 0.5
	    return 1
	fi
        # is cpu usage zero over the past cycle?
	echo "Check # ${z}: ${report}   date=`date`"
	((z++))
	ps -eo stat,pid -p ${_pid} | grep ${_pid} | grep -v 'grep' | grep -q "^S" && {
	    ((ncycles++))
	    echo "INFO: incrementing sleep state counter to ${ncycles}"
	    sleep 0.1
	}
	ps -eo stat,pid -p ${_pid} | grep ${_pid} | grep -v 'grep' | grep -q "R" && {
	    echo "INFO: resetting sleep state counter"
	    ncycles=0
	}
	sleep $slp
    done
    echo "END MONITORING pid=${_pid} attempt=${_try}"
    # check if $HOME was not mounted
    tail -n2 ${_log} | grep -q "${patternh}" && {
	echo "FAILED PROCESS: ${_pid} attempt=${_try}"
	sleep 0.5
	echo "HOME directory error"
	return 1
    }
    # check if the job failed with std::bad_alloc
    tail -n1000 ${_log} | grep -q "${patternm}" && {
	echo "FAILED PROCESS: ${_pid} attempt=${_try}"
	sleep 0.5
	echo "${patternm} error"
	return 3
    }
    # check if the job failed with a segmentation violation
    tail -n2000 ${_log} | grep -q "${patternseg}" && {
	echo "FAILED PROCESS: ${_pid} attempt=${_try}"
	sleep 0.5
	echo "${patternseg} error"
	return 4
    }
    # check if the job failed with a segmentation violation
    tail -n2000 ${_log} | grep -q "${patternboost}" && {
	echo "FAILED PROCESS: ${_pid} attempt=${_try}"
	sleep 0.5
	echo "${patternboost} error"
	return 4
    }
    return 0
}

function dump () {
    _log=$1
    _err=$2
    _try=$3
    #sync # WARNING: sync often crashes NFS server
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
    monitor ${PID} ${err} ${i}
    MONRES=$?
    if [ "${MONRES}" -eq "0" ]; then
	success=1
	break; 
    fi;
    dump ${log} ${err} ${i}
    sleep 0.5
done

if [ "${success}" == "1" ]; then
    dump ${log} ${err} ${i}
    echo "SUCCESS!"
    rm -f ${log} ${err}
    exit 0;
else
    echo "FAILURE!"
    rm -f ${log} ${err}
    exit 1;
fi;

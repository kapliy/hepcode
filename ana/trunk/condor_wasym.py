#!/usr/bin/env python

import sys,os,datetime,getpass,stat,re
import subprocess

ANAMEM_DATA='1900' #2700
ANAMEM_MC='2000'   #3800mb
ANAMEM_Z='2000'    #powheg zmumu
MRGMEM='1900'      #4000mb
NRETRY=7
smart_runner='./smart_runner.sh'
smart_runner='time'

CONDOR="""universe = vanilla

executable = {executable}
output = {stdout}
error = {stderr}
log = {log}

Arguments = None

Requirements   =  ( (Memory >= {memory}) && (HAS_CVMFS =?= TRUE) )

transfer_input_files = /tmp/{user}/TAR/TrigFTKAna.tar.bz2,/home/{user}/.globus/tmp.proxy
#transfer_output_files = {outputs}
should_transfer_files = YES
when_to_transfer_output = ON_EXIT

+AccountingGroup = \"group_uct3.{user}\"

Queue
"""

SH_PRE="""#!/bin/bash

# setup based on: http://twiki.mwt2.org/bin/view/UCTier3/CondorUCT3

date
df -h .
free -m
echo 'START <<<<<<<<<<<<<<<<<<<<<<<'
ROOTDIR=$PWD
LOCDIR=results/ana_wasym
RESHOST=root://uct3-xrd.mwt2.org/
SHRDIR=%s
RESDIR=%s
export HOME=$PWD
echo ROOTDIR ${ROOTDIR}
echo HOSTNAME ${HOSTNAME}
echo USER ${USER}
if [ -f ${ROOTDIR}/tmp.proxy ]; then
    export X509_USER_PROXY=${ROOTDIR}/tmp.proxy
else
    echo ERROR: cannot locate x509 proxy under: ${ROOTDIR}/tmp.proxy
    echo exit 10
    exit 10
fi

if [ $PWD == '/home/antonk' ]; then
    echo ERROR: invoked from home directory. Exiting...
    echo exit 20
    exit 20
fi

# Unpack code tarball
echo 'UNPACK <<<<<<<<<<<<<<<<<<<<<<<'
date
df -h .
if [ -s TrigFTKAna.tar.bz2 ]; then
    tar xfj TrigFTKAna.tar.bz2
    if [ ! -d TrigFTKAna ]; then
	echo ERROR: extracted TrigFTKAna.tar.bz2, but cannot find TrigFTKAna folder
        echo exit 30
	exit 30
    fi
else
    echo ERROR: unable to find TrigFTKAna.tar.bz2 in $PWD
    echo exit 40
    exit 40
fi

# Setup athena environment
echo 'SOURCE <<<<<<<<<<<<<<<<<<<<<<<'
date
df -h .
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh --quiet
#source $AtlasSetup/scripts/asetup.sh 17.6.0.1,64,here
# 11/28/2013: trying to add slc5 to get it to work on slc6 machines
source $AtlasSetup/scripts/asetup.sh 17.6.0.1,64,here,slc5
if [ -z $ROOTSYS ]; then echo 'ERROR: ROOTSYS undefined. Exiting...'; echo exit 50; exit 50; fi
if [ -z $SITEROOT ]; then echo 'ERROR: SITEROOT undefined. Exiting...'; echo exit 60; exit 60; fi
export XrdSecGSISRVNAMES=uct2-s5.uchicago.edu\|uct2-s6.uchicago.edu\|uct2-s20.uchicago.edu
export XrdSecGSISRVNAMES='uct2-*.uchicago.edu'

# Set up TrigFTKAna
echo 'SETUP <<<<<<<<<<<<<<<<<<<<<<<'
date
df -h .
if [ ! -s TrigFTKAna/ana/ana_wasym ]; then echo 'ERROR: cannot find TrigFTKAna/ana/ana_wasym. Exiting...'; echo exit 70; exit 70; fi
export uct3_64=1
export ROOTCORE_NCPUS=24
source ${ROOTDIR}/TrigFTKAna/scripts/dependencies.sh &> /dev/null
export ROOTCOREDIR=${ROOTDIR}/TrigFTKAna/CommonAnalysis/RootCore
export ROOTCOREBIN=${ROOTDIR}/TrigFTKAna/CommonAnalysis/RootCore
$ROOTCOREDIR/scripts/link.sh
echo $ROOTCOREDIR
export PATH=$ROOTCOREBIN/bin:$PATH
export LD_LIBRARY_PATH=$ROOTCOREBIN/lib:$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$ROOTCOREBIN/lib:$DYLD_LIBRARY_PATH
export PYTHONPATH=$ROOTCOREBIN/python:$PYTHONPATH
export LD_LIBRARY_PATH=${ROOTDIR}/TrigFTKAna/LHAPDF-6.1.3/LHAPDF/lib:${LD_LIBRARY_PATH}
#export LD_LIBRARY_PATH=${ROOTDIR}/TrigFTKAna/LIBXML/:${LD_LIBRARY_PATH}

# Execute code with arguments
echo 'EXECUTE <<<<<<<<<<<<<<<<<<<<<<<'
date
df -h .
cd TrigFTKAna
rm -rf results
RMODE=ana
mkdir -p $LOCDIR
echo $@

"""

SH_POST="""
if [ ${status} != '0' ]; then
    echo ERROR: job failed with status: ${status}
    echo exit `expr 100 + ${status}`
    exit `expr 100 + ${status}`
fi

# Copy output with xrdcp
echo 'COPY <<<<<<<<<<<<<<<<<<<<<<<'
date
df -h .
outfiles=`find $LOCDIR -maxdepth 2 -type f -name \*root\* 2>/dev/null`
# check that the output xrootd directory exists
dirchk=`xrd uct3-xrd.mwt2.org existdir ${RESDIR}`
if [ \"$dirchk\" == 'The directory exists.' ]; then
  echo INFO: output directory already exists
else
  xrd uct3-xrd.mwt2.org mkdir ${RESDIR}
  sleep 5
fi
dirchk=`xrd uct3-xrd.mwt2.org existdir ${RESDIR}`
if [ \"$dirchk\" == 'The directory exists.' ]; then
  echo INFO: passed second dir check
else
  echo ERROR: failed to create output directory: ${RESDIR}
  echo $dirchk
  echo exit 80
  exit 80
fi
nout=0
for outfile in ${outfiles}; do
    fbase=`basename ${outfile}`
    # check that the file was closed correctly
    root -b -q ${outfile}  2>&1 | grep Error && {
      echo ERROR: file ${outfile} was not closed correctly
      echo exit 81
      exit 81
    }
    cmd=\"xrdcp -np -v -f $PWD/${outfile} ${RESHOST}/${RESDIR}/${fbase}\"
    echo ${cmd}
    eval ${cmd}
    status=$?
    if [ ${status} != '0' ]; then sleep 5; eval ${cmd}; status=$?; fi
    if [ ${status} != '0' ]; then sleep 10; eval ${cmd}; status=$?; fi
    if [ ${status} != '0' ]; then sleep 30; eval ${cmd}; status=$?; fi
    if [ ${status} != '0' ]; then
	echo ERROR: failed to transfer files: ${cmd}
        echo exit 90
	exit 90
    fi
    # manually check that the file at destination is open-able
    root -b -q ${RESHOST}/${RESDIR}/${fbase} 2>&1 | grep Error && {
      echo ERROR: file ${RESHOST}/${RESDIR}/${fbase} contains error AFTER xrdcp
      xrd uct3-xrd.mwt2.org rm ${RESDIR}/${fbase}
      echo exit 91
      exit 91
    }
    # manually check that we can read ALL bytes from the file at destination
    # note that this check adds about 15 minutes to total run time, and may hang
    if [ ${RMODE} == \"ana\" ]; then
      ./smart_killer.sh hadd -O -f CRAP.root ${RESHOST}/${RESDIR}/${fbase} ${RESHOST}/${RESDIR}/${fbase}
      stmrg=$?
      if [ $stmrg != '0' ]; then
         echo ERROR: file ${RESHOST}/${RESDIR}/${fbase} is corrupted AFTER xrdcp: detected file errors
         rm -f CRAP.root
         xrd uct3-xrd.mwt2.org rm ${RESDIR}/${fbase}
         echo exit 92
         exit 92
      fi
      rm -f CRAP.root
    fi
    ((nout++))
done
if [ ${nout} == '0' ]; then
    echo ERROR: executable finished but failed to produce any ROOT files in the output directory
    echo exit 100
    exit 100
fi

# Clean up worker so no files are transferred back
echo 'CLEAN <<<<<<<<<<<<<<<<<<<<<<<'
date
df -h .
cd ${ROOTDIR}
rm -rf TrigFTKAna

echo 'STOP <<<<<<<<<<<<<<<<<<<<<<<'
ls
date
df -h .
free -m

"""

# write the history file
history_file = open(".ana_history","a")
sep = " "
history_file.write(sep.join(sys.argv)+"\n")
history_file.close()

class Job:
    def __init__(s,jobtype):
        s.id = "" # this is only set upon submission
        s.sleep = 1
        s.jobtype = jobtype
        s.user = getpass.getuser()
    def create(s,tag,workdir,executable,options,submitdir,sample="",nsplits=1,isplit=0,sleep=1,cput=None,mem=None):
        s.workdir = workdir
        s.executable = executable
        s.options = options
        s.submitdir = submitdir
        assert re.search('/share/t3data3/%s/ana/'%(s.user),submitdir)
        s.resdir = submitdir.replace('/share/t3data3/%s/ana/'%(s.user),'/atlas/uct3/data/users/%s/ana/'%(s.user))
        s.fdic = [s.submitdir,s.resdir]
        s.sample = sample
        s.nsplits = nsplits
        s.isplit = isplit
        s.sleep = sleep
        s.cput = cput
        s.mem = mem
        s.tag = tag
        s.outputs = []
    def jobname(s):
        jobname = '%s_%s'%(s.jobtype,s.tag)
        if s.jobtype=='ANA':
            jobname += '_%d'%s.isplit
        return jobname
    def choose_memory(s):
        """ decides on the memory requirement for this job """
        if s.jobtype == "MRG":  # request more memory for merge jobs
            return MRGMEM
        elif re.search('mc_',s.tag):
            if re.search('mc_powheg_pythia_zmumu',s.tag):
                return ANAMEM_Z
            else:
                return ANAMEM_MC
        else:
            assert re.search('data_',s.tag)
            return ANAMEM_DATA
        assert False
        return 2000
    def write_submit_script(s):
        assert s.jobtype, 'ERROR: write_submit_script must run after write_ana_script'
        s.condorfile = os.path.join(s.submitdir, s.jobtype+'_'+s.tag+"_"+str(s.isplit)+".submit")
        s.cdic = cdic = {}
        cdic['user'] = s.user
        cdic['executable'] = s.jobfile
        cdic['stdout'] = s.outOU
        cdic['stderr'] = s.outER
        cdic['log'] = s.outLOG
        cdic['memory'] = s.choose_memory()
        cdic['outputs'] = ','.join(s.outputs)
        f = open(s.condorfile, "w")
        print >>f, CONDOR.format(**cdic)
        f.close()
    def write_ana_script(s):
        s.jobfile = os.path.join(s.submitdir, s.tag+"_"+str(s.isplit)+".sh")
        s.outROOT = 'wasym_'+s.tag+"_"+str(s.isplit)
        s.outROOTpath = os.path.join('results','ana_wasym', s.outROOT)
        s.outOU = os.path.join(s.submitdir, "logs", s.tag+"_"+str(s.isplit)+".OU")
        s.outER = os.path.join(s.submitdir, "logs", s.tag+"_"+str(s.isplit)+".ER")
        s.outLOG = os.path.join(s.submitdir, "logs", s.tag+"_"+str(s.isplit)+".LG")
        s.outputs += ['cached_ignore_filenames_%d.xml'%s.isplit]
        f = open(s.jobfile, "w")
        print >>f, SH_PRE%(s.fdic[0],s.fdic[1])
        print >>f, 'echo ana/%s %s -i %s --force-output-filename %s --nsplits %d --split %d'%(s.executable,s.options,s.sample,s.outROOTpath,s.nsplits,s.isplit)
        print >>f, smart_runner,'ana/%s %s -i %s --force-output-filename %s --nsplits %d --split %d'%(s.executable,s.options,s.sample,s.outROOTpath,s.nsplits,s.isplit)
        print >>f, "status=$?"
        print >>f, """
        fc=cached_ignore_filenames.xml
        if [ -f ${fc} ]; then
           nbad=`grep -c '<item>' $fc`
           if [ $nbad != '0' ]; then
                echo ERROR: detected $nbad failures in cache file:
                cat ${fc}
                rm -f ${fc}
                echo exit 200
                exit 200
           else
                mv ${fc} ${ROOTDIR}/cached_ignore_filenames_%d.xml
           fi
        else
           echo ERROR: TrigFTKAna failed to produce ${fc}
           echo exit 201
           exit 201
        fi
        """%(s.isplit)
        print >>f, SH_POST
        f.close()
        os.system('chmod +x %s'%s.jobfile)
        s.write_submit_script()
        return True
    def write_merge_script(s,inputs=[]):
        """ A merge script either for main files, or for unfolding """
        assert len(inputs)>0
        # hadd determines if we are merging main histograms file, or unfolding files
        hadd = True if s.jobtype == "MRG" else False
        s.jobfile = os.path.join(s.submitdir, 'merge_wasym.sh' if hadd else 'munfold_wasym.sh')
        s.outROOT = ('root_' if hadd else 'unfold_')+s.tag+".root"
        s.outROOTpath = os.path.join('results','ana_wasym',s.outROOT)
        pre = 'merge' if hadd else 'munfold'
        s.outOU = os.path.join(s.submitdir, pre+'_wasym.out.log')
        s.outER = os.path.join(s.submitdir, pre+'_wasym.err.log')
        s.outLOG = os.path.join(s.submitdir, pre+'_wasym.log.log')
        flist = 'wasym.root.list' if hadd else 'wasym.unfold.list'
        s.outputs += [flist]
        f = open(s.jobfile, "w")
        print >>f, SH_PRE%(s.fdic[0],s.fdic[1])
        print >>f,'RMODE=merge'
        print >>f,'nexpected=%d'%len(inputs)
        print >>f,'ntot=0'
        print >>f,'rm -f ${ROOTDIR}/%s ; touch ${ROOTDIR}/%s;'%(flist,flist)
        for fin in inputs:
            fname = fin if hadd else '%s.unfold'%fin
            print >>f,'f="${RESDIR}/%s.root"'%fname
            print >>f,'st=`xrd uct3-xrd.mwt2.org existfile $f`'
            print >>f,'if [ "$st" == "The file exists." ]; then'
            # xrootd files: reduce cache size, since hadd is stupid and will eat 100% of RAM
            print >>f,'echo ${RESHOST}/$f?cachesz=1000000 >> ${ROOTDIR}/%s'%flist
            print >>f,'((ntot++))'
            print >>f,'else'
            print >>f,'echo ERROR: failed to locate file $f'
            print >>f,'fi'
        print >>f,'if [ "$ntot" -eq "$nexpected" ]; then echo "ALL DONE"; else echo "ERROR: missing `expr $nexpected - $ntot` files"; echo exit 202; exit 202; fi'
        print >>f,'if [ "$ntot" -eq "0" ]; then echo "ERROR: no files to merge"; echo exit 203; exit 203; fi'
        print >>f,"""
# a special version of hadd that adds files in chunks of 20
function hadd2() {
    local per
    per=30 #20
    fin=$1
    opts=$2
    fout=$3
    shift
    n=`cat $fin | wc -l`
    ngrp=`expr $n / $per`
    nrem=`expr $n % $per`
    if [ \"$nrem\" == \"0\" ]; then ngrp=`expr $ngrp - 1`; fi
    for igrp in `seq 0 $ngrp`; do
	imin=`expr $per \* $igrp`
	imax=`expr $per \* $igrp + $per`
	if [ \"$imax\" -gt \"$n\" ]; then imax=`expr $per \* $igrp + $nrem`; fi
	# offset by 1
	imin=`expr $imin + 1`
	imax=`expr $imax`
	idel=`expr $imax - $imin + 1`
	echo \"===== Part $igrp / $ngrp : $imin to $imax\"
	echo hadd ${opts} \"${fout}.TMPHADD_${igrp}.root\" `cat $fin | head -n $imax | tail -n $idel`
	hadd ${opts} \"${fout}.TMPHADD_${igrp}.root\" `cat $fin | head -n $imax | tail -n $idel`
	st=$?
	if [ \"$st\" != \"0\" ]; then
	    echo \"ERROR: merge step $igrp failed. Bailing out...\"
	    return $st
	fi
    done
    # remove opts to speed up the last step and prevent creation of additional ntuple cycles;2
    echo hadd ${fout} ${fout}.TMPHADD_*root*
    hadd ${fout} ${fout}.TMPHADD_*root*
    st=$?
    rm -f ${fout}.TMPHADD_*root*
    return $st
}
        """
        if False:
            if hadd:
                print >>f, 'echo hadd -O %s `cat ${ROOTDIR}/%s`'%(s.outROOTpath,flist)
                print >>f, 'hadd -O %s `cat ${ROOTDIR}/%s`'%(s.outROOTpath,flist)
            else:
                print >>f, 'echo hadd -T %s `cat ${ROOTDIR}/%s`'%(s.outROOTpath,flist)
                print >>f, 'hadd -T %s `cat ${ROOTDIR}/%s`'%(s.outROOTpath,flist)
        else:
            print >>f, 'hadd2 ${ROOTDIR}/%s "%s" %s'%(flist,"-O" if hadd else "-T",s.outROOTpath)
        print >>f, "status=$?"
        print >>f, SH_POST
        f.close()
        os.system('chmod +x %s'%s.jobfile)
        s.write_submit_script()
        return True
    def write_unfold_script(s):
        """ merges RooUnfold files """
        s.jobfile = os.path.join(s.submitdir, 'merge_unfold.sh')
        return True

class JobSet:
    """ A collection of jobs - a unit of dependency generation """
    def __init__(s):
        s.jobs = [] # these are the actual job objects
        s.deps = [] # list of other JobSets that this JobSet depends on
    def add(s, job):
        s.jobs.append(job)
    def add_dep(s, otherset):
        s.deps.append( otherset )
    def get_deps(s):
        res = []
        for dep in s.deps:
            res += dep.jobs
        return res
    def root_outputs(s):
        """ Returns a list of ROOT files expected to be created by this JobSet"""
        out = []
        for j in s.jobs:
            out.append(j.outROOT)
        return out
    def write_dag_script(s):
        """
        master submit script for condor DAG
        Job  Setup job.setup.submit
        Job  Work1 job.work1.submit
        Job  Work2 job.work2.submit
        Job  Final job.finalize.submit
        PARENT Setup CHILD Work1 Work2
        PARENT Work1 Work2 CHILD Final
        Retry Work1 8
        """
        assert len(s.jobs) in (1,2),'ERROR: write_dag_script should be called from the final merge JobSet'
        s.dag = os.path.join( s.jobs[0].submitdir, 'global.dag')
        f = open(s.dag,'w')
        # condor submit scripts
        for dep in s.get_deps():
            print >>f,'Job   %s %s'%(dep.jobname(),dep.condorfile)
        for job in s.jobs:
            print >>f,'Job   %s %s'%(job.jobname(),job.condorfile)
        # retry instructions
        for dep in s.get_deps():
            print >>f,'Retry   %s %s'%(dep.jobname(),NRETRY)
        for job in s.jobs:
            print >>f,'Retry   %s %s'%(job.jobname(),NRETRY)
        a_parent = ' '.join( [ dep.jobname() for dep in s.get_deps() ] )
        for job in s.jobs:
            a_child = job.jobname()
            print >>f,'PARENT %s CHILD %s'%(a_parent,a_child)
        f.close()
    def submit(s):
        """ DAG submission """
        s.write_dag_script()
        submitdir = s.jobs[0].submitdir
        assert os.path.isdir(submitdir)
        os.chdir(submitdir)
        pipe=os.popen('condor_submit_dag -outfile_dir . %s'%s.dag, "r")
        for line in pipe.readlines():
            id = line.strip()
            print id
        return 0

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-s","--sample",dest="sample",
                  default = None,
                  type="string",help="Data input sample")
parser.add_option("-f","--force", default=False,
                  action="store_true",dest="force",
                  help="If set to true, allows submission into existing directories")
parser.add_option("-o", "--output",dest="output",
                  default = None,
                  type="string",help="Output directory")
parser.add_option("-t", "--tag",dest="tag",
                  default = "tag",
                  type="string",help="Tag name")
parser.add_option("-n","--nsplits",dest="nsplits",
                  default = 1,
                  type="int",help="Number of splits")
parser.add_option("-e","--executable",dest="executable",
                  default = 'ana_wasym',
                  type="string",help="Name of executable under ana/")
parser.add_option("--workdir",dest="workdir",
                  #default = os.getcwd(),
                  default = "",
                  type="string",help="Working directory for the job")
parser.add_option("--cput",dest="cput",
                  default = "33:59:59",
                  type="string",help="Total and CPU time string for PBS")
parser.add_option("--mem",dest="mem",
                  default = "2500mb",
                  type="string",help="Total and CPU time string for PBS")
parser.add_option("--options",dest="options",
                  default = "",
                  type="string",help="Options that are passed directly to the executable")

if __name__ == '__main__':
    (opts, args) = parser.parse_args()
    assert opts.sample,'--sample is required'
    assert opts.output,'--output is required'
    assert opts.tag,'--tag is required'

    # this is a stupid workaround -- 
    # os.getcwd() when called from /share/ftkdata on uct3-s1 returns the full /export/t3data/... path tht is 
    # not availible on the working nodes. So, instead of setting workdir by default using os.getcwd(), we 
    # just have to call `pwd` and read the output:
    if opts.workdir == "":
        pipe=os.popen("pwd","r")
        for line in pipe.readlines():
            opts.workdir = line.strip()
    date = datetime.date.today().isoformat().replace('-','')
    submitdir = os.path.join(opts.output,opts.tag)
    if os.path.exists(submitdir):
        print 'WARNING:',submitdir,'already exists'
        if not opts.force:
            print 'Maybe you want to remove it?'
            print '     rm -r',submitdir
            sys.exit(1)
    else:
        os.makedirs(submitdir)
    logdir = os.path.join(submitdir,'logs')
    if not os.path.exists(logdir):
        os.makedirs(logdir);

    # create ana jobs
    anaopts = ""
    anaopts += " " + opts.options
    anaset = JobSet()
    for isplit in range(opts.nsplits):
        j = Job('ANA')
        j.create(opts.tag, opts.workdir, opts.executable, anaopts, submitdir, opts.sample, opts.nsplits, isplit, 0, opts.cput,opts.mem)
        j.write_ana_script()
        anaset.add( j )

    # create merge job
    mergeset = JobSet()
    mergejob = Job('MRG')
    mergejob.create(opts.tag, opts.workdir, opts.executable, anaopts, submitdir, opts.sample, 1, 0, 1, opts.cput, opts.mem)
    mergejob.write_merge_script(anaset.root_outputs())
    mergeset.add( mergejob )
    mergeset.add_dep( anaset )

    # create unfolding-merge job - only if this is signal MC
    if re.search('wminmunu',opts.tag) or re.search('wplusmunu',opts.tag):
        unfoldjob = Job('UNF')
        unfoldjob.create(opts.tag, opts.workdir, opts.executable, anaopts, submitdir, opts.sample, 1, 0, 1, opts.cput, opts.mem)
        unfoldjob.write_merge_script(anaset.root_outputs())
        mergeset.add(unfoldjob)

    mergeset.submit()

#!/usr/bin/env python

import sys,os,datetime,getpass,stat,re
import subprocess

ANAMEM_DATA='2700'
ANAMEM_MC='3800' #3800mb
ANAMEM_Z='4100' #powheg zmumu
MRGMEM='4000' #4000mb
smart_runner='./smart_runner.sh'
DGUNION_Z=' -u 1'
DGUNION=' -u 5'

CONDOR="""universe = vanilla

executable = {executable}
output = {stdout}
error = {stderr}
log = {log}

Arguments = None

Requirements   = Memory >= {memory}

should_transfer_files = YES
transfer_input_files = /tmp/antonk/TAR/TrigFTKAna.tar.bz2
when_to_transfer_output = ON_EXIT
transfer_output_files = {outputs}

+AccountingGroup = \"group_uct3.antonk\"

"""

SH_PRE="""#!/bin/bash

date
echo 'START <<<<<<<<<<<<<<<<<<<<<<<'
ROOTDIR=$PWD
LOCDIR=results/ana_wasym
RESHOST=root://uct3-xrd.mwt2.org/
SHRDIR=%s
RESDIR=%s
echo ROOTDIR ${ROOTDIR}
echo HOSTNAME ${HOSTNAME}

if [ $PWD == '/home/antonk' ]; then
    echo ERROR: invoked from home directory. Exiting...
fi

# Unpack code tarball
echo 'UNPACK <<<<<<<<<<<<<<<<<<<<<<<'
date
if [ -s TrigFTKAna.tar.bz2 ]; then
    tar xfj TrigFTKAna.tar.bz2
    if [ ! -d TrigFTKAna ]; then
	echo ERROR: extracted TrigFTKAna.tar.bz2, but cannot find TrigFTKAna folder
	exit 1
    fi
else
    echo ERROR: unable to find TrigFTKAna.tar.bz2 in $PWD
    exit 1
fi

# Setup athena environment
echo 'SOURCE <<<<<<<<<<<<<<<<<<<<<<<'
date
export XrdSecGSISRVNAMES=uct2-s5.uchicago.edu\|uct2-s6.uchicago.edu\|uct2-s20.uchicago.edu
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh --quiet
asetup 17.6.0.1,64,here
if [ -z $ROOTSYS ]; then echo 'ERROR: ROOTSYS undefined. Exiting...'; exit 2; fi
if [ -z $SITEROOT ]; then echo 'ERROR: SITEROOT undefined. Exiting...'; exit 2; fi

# Set up TrigFTKAna
echo 'SETUP <<<<<<<<<<<<<<<<<<<<<<<'
date
if [ ! -s TrigFTKAna/ana/ana_wasym ]; then echo 'ERROR: cannot find TrigFTKAna/ana/ana_wasym. Exiting...'; exit 3; fi
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
export LD_LIBRARY_PATH=${ROOTDIR}/TrigFTKAna/lhapdf-5.8.8/LHAPDF/lib:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${ROOTDIR}/TrigFTKAna/LIBXML/:${LD_LIBRARY_PATH}

# Execute code with arguments
echo 'EXECUTE <<<<<<<<<<<<<<<<<<<<<<<'
date
cd TrigFTKAna
rm -rf results
echo $@

"""

SH_POST="""
status=$?
if [ ${status} != '0' ]; then
    echo ERROR: job failed with status: ${status}
    exit ${status}
fi

# Copy output with xrdcp
echo 'COPY <<<<<<<<<<<<<<<<<<<<<<<'
date
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
  exit 4
fi
nout=0
for outfile in ${outfiles}; do
    fbase=`basename ${outfile}`
    cmd=\"xrdcp -f $PWD/${outfile} ${RESHOST}/${RESDIR}/${fbase}\"
    echo ${cmd}
    eval ${cmd}
    status=$?
    if [ ${status} != '0' ]; then sleep 5; eval ${cmd}; status=$?; fi
    if [ ${status} != '0' ]; then sleep 10; eval ${cmd}; status=$?; fi
    if [ ${status} != '0' ]; then sleep 30; eval ${cmd}; status=$?; fi
    if [ ${status} != '0' ]; then
	echo ERROR: failed to transfer files: ${cmd}
	exit 5
    fi
    ((nout++))
done
if [ ${nout} == '0' ]; then
    echo ERROR: executable finished but failed to produce any ROOT files in the output directory
    exit 6
fi

# Clean up worker so no files are transferred back
echo 'CLEAN <<<<<<<<<<<<<<<<<<<<<<<'
date
cd ${ROOTDIR}
rm -rf TrigFTKAna

echo 'STOP <<<<<<<<<<<<<<<<<<<<<<<'
ls
date

"""

# write the history file
history_file = open(".ana_history","a")
sep = " "
history_file.write(sep.join(sys.argv)+"\n")
history_file.close()

class Job:
    def __init__(s):
        s.id = "" # this is only set upon submission
        s.sleep = 1
        s.jobtype = None
    def create(s,tag,workdir,executable,options,submitdir,sample="",nsplits=1,isplit=0,sleep=1,cput=None,mem=None):
        s.workdir = workdir
        s.executable = executable
        s.options = options
        s.submitdir = submitdir
        assert re.search('/share/t3data3/antonk/ana/',submitdir)
        s.resdir = submitdir.replace('/share/t3data3/antonk/ana/','/atlas/uct3/data/users/antonk/ana/')
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
        s.condorfile = os.path.join(s.submitdir, s.tag+"_"+str(isplit)+".submit")
        s.cdic = cdic = {}
        cdic['executable'] = s.jobfile
        cdic['stdout'] = s.outOU
        cdic['stderr'] = s.outER
        cdic['log'] = s.outLOG
        cdic['memory'] = s.choose_memory()
        cdic['outputs'] = ' '.join(s.outputs)
        f = open(s.condorfile, "w")
        print >>f, CONDOR.format(**cdic)
        f.close()
    def write_ana_script(s):
        s.jobtype = "ANA"
        s.jobfile = os.path.join(s.submitdir, s.tag+"_"+str(isplit)+".sh")
        s.outDG = 'wasym_'+s.tag+"_"+str(isplit)
        s.outDGpath = os.path.join('results','ana_wasym', s.outDG)
        s.outOU = os.path.join(s.submitdir, "logs", s.tag+"_"+str(isplit)+".OU")
        s.outER = os.path.join(s.submitdir, "logs", s.tag+"_"+str(isplit)+".ER")
        s.outLOG = os.path.join(s.submitdir, "logs", s.tag+"_"+str(isplit)+".LG")
        f = open(s.jobfile, "w")
        print >>f, SH_PRE%(s.fdic[0],s.fdic[1])
        print >>f, smart_runner,'ana/%s %s -i %s --force-output-filename %s --nsplits %d --split %d'%(s.executable,s.options,s.sample,s.outDGpath,s.nsplits,s.isplit)
        print >>f, SH_POST
        f.close()
        os.system('chmod +x %s'%s.jobfile)
        s.write_submit_script()
        return True
    def write_dgmerge_script(s,inputs=[]):
        assert len(inputs)>0
        s.jobtype = "MRG"
        s.jobfile = os.path.join(s.submitdir, 'merge_wasym.sh')
        s.outDG = s.tag+".root"
        s.outDGpath = os.path.join('results','ana_wasym',s.outDG)
        s.outROOT = 'root_'+s.tag+".root"
        s.outROOTpath = os.path.join('results','ana_wasym',s.outROOT)
        s.outOU = os.path.join(s.submitdir, 'merge_wasym.out.log')
        s.outER = os.path.join(s.submitdir, 'merge_wasym.err.log')
        s.outLOG = os.path.join(s.submitdir, 'merge_wasym.log.log')
        flist = 'wasym.root.list'
        s.outputs += 'TrigFTKAna/'+flist
        f = open(s.jobfile, "w")
        print >>f, SH_PRE%(s.fdic[0],s.fdic[1])
        print >>f,'nexpected=%d'%len(inputs)
        print >>f,'ntot=0'
        print >>f,'rm -f %s ; touch %s;'%(flist,flist)
        for fin in inputs:
            print >>f,'f="%s.root"'%fin
            print >>f,'st=`xrd uct3-xrd.mwt2.org existfile $f`'
            print >>f,'if [ "$st" == "The file exists." ]; then'
            print >>f,'echo $f >> %s'%flist
            print >>f,'((ntot++))'
            print >>f,'else'
            print >>f,'echo ERROR: failed to locate file $f'
            print >>f,'fi'
        print >>f,'if [ "$ntot" -eq "$nexpected" ]; then echo "ALL DONE"; else echo "Missing `expr $nexpected - $ntot` files"; fi'
        DGU = DGUNION_Z if (re.search('mc_powheg_pythia_zmumu',s.tag) or re.search('mc_powheg_pythia_wminmunu',s.tag) or re.search('mc_powheg_pythia_wplusmunu',s.tag)) else DGUNION 
        print >>f, 'plot/dgadd -f 1 %s -d %s -l %s -r %s'%(DGU,s.outDGpath,flist,s.outROOTpath)
        print >>f, SH_POST
        f.close()
        os.system('chmod +x %s'%s.jobfile)
        s.write_submit_script()
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
    def dg_outputs(s):
        """ Returns a list of ROOT files expected to be created by this JobSet"""
        out = []
        for j in s.jobs:
            out.append(j.outDG)
        return out
    def write_dag_script(s):
        """ TODO: make it work with recursive dependencies 
        master submit script for condor DAG
        Job  Setup job.setup.submit
        Job  Work1 job.work1.submit
        Job  Work2 job.work2.submit
        Job  Final job.finalize.submit
        PARENT Setup CHILD Work1 Work2
        PARENT Work1 Work2 CHILD Final
        """
        assert len(s.jobs)==1,'ERROR: write_dag_script should be called from the final merge JobSet'
        job = s.jobs[0]
        s.dag = os.path.join( job.submitdir, 'global.dag')
        f = open(s.dag,'w')
        for dep in s.get_deps():
            print >>f,'Job   %s %s'%(dep.jobname(),dep.condorfile)
        print >>f,'Job   %s %s'%(job.jobname(),job.condorfile)
        a_parent = ' '.join( [ dep.jobname() for dep in s.get_deps() ] )
        a_child = job.jobname()
        print >>f,'PARENT %s CHILD %s'%(a_parent,a_child)
        f.close()
    def submit(s):
        """ DAG submission """
        s.write_dag_script()
        pipe=os.popen('echo condor_submit_dag -outfile_dir . %s'%s.dag, "r")
        for line in pipe.readlines():
            id = line.strip()
            print id
        return 0

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-s","--sample",dest="sample",
                  default = None,
                  type="string",help="Data input sample")
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
parser.add_option("--root",action="store_true",dest="root",
                  default=False,help="Produce ROOT output instead of dgplot output")



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
    user = getpass.getuser()
    date = datetime.date.today().isoformat().replace('-','')
    submitdir = os.path.join(opts.output,opts.tag)
    if os.path.exists(submitdir):
        print submitdir,'already exists'
        print 'Maybe you want to remove it?'
        print '     rm -r',submitdir
        sys.exit(1)
    else:
        os.makedirs(submitdir)
    logdir = os.path.join(submitdir,'logs')
    if not os.path.exists(logdir):
        os.makedirs(logdir);

    # create ana jobs
    anaopts = "--noDG"
    if not opts.root: anaopts = "--noROOT"
    anaopts += " " + opts.options
    anaset = JobSet()
    for isplit in range(opts.nsplits):
        j = Job()
        j.create(opts.tag, opts.workdir, opts.executable, anaopts, submitdir, opts.sample, opts.nsplits, isplit, 20+isplit, opts.cput,opts.mem)
        j.write_ana_script()
        anaset.add( j )

    # create merge job
    mergeset = JobSet()
    mergejob = Job()
    mergejob.create(opts.tag, opts.workdir, opts.executable, anaopts, submitdir, opts.sample, 1, 0, 1, opts.cput, opts.mem)
    if not opts.root: mergejob.write_dgmerge_script(anaset.dg_outputs())
    else: mergejob.write_rootmerge_script()
    mergeset.add( mergejob )
    mergeset.add_dep( anaset )

    mergeset.submit()

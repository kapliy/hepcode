#!/usr/bin/env python

import sys,os,datetime,getpass,stat,re
import subprocess


# VARIABLES TO PERSONALIZE
# ==================================
script_setup="""#!/bin/bash
# setup begin
hostname
source /home/$USER/.bashrc
cd /home/antonk/TrigFTKAna/
anaquick5
# source SETUP.sh  # only source if running from TrigFTKAna_trunk
# X509 proxy is needed to access MWT2 LOCALGROUPDISK from tier3 nodes
export X509_USER_PROXY=/home/antonk/.globus/tmp.proxy
# setup end
"""
ANAMEM_DATA='2700mb'
ANAMEM_MC='3800mb' #3800mb
ANAMEM_Z='4100mb' #powheg zmumu
MRGMEM='4000mb' #4000mb
smart_runner='./smart_runner.sh'
DGUNION_Z=' -u 1'
DGUNION=' -u 5'
# ==================================


# write the history file
history_file = open(".ana_history","a")
sep = " "
history_file.write(sep.join(sys.argv)+"\n")
history_file.close()


class Job:
    def __init__(s):
        s.id = "" # this is only set upon submission
        s.sleep = 1
        s.jobtype = "ANA"
    def create(s,tag,workdir,executable,options,submitdir,sample="",nsplits=1,isplit=0,sleep=1,cput=None,mem=None):
        s.workdir = workdir
        s.executable = executable
        s.options = options
        s.submitdir = submitdir
        s.sample = sample
        s.nsplits = nsplits
        s.isplit = isplit
        s.sleep = sleep
        s.cput = cput
        s.mem = mem
        s.tag = tag
    def write_ana_script(s):
        s.jobtype = "ANA"
        s.submitfile = os.path.join(s.submitdir, s.tag+"_"+str(isplit)+".sh")
        s.outROOT = os.path.join(s.submitdir, 'wasym_'+s.tag+"_"+str(isplit))
        s.outOU = os.path.join(s.submitdir, "logs", s.tag+"_"+str(isplit)+".OU")
        s.outER = os.path.join(s.submitdir, "logs", s.tag+"_"+str(isplit)+".ER")
        f = open(s.submitfile, "w")
        print >>f, script_setup
        print >>f, 'ls -ltra /share/t3data3/antonk/'
        print >>f, 'sleep',s.sleep
        print >>f, 'ls -ltra /share/t3data3/antonk/'
        print >>f,'cd',s.workdir
        print >>f,'pwd'
        print >>f,"echo ROOTCOREDIR ${ROOTCOREDIR}"
        #print >>f,"ldd ana/%s"%s.executable
        print >>f,'date'
        print >>f, smart_runner,'ana/%s %s -i %s --force-output-filename %s --nsplits %d --split %d'%(s.executable,s.options,s.sample,s.outROOT,s.nsplits,s.isplit)
        print >>f,'date'
        f.close()
        os.system('chmod +x %s'%s.submitfile)
        return True
    def write_dgmerge_script(s,inputs=[]):
        assert len(inputs)>0
        s.jobtype = "MRG"
        s.submitfile = os.path.join(s.submitdir, 'merge_wasym.sh')
        s.outDG = os.path.join(s.submitdir, s.tag+".root")
        s.outROOT = os.path.join(s.submitdir, 'root_'+s.tag+".root")
        s.outOU = os.path.join(s.submitdir, 'merge_wasym.out.log')
        s.outER = os.path.join(s.submitdir, 'merge_wasym.err.log')
        flist = os.path.join(s.submitdir,'wasym.root.list')
        f = open(s.submitfile, "w")
        print >>f,script_setup
        print >>f,'cd',s.submitdir
        print >>f,'ntot=0'
        print >>f,'nexpected=%d'%len(inputs)
        print >>f,'rm -f %s ; touch %s;'%(flist,flist)
        for fin in inputs:
            print >>f,'f="%s.root"'%fin
            print >>f,'if [ -s "%s.4.root" ]; then f="%s.4.root";'%(fin,fin)
            print >>f,'elif [ -s "%s.3.root" ]; then f="%s.3.root";'%(fin,fin)
            print >>f,'elif [ -s "%s.2.root" ]; then f="%s.2.root";'%(fin,fin)
            print >>f,'elif [ -s "%s.1.root" ]; then f="%s.1.root"; fi'%(fin,fin)
            print >>f, 'if [ -s "$f" ]; then echo $f >> %s; ((ntot++)); else echo "WARNING: missing $f" >&2 ; fi'%(flist)
        print >>f,'if [ "$ntot" -eq "$nexpected" ]; then echo "ALL DONE"; else echo "Missing `expr $nexpected - $ntot` files"; fi'
        print >>f,'cd',s.workdir
        print >>f,'pwd'
        print >>f,'date'
        DGU = DGUNION_Z if (re.search('mc_powheg_pythia_zmumu',s.tag) or re.search('mc_powheg_pythia_wminmunu',s.tag) or re.search('mc_powheg_pythia_wplusmunu',s.tag)) else DGUNION 
        print >>f, 'plot/dgadd -f 1 %s -d %s -l %s/wasym.root.list -r %s'%(DGU,s.outDG,s.submitdir,s.outROOT)
        print >>f,'date'
        f.close()
        os.system('chmod +x %s'%s.submitfile)
        return True
    def submit(s, deplist=[]):
        depstr = ""
        if len(deplist) > 0:
            sep = ":"
            depstr = sep.join(deplist)
            depstr = " -W depend=afterany:"+depstr
        lstr = ""
        if s.cput!=None or s.mem!=None:
            lstr = " -l "
            lopts=[]
            if s.cput!=None:
                lopts.append('cput=%s'%s.cput)
                lopts.append('walltime=%s'%s.cput)
            if s.mem!=None:
                if s.jobtype == "MRG":  # request more memory for merge jobs
                    lopts.append('mem=%s'%MRGMEM)
                elif re.search('mc_',s.tag):
                    if re.search('mc_powheg_pythia_zmumu',s.tag):
                        lopts.append('mem=%s'%ANAMEM_Z)
                    else:
                        lopts.append('mem=%s'%ANAMEM_MC)
                else:
                    assert re.search('data_',s.tag)
                    lopts.append('mem=%s'%ANAMEM_DATA)
            lstr += ','.join(lopts)
        jobname = '%s_%s'%(s.jobtype,s.tag)
        if s.jobtype=='ANA':
            jobname += '_%d'%s.isplit
        submitstr = "qsub" + depstr + lstr + " -N %s"%(jobname) + " -w " + s.workdir + " -o " + s.outOU + " -e " + s.outER + " " + s.submitfile
        #print submitstr
        id=""
        def do_submit():
            id = ""
            pipe=os.popen(submitstr, "r")
            for line in pipe.readlines():
                id = line.strip()
            return id
        id = do_submit()
        if id=="":
            print 'WARNING: qsub failed! Will try two more times...'
            sys.stdout.flush()
            time.sleep(3)
            id = do_submit()
            if id=="":
                print 'WARNING: qsub failed! Will try one LAST time...'
                sys.stdout.flush()
                time.sleep(10)
                id = do_submit()
        s.id = id
        assert s.id!=""
        print s.id
        return s.id

class JobSet:
    """ A collection of jobs - a unit of dependency generation """
    def __init__(s):
        s.jobs = [] # these are the actual job objects
        s.jobids = [] # this is only set after submission
        s.deps = [] # list of other JobSets that this JobSet depends on
    def add(s, job):
        s.jobs.append(job)
    def add_dep(s, otherset):
        s.deps.append( otherset )
    def root_outputs(s):
        """ Returns a list of ROOT files expected to be created by this JobSet"""
        out = []
        for j in s.jobs:
            out.append(j.outROOT)
        return out
    def submit(s):
        deplist = []
        for ijob,d in enumerate(s.deps):
            d.submit()
            for i in d.jobids: deplist.append(i)
            if ijob%10==0: time.sleep(0.2)
        for j in s.jobs:
            if len(deplist)>0:
                print 'Dependencies: ',len(deplist)
            j.submit( deplist )
            s.jobids.append( j.id )


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
    #sys.exit(0)
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
        #j.write_test_script()
        anaset.add( j )

    # create merge job
    import time
    time.sleep(0.2)
    mergeset = JobSet()
    mergejob = Job()
    mergejob.create(opts.tag, opts.workdir, opts.executable, anaopts, submitdir, opts.sample, 1, 0, 1, opts.cput, opts.mem)
    if not opts.root: mergejob.write_dgmerge_script(anaset.root_outputs())
    else: mergejob.write_rootmerge_script()
    mergeset.add( mergejob )
    mergeset.add_dep( anaset )

    mergeset.submit()

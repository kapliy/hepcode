#!/usr/bin/env python

import sys,os,datetime,getpass
import subprocess


# VARIABLES TO PERSONALIZE
# ==================================
script_setup="""
#!/bin/bash
# setup begin
hostname
source /home/jwebster/.bashrc
anaquick
cd /share/ftkdata1/jwebster/TrigFTKAna/CommonAnalysis
. setup.sh
# setup end
"""
cdf_username = "jswebster"
email = "jwebste2@gmail.com"
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
    def create(s,tag,workdir,executable,options,submitdir,sample="",nsplits=1,isplit=0,sleep=0,cput=None,mem=None):
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
    def write_test_script(s):
        s.submitfile = os.path.join(s.submitdir, "pbsjob_"+s.tag+"_TEST_"+str(isplit)+".sh")
        s.outOU = os.path.join(s.submitdir, "logs", s.tag+"_TEST_"+str(isplit)+".OU")
        s.outER = os.path.join(s.submitdir, "logs", s.tag+"_TEST_"+str(isplit)+".ER")
        tmpfile = open(s.submitfile, "w")
        out = """%s\n sleep %d\n echo DONE
              """ % (script_setup, s.sleep)
        tmpfile.writelines(out)
        tmpfile.close()
        return out
    def write_ana_script(s):
        s.submitfile = os.path.join(s.submitdir, "pbsjob_"+s.tag+"_ANA_"+str(isplit)+".sh")
        s.outROOT = os.path.join(s.submitdir, s.tag+"_ANA_"+str(isplit))
        s.outOU = os.path.join(s.submitdir, "logs", s.tag+"_ANA_"+str(isplit)+".OU")
        s.outER = os.path.join(s.submitdir, "logs", s.tag+"_ANA_"+str(isplit)+".ER")
        tmpfile = open(s.submitfile, "w")
        #out = """%s\n sleep %d\n cd %s\n ./smart_runner.sh ana/%s %s -i %s --force-output-filename %s --nsplits %d --split %d
        out = """%s\n sleep %d\n cd %s\n ana/%s %s -i %s --force-output-filename %s --nsplits %d --split %d
              """ % (script_setup, s.sleep, s.workdir, s.executable, s.options, s.sample, s.outROOT, s.nsplits, s.isplit)
        tmpfile.writelines(out)
        tmpfile.close()
        return out
    def write_rootmerge_script(s):
        s.submitfile = os.path.join(s.submitdir, "pbsjob_"+s.tag+"_ROOTMERGE.sh")
        s.outROOT = os.path.join(s.submitdir, "merged.root")
        s.outOU = os.path.join(s.submitdir, "logs", s.tag+"_ROOTMERGE.OU")
        s.outER = os.path.join(s.submitdir, "logs", s.tag+"_ROOTMERGE.ER")
        tmpfile = open(s.submitfile, "w")
        out = """%s\n sleep %d\n cd %s\n . scripts/smart_root_merge.sh %s
              """ % (script_setup, s.sleep, s.workdir, s.submitdir)
        tmpfile.writelines(out)
        tmpfile.write( "\nchmod 777 merged.root\n" )
        tmpfile.close()
        return out
    def write_dgmerge_script(s):
        s.submitfile = os.path.join(s.submitdir, "pbsjob_"+s.tag+"_DGMERGE.sh")
        s.outROOT = os.path.join(s.submitdir, "merged.root")
        s.outOU = os.path.join(s.submitdir, "logs", s.tag+"_DGMERGE.OU")
        s.outER = os.path.join(s.submitdir, "logs", s.tag+"_DGMERGE.ER")
        tmpfile = open(s.submitfile, "w")
        out = """%s\n sleep %d\n cd %s\n ls ANA_*.root > DGMERGE_list.txt\n cd %s\n dgplot -m %s -q -l %s/DGMERGE_list.txt
              """ % (script_setup, s.sleep, s.submitdir, s.workdir, s.outROOT, s.submitdir)
        tmpfile.writelines(out)
        tmpfile.close()
        return out
    def write_notify_script(s):
        s.submitfile = os.path.join(s.submitdir, "pbsjob_"+s.tag+"_NOTIFY.sh")
        s.outOU = os.path.join(s.submitdir, "logs", s.tag+"_NOTIFY.OU")
        s.outER = os.path.join(s.submitdir, "logs", s.tag+"_NOTIFY.ER")
        tmpfile = open(s.submitfile, "w")
        out = """%s\n sleep %d\n cd %s\n nfiles=`ls *.root | wc -l`\n echo -e \"%s job complete\\nsuccessfully produced $nfiles out of %i root files\" | ssh %s@cdf.uchicago.edu \"mail -s \\\"%s job complete\\\" %s\"
              """ % (script_setup, s.sleep, s.submitdir, s.tag, s.nsplits+1, cdf_username, s.executable, email)
        tmpfile.writelines(out)
        tmpfile.close()
        return out
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
            if s.cput!=None: lopts.append('cput=%s'%s.cput)
            if s.mem!=None: lopts.append('mem=%s'%s.mem)
            lstr += ','.join(lopts)
        submitstr = "qsub" + depstr + lstr + " -w " + s.workdir + " -o " + s.outOU + " -e " + s.outER + " " + s.submitfile
        if 'MERGE' in s.submitfile:
            print submitstr
        elif 'ANA' in s.submitfile:
            print 'A',
        elif 'NOTIFY' in s.submitfile:
            print 'N',
        else:
            print '?',
        pipe=os.popen(submitstr, "r")
        for line in pipe.readlines():
            s.id = line.strip()
        # print s.id

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
    def submit(s):
        deplist = []
        for d in s.deps:
            d.submit()
            for i in d.jobids: deplist.append(i)
        for j in s.jobs:
            # print deplist
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
                  default = 'ana_wzjets',
                  type="string",help="Name of executable under ana/")
parser.add_option("--workdir",dest="workdir",
                  #default = os.getcwd(),
                  default = "",
                  type="string",help="Working directory for the job")
parser.add_option("--cput",dest="cput",
                  default = "23:59:59",
                  type="string",help="Total and CPU time string for PBS")
parser.add_option("--mem",dest="mem",
                  default = "2500mb",
                  type="string",help="Total and CPU time string for PBS")
parser.add_option("--options",dest="options",
                  default = "",
                  type="string",help="Options that are passed directly to the executable")
parser.add_option("--notify",action="store_true",dest="notify",
                  default=False,help="Send out notification once job completes")
parser.add_option("--dg",action="store_true",dest="dg",
                  default=False,help="Produce dgplot output instead of ROOT output")



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
        print 'Working directory set to',opts.workdir
    
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
    if opts.dg: anaopts = "--noROOT"
    anaset = JobSet()
    for isplit in range(opts.nsplits):
        j = Job()
        j.create(opts.tag, opts.workdir, opts.executable, anaopts, submitdir, opts.sample, opts.nsplits, isplit, 15, opts.cput, opts.mem)
        j.write_ana_script()
        #j.write_test_script()
        anaset.add( j )

    # create merge job
    mergeset = JobSet()
    mergejob = Job()
    mergejob.create(opts.tag, opts.workdir, opts.executable, anaopts, submitdir, opts.sample, 1, 0, 5, opts.cput, opts.mem)
    if opts.dg: mergejob.write_dgmerge_script()
    else: mergejob.write_rootmerge_script()
    mergeset.add( mergejob )
    mergeset.add_dep( anaset )

    if not opts.notify:
        mergeset.submit()
    else:
        # create notification job
        notifyset = JobSet()
        notifyjob = Job()
        notifyjob.create(opts.tag, opts.workdir, opts.executable, anaopts, submitdir, opts.sample, opts.nsplits, 0, 5, opts.cput, opts.mem)
        notifyjob.write_notify_script()
        notifyset.add( notifyjob )
        notifyset.add_dep( mergeset )
        #notifyset.add_dep( anaset )
        notifyset.submit()


    
    

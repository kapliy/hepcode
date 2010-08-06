#!/usr/bin/env python
import sys,re
import xml.dom.minidom

class dom_job:
    """ infiles[inds]=[file1,file2...]
        outfiles = [file1,file2...]
        prepend  - list of (option,value) prepended to output file name
        forward  - list of (option,value) forwarded to the grid job
        command  - script that will be executed on the grid
    """
    def __init__(s,domjob,requiredDS=None):
        """ Loads <job></job> from xml file.
        If requiredDS is set, makes sure it is present in job spec """
        s.infiles = {}
        s.outfiles = []
        s.prepend  = []
        s.forward = []
        s.command = domjob.command
        # input files
        for inds in domjob.getElementsByTagName('inds'):
            name = dom_parser.text(inds.getElementsByTagName('name')[0])
            files = inds.getElementsByTagName('file')
            if len(files)==0:
                continue
            s.infiles[name]=[]
            for file in files:
                s.infiles[name].append(dom_parser.text(file))
        if requiredDS and requiredDS not in s.infiles.keys():
            print 'ERROR: inDS=%s is not used in every job specified in xml file'%requiredDS
            sys.exit(0)
        # output files
        [s.outfiles.append(dom_parser.text(v)) for v in domjob.getElementsByTagName('output')]
        # gearing options
        def true(v):
            return v in ('true', '1', 'True', 'TRUE', 'yes')
        for o in domjob.getElementsByTagName('option'):
            name=o.attributes['name'].value
            value=dom_parser.text(o)
            prepend=true(o.attributes['prepend'].value)
            forward=true(o.attributes['forward'].value)
            if prepend:
                s.prepend.append((name,value))
            if forward:
                s.forward.append((name,value))
        # script executed on the grid node for this job
        if domjob.getElementsByTagName('command'):
            s.command = dom_parser.text(domjob.getElementsByTagName('command')[0])
    def files_in_DS(s,DS):
        """ Returns a list of files used in a given job in a given dataset"""
        return s.infiles[DS] if DS in s.infiles else []
    def outputs(s):
        """ Comma-separated list of output files accepted by prun """
        return ','.join(s.outfiles)
    def forward_opts(s):
        """ passable string of forward options """
        return ' '.join( ['%s=%s'%(v[0],v[1]) for v in s.forward])
    def prepend_string(s):
        """ a tag string prepended to output files """
        return '_'.join( ['%s%s'%(v[0],v[1]) for v in s.prepend])
    def exec_string(s):
        """ exec string for prun """
        return '%s %s'%(s.command,s.forward_opts())

class dom_parser:
    def __init__(s,fname):
        """ creates a dom object out of a text file """
        s.dom = xml.dom.minidom.parse(fname)
        s.fname = fname
        s.parse()
        s.check()
        s.separate_inDS = False
    @staticmethod
    def text(pnode):
        """ extracts the value stored in the node """
        rc = []
        for node in pnode.childNodes:
            if node.nodeType == node.TEXT_NODE:
                rc.append(node.data)
        return ''.join(rc)
    def parse(s):
        """ loads submission configuration from an xml file """
        try:
            s.title = dom_parser.text(s.dom.getElementsByTagName('title')[0])
            s.tag = dom_parser.text(s.dom.getElementsByTagName('tag')[0])
            s.command = dom_parser.text(s.dom.getElementsByTagName('command')[0])
            # see if one of the input datasets was explicitly labeled as inDS
            s.requiredDS = None
            if len(s.dom.getElementsByTagName('inDS'))>0 and dom_parser.text(s.dom.getElementsByTagName('inDS')[0])!='':
                s.requiredDS = dom_parser.text(s.dom.getElementsByTagName('inDS')[0])
            s.outds = dom_parser.text(s.dom.getElementsByTagName('outds')[0].getElementsByTagName('name')[0])
            s.jobs = []
            for job in s.dom.getElementsByTagName('job'):
                job.command = s.command      # default command if the job doesn't specify its own
                s.jobs.append(dom_job(job,requiredDS=s.requiredDS))
        except:
            print 'ERROR: failed to parse',s.fname
            sys.exit(0)
    def check(s):
        """ checks that all output files have unique qualifiers """
        quals=[]
        for j in s.jobs:
            for o in j.outfiles:
                quals.append('%s_%s'%(j.prepend_string(),o))
        if len(list(set(quals))) != len(quals):
            print 'ERROR: found non-unique output file names across the jobs'
            print '(you likely need to review xml options with prepend=true)'
            sys.exit(0)
    def input_datasets(s):
        """ returns a list of all used input datasets """
        DSs = set()
        for j in s.jobs:
            for ds in j.infiles.keys():
                DSs.add(ds)
        return list(DSs)
    def outDS(s):
        """ output dataset """
        return s.outds
    def inDS(s):
        """ chooses a dataset we'll call inDS; others will become secondaryDS """
        # user manually labeled one of datasets as inDS:
        if s.requiredDS:
            return s.requiredDS
        # choose inDS dataset randomly
        else:
            return s.input_datasets()[0]
    def secondaryDSs(s):
        """ returns all secondaryDSs. This excludes inDS, unless inDS is managed by prun"""
        return [d for d in s.input_datasets() if d!=s.inDS() or s.separate_inDS==True ]
    def secondaryDSs_config(s):
        """ returns secondaryDSs string in prun format:
        StreamName:nFilesPerJob:DatasetName[:MatchingPattern[:nSkipFiles]]
        nFilesPerJob is set to zero, so that it is updated later from actual file count.
        MatchingPattern is an OR-separated list of actual file names.
        """
        out = []
        DSs = s.secondaryDSs()
        for i,DS in enumerate(DSs):
            out.append('IN%d:0:%s:%s'%(i+1,DS,s.files_in_DS(DS,regex=True)))
        return ','.join(out)
    def files_in_DS(s,DS,regex=False):
        """ Returns a list of all files from a given dataset
            that will be used in at least one job in this submission
            If regex==True, the list is converted to a regex string
        """
        assert DS in s.input_datasets(),'ERROR: dataset %s was not requested in the xml file'%DS
        files = []
        for j in s.jobs:
            if DS in j.infiles.keys():
                files+=j.infiles[DS]
        if regex:
            return '|'.join(sorted(list(set(files))))
        else:
            return sorted(list(set(files)))
    def nFiles_in_DS(s,DS):
        return len(s.files_in_DS(DS))
    def nJobs(s):
        return len(s.jobs)
    def dump(s,verbose=True):
        """ prints a summary of this submission """
        def P(key,value=''):
            if value=='':
                print key
            else:
                print (key+':').ljust(14),
                print value
        P('XML FILE LOADED',s.fname)
        P('Title',s.title)
        P('Command',s.command)
        P('InDS',s.inDS())
        P('Output DS',s.outds)
        P('njobs',s.nJobs())
        if verbose:
            for i,job in enumerate(s.jobs):
                P('===============> JOB%d'%i)
                P('command',job.exec_string())
                P('outfiles',job.prepend_string()+'^ '+' '.join(job.outfiles))
                P('INPUTS:')
                j=0
                for dsname,files in job.infiles.iteritems():
                    P('  Dataset%d'%j,dsname)
                    for k,fname in enumerate(files):
                        P('     File%d'%k,fname)
                    j+=1

if __name__=="__main__":
    p = dom_parser('./job.xml')
    p.dump()
    print p.input_datasets()

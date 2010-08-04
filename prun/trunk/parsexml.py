#!/usr/bin/env python
import xml.dom.minidom

class dom_job:
    """ infiles[inds]=[file1,file2...]
        outfiles = [file1,file2...]
        prepend  - list of (option,value) prepended to output file name
        forward  - list of (option,value) forwarded to the grid job
        command  - script that will be executed on the grid
    """
    def __init__(s,domjob):
        s.infiles = {}
        s.outfiles = []
        s.prepend  = []
        s.forward = []
        s.command = domjob.command
        # input files
        for inds in domjob.getElementsByTagName('inds'):
            name = dom_parser.text(inds.getElementsByTagName('name')[0])
            s.infiles[name]=[]
            files = inds.getElementsByTagName('file')
            for file in files:
                s.infiles[name].append(dom_parser.text(file))
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
    def forward_opts(s):
        """ passable string of forward options """
        return ' '.join( ['%s=%s'%(v[0],v[1]) for v in s.forward])
    def prepend_string(s):
        """ a tag string prepended to output files """
        return '_'.join( ['%s%s'%(v[0],v[1]) for v in s.prepend])
    
class dom_parser:
    def __init__(s,fname):
        """ creates a dom object out of a text file """
        s.dom = xml.dom.minidom.parse(fname)
        s.fname = fname
        s.parse()
    @staticmethod
    def text(pnode):
        """ extracts the value stored in the node """
        rc = []
        for node in pnode.childNodes:
            if node.nodeType == node.TEXT_NODE:
                rc.append(node.data)
        return ''.join(rc)
    def parse(s):
        try:
            s.title = dom_parser.text(s.dom.getElementsByTagName('title')[0])
            s.tag = dom_parser.text(s.dom.getElementsByTagName('tag')[0])
            s.command = dom_parser.text(s.dom.getElementsByTagName('command')[0])
            s.outds = dom_parser.text(s.dom.getElementsByTagName('outds')[0].getElementsByTagName('name')[0])
            s.jobs = []
            for job in s.dom.getElementsByTagName('job'):
                job.command = s.command      # default command if the job doesn't specify its own
                s.jobs.append(dom_job(job))
        except:
            import sys
            print 'ERROR: failed to parse',s.fname
            sys.exit(0)
    def dump(s):
        def P(key,value=''):
            if value=='':
                print key
            else:
                print (key+':').ljust(14),
                print value
        P('Title',s.title)
        P('Command',s.command)
        P('Output DS',s.outds)
        for i,job in enumerate(s.jobs):
            P('===============> JOB%d'%i)
            P('command','%s %s'%(job.command,job.forward_opts()))
            P('prepend',job.prepend_string())
            P('outfiles',' '.join(job.outfiles))
            P('INPUTS:')
            j=0
            for dsname,files in job.infiles.iteritems():
                P('  Dataset%d'%j,dsname)
                for k,fname in enumerate(files):
                    P('     File%d'%k,fname)
                j+=1
            
p = dom_parser('./job.xml')
p.dump()

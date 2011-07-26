#!/usr/bin/env python

""" 
A simple database module that allows concurrent read/write access
through the use of file locking.
All python data is stored in a picked dictionary.
All ROOT data is stored in ROOT file with a similar directory structure.

Folder structure: /TAG/{CMB,ID,EXMS}/{REG}/{ksp,ksf,chip,chif; npos,nneg ; plots/ }
"""

import os,sys,pickle,re
from FileLock import FileLock

class antondb:
    """ DB manager """
    def __init__(s,fbase):
        """ contstructor """
        s.fbase = fbase
        s.fname = fbase + '.db'   # antondb
        s.rname = fbase + '.root' # ROOT TFile
        s.data = {}
    def load(s):
        """ non-locking """
        f = open(s.fname,'r')
        s.data = pickle.load(f)
        f.close()
    def ls(s,path='/'):
        """ trivial ls() """
        keys = s.data.keys()
        dirs = {}
        lev = len(path.split('/'))
        for k in keys:
            jn='/'.join(k.split('/')[:lev])
            if not re.match(path,jn): continue
            dirs['/'.join(k.split('/')[:lev])] = 1
        for lj in sorted(dirs.keys()):
            print lj
            if lj==path and path in s.data and isinstance(s.data[path],dict):
                for ikey in sorted(s.data[path].keys()):
                    print '-->',ikey
    def add(s,path,addition):
        """ locking write """
        with FileLock(s.fname):
            if os.path.isfile(s.fname):
                f = open(s.fname,'r')
                s.data = pickle.load(f)
                f.close()
            # update the provenance
            if path not in s.data:
                s.data[path] = {}
            s.data[path].update(addition)
            # save via pickle
            f = open(s.fname,'w')
            pickle.dump(s.data,f)
            f.close()
    def add_root(s,path,objects):
        """ adds a bunch of TObjects to a directory """
        import ROOT
        with FileLock(s.rname):
            f = ROOT.TFile.Open(s.rname,'UPDATE')
            sdirs = path.split('/')
            dnext = ROOT.gDirectory
            for dir in sdirs:
                if dir=='': continue
                if dnext.GetDirectory(dir):
                    dnext.cd(dir)
                    dnext = ROOT.gDirectory
                else:
                    dnext = dnext.mkdir(dir)
                    ROOT.gDirectory.cd(dir)
                assert dnext, 'Failed to create directory %s'%dir
            assert ROOT.gDirectory.GetName()==sdirs[-1]
            [o.SetTitle("") for o in objects]
            [o.Write(o.GetName(),ROOT.TObject.kOverwrite) for o in objects]
            f.Close()
            
if __name__ == "__main__":
    fname = 'test'
    a = antondb(fname)
    v = {'chip' : 2, 'chif' : 0.22}
    a.add('/DEFAULT/CMB/BB/R0',v)
    a.add_root('/DEFAULT/CMB/BB/R0',[None])
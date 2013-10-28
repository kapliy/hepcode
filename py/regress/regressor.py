#!/usr/bin/env python

"""
Requirements: Python 2.7, numpy;    [optional]: scipy, pylab/matplotlib
Requirements: file 'test.csv' should be in CWD (a different name can be specified via --input fname.csv)
Usage: 'python regressor.py' - demonstrates histogramming and regression functionality.
       'python -i regressor.py --plot [--pq]' - makes some scatter plots with regression lines

- CollegeGPA (a number, ranging from 0 to 4.0)
- HighSchoolGPA (ranging from 0 to 4.0)
- SATScores (reflecting only two sections of the test, ranging from 0 to 1600)
- LetterQuality (ranging from 0 to 10)
"""

import numpy as np

_HAVE_STATS = False
try:
    from scipy import stats
    _HAVE_STATS = True
except ImportError:
    print 'Bummer, no t-statistics...'
    pass

_HAVE_PYLAB = False
try:
    import pylab
    _HAVE_PYLAB = True
except ImportError:
    print 'Bummer, no plotting...'
    pass

class Table(object):
    """ A generic class that represents a multi-column table of numerical data.
    Supports common data manipulations, such as histogramming, plotting, and regressing.
    In case of errors, exceptions are thrown and should be caught by the caller.
    """
    def __init__(s,verbose=False):
        """ Dummy constructor to define a table """
        s.verbose = verbose
        s.data = None   # structured numpy ndarray containing full table data
        s.names = None  # alias to column names (already stored in ndarray.dtype.names)
    def dump(s,nmax=10):
        """ Dumps a few points on screen """
        print 'COLUMN NAMES:',s.names
        if len(s.data)<nmax*2+1:
            print s.data
        else:
            print s.data[:nmax]
            print '...'
            print s.data[-nmax:]
    def load_file_txt(s,dsource,dtype=float,delimiter=','):
        """ Loads the table data from a text file
        Input: dsource - a filename
        Input: [dtype=float] - type of numbers beyond first row
        Input: [delimiter=','] - separator between fields
        """
        s.data = np.genfromtxt(dsource,dtype=dtype,delimiter=delimiter,invalid_raise=True,names=True)
        s.names = s.data.dtype.names   # alias column names for quicker access
        if s.verbose:
            print 'INFO: loaded %d %d-dimensional points with column names:'%(len(s.data),len(s.names)),', '.join(s.names)
    def apply_bounds(s,bdic={}):
        """ Removed points from the dataset that fail the given bounds. In-place.
        Input: bdic - dictionary of bounds in the form of: { 'ColName' : (min,max) }
        Note: bounds are inclusive, i.e., points right at the bound will pass
        """
        d,nd = s.data,len(s.data)  # alias
        chosen_points = np.ones(nd,dtype=bool)  #initially, all points pass
        for name,rg in bdic.iteritems():
            assert name in s.names, 'ERROR: apply_bounds failed because column %s does not exist in data'%name
            # construct boolean ndarray that tells which points pass this column's bounds, and intersect with previous columns
            chosen_points = np.logical_and( chosen_points, np.logical_and(rg[0]<=d[name] , d[name]<=rg[1]) )
        if sum(chosen_points)!=nd: # if anything should be removed
            if s.verbose:
                print 'INFO: list of points outside of range bounds:'
                for point in d[ np.logical_not(chosen_points) ]:
                    print '      --> ',point
            s.data = d[ chosen_points ]
            print 'INFO: apply_bounds removed %d/%d points from the data, leaving %d points'%(nd-len(s.data),nd,len(s.data))
    def remove_outliers(s,cols,ndev=5.0):
        """ Remove outliers from the dataset. In-place.
        Input: cols     - list of column names on which to filter.
        Input: [ndev=3] - number of standard deviations away from the mean to be called an outlier
        Outliers are defined by the number of std away from the mean.
        If multiple cols are given, an event that is an outlier in AT LEAST one of them will be dropped
        """
        d,nd = s.data,len(s.data)  # alias
        chosen_points = np.ones(nd,dtype=bool)
        for name in cols:
            assert name in s.names, 'ERROR: remove_outliers failed because column %s does not exist in data'%name
            colmean,colstd = np.mean(d[name]),np.std(d[name])
            # construct boolean ndarray that tells which points are outliers in this column, and intersect with previous columns
            chosen_points = np.logical_and( chosen_points, abs(d[name]-colmean) < ndev*colstd )
        if sum(chosen_points)!=nd: # if anything should be removed
            if s.verbose:
                print 'INFO: list of points deemed to be outliers (more than %.1f sigma away):'%ndev
                for point in d[ np.logical_not(chosen_points) ]:
                    print '      --> ',point
            s.data = d[ chosen_points ]
            print 'INFO: remove_outliers removed %d/%d points from the data, leaving %d points'%(nd-len(s.data),nd,len(s.data))
    def data_from_nth_quartile(s,col,NQ):
        """ Returns a subset of the data that belonds to NQ'th quartile when sorted according to column col
        NQ must be one of: 1,2,3,4
        """
        d = s.data # alias
        assert NQ in (1,2,3,4),'ERROR: nth_quartile NQ argument out of bounds'
        assert col in s.names,'ERROR: nth_quartile: unknown column %s'%col
        chosen_points = np.logical_and( d[col]>=np.percentile(d[col],(NQ-1)*25) , d[col]<=np.percentile(d[col],NQ*25) )
        data = s.data[chosen_points]
        if s.verbose:
            print 'INFO: nth_quartile selected %d/%d points in %d-quartile along column %s'%(len(data),len(s.data),NQ,col)
        return data
    def histo(s,cols=None,nbins=None,algo='FreedmanDiaconis'):
        """ Bins the data into 1d histogram(s) along each requested column
        Input: [cols=None] - a string or list-of-strings providing column names to be histogrammed
                             if cols is None, all available columns will be histogrammed independently
        Input: [bins=None] - a scalar or list of len(bins)==len(cols), specifying binning for each column
                             if None, an algorithm is used to determine optimal binning automatically
                             None can be a scalar, or among the members of the bins list
        Input: [algo]      - if bins==None, chooses the algorithm to use to automatically bin the data
                             options are: FreedmanDiaconis,Scott,Sqrt    [TODO: add more choices]
        Output: [ (bounds1,counts1), (bounds2,counts2), ... ], where each tuple corresponds to one of cols.
        """
        if cols==None: cols = s.names[:]
        if type(cols)==type(""): cols = [cols,]  # allow strings
        for col in cols:
            assert col in s.names,'ERROR: column %s does not exist in data'%col
        # choose the auto-binning algorithm, if needed
        # I'm being lazy here and just putting the algorithms as local functions
        def nbin_FreedmanDiaconis(x):
            """ Binning via Freedman-Diaconis algorithm """
            N = len(x)
            if N<2: return 1
            IQR = np.percentile(x,75) - np.percentile(x,25) # inter-quartile percentile
            bwidth = 2 * IQR / N**(1.0/3.0)
            return int(np.ceil(  (max(x)-min(x))/bwidth  ))
        def nbin_Scott(x):
            """ Binning via Scott rule """
            N = len(x)
            if N<2: return 1
            sigma = np.std(x)
            bwidth =  3.5*sigma / N**(1.0/3.0)
            return int(np.ceil(  (max(x)-min(x))/bwidth  ))
        def nbin_Sqrt(x):
            """ Binning via square-root rule (e.g., Excel-style) """
            N = len(x)
            if N<2: return 1
            return int(np.ceil(np.sqrt(N)))
        nbin_map = {'FreedmanDiaconis':nbin_FreedmanDiaconis,'Scott':nbin_Scott,'Sqrt':nbin_Sqrt}
        assert algo in nbin_map,'ERROR: unknown auto-binning alorithm'
        nbin_fun = nbin_map[algo]
        # prepare nbins for each column
        if hasattr(nbins, '__iter__'):   # explicit binning for each col
            assert len(nbins)==len(cols)
            for icol,nbin in enumerate(nbins):  # but check if any of them are None
                if nbin==None:
                    nbins[icol] = nbin_fun(s.data[cols[icol]])
        elif nbins==None:                # all None - we need to auto-determine bins
            nbins = [ nbin_fun(s.data[col]) for col in cols ]
        else:                            # integer (common binning for all cols)
            assert nbins,'ERROR: nbins should be an iterable or None'
            nbins = [nbins]*len(cols)
        if s.verbose:
            print 'INFO: histo() determined the following optimal number of bins:'
            for col,nbin in zip(cols,nbins):
                print '      --> ',col,nbin
        # prepare bin bounds for each column (needed for output)
        BOUNDS = []
        for col,nbin in zip(cols,nbins):
            bmin = min(s.data[col])
            bmax = max(s.data[col])
            if bmin==bmax:  # in case the data has zero spread
                bmin-=0.5
                bmax+=0.5
            bdel = (bmax-bmin)/nbin
            BOUNDS.append( [bmin+bdel*i for i in range(0,nbin+1)] )
        # prepare bin counts for each column (needed for output)
        HISTS = []
        for col,nbin,bound in zip(cols,nbins,BOUNDS):
            HIST = np.zeros(nbin,dtype=int)
            for v in s.data[col]:
                # in case of overflows, limit the maximum ibin to (nbin-1)
                ibin = min( np.floor( (v-bound[0])/(bound[1]-bound[0]) ) , nbin-1 )
                if ibin<0: ibin=0   # underflow goes into the first bin
                HIST[ibin] += 1
            HISTS.append(HIST)
        # pair up bin bounds and bin counts for each column's histogram
        return [ (x,y) for x,y in zip(BOUNDS,HISTS) ]
    def regress(s,yname=None,xnames=None,xcheck=False,quartile=None):
        """ Linear regression of one column against one or more columns 
        Input: [yname]    - name of dependent column. by default, 1st column is used
        Input: [xnames]   - string or list of independent columns. by default, all-but-1st are used
        Input: [xcheck]   - boolean flag that will run a standard scipy regression for comparison
        Input: [quartile] - if None, regress the whole dataset.
                            if one of [1,2,3,4], regress only corresponding quartile in yname column
        Limitations: assumes the regression matrix is invertible; throws otherwise
                     assumes local minimum in sum-of-square-errors (doesn't check at boundary)
        B = (X'X)^-1 X' Y, where X' refers to transpose
        Reference: http://cran.r-project.org/doc/contrib/Faraway-PRA.pdf
        """
        # if dependent variable is not given, assume it's the first one
        if yname==None:
            yname = s.names[0]
        # if xnames is a scalar, convert it to a single-element list
        if not hasattr(xnames, '__iter__'):
            if xnames==None:          # if None, assume we want to regress against all columns
                xnames = [name for name in s.names if name!=yname]
            else:                     # or: just take an explicit xname
                xnames = [xnames,]
        assert yname in s.names,'ERROR: unknown yname: %s'%yname
        for xname in xnames:
            assert xname in s.names,'ERROR: unknown xname: %s'%xname
        # choose subset of data to regress (all, or a particular quartile in yname)
        data = s.data
        if quartile!=None:
            assert quartile in (1,2,3,4),'ERROR: data quartile must be between 1-4'
            data = s.data_from_nth_quartile(yname,quartile)
        # dependent variable
        Y = data[yname]
        # number of points
        N = len(Y)
        # number of parameters
        K = len(xnames)+1
        # degrees of freedom
        ndof = N - K
        assert ndof>0,'ERROR: number of points must exceed number of parameters'
        # independent variables, plus np.ones for the offset parameter
        X = np.vstack( [ data[name] for name in xnames ] + [ np.ones(N) ]).T
        # Because inverse may not exist, protect it in a try block: (X'X)^(-1)
        XtXi = None
        try:
            XtXi = np.linalg.inv(np.dot(X.T,X))
        except:
            print 'ERROR: regression failed, cannot invert the matrix (insufficient rank)'
            raise
        # ok, work through the linear algebra
        XtY = np.dot(X.T,Y)
        B = np.dot(XtXi,XtY)                 # regression coefficients
        Yh = np.dot(X,B)                     # fitted Y's
        H = np.dot(X, np.dot(XtXi,X.T))      # hat matrix: X(X'X)^-1X'
        Ei = np.dot( (np.eye(N)-H) , Y )     # residuals (Y-Yh)
        SSR = np.dot(Ei.T,Ei)                # residuals sum-of-squares
        S2 = SSR / ndof                      # standard error squared: sigma^2
        covB = XtXi * S2                     # covariance matrix: (XtX)^-1 * SSR/ndof
        sB = np.sqrt(np.diag(covB))          # error on B: sqrt of diag elms of cov matrix
        sigB = B/sB                          # significance on each B parameter
        SST = sum((Y-np.mean(Y))**2)         # total variation = sum[(Y-Y_mean)^2]
        R2 = 1.0-SSR/SST if SST!=0 else 0    # R^2
        R2a = 1-(N-1.0)/(N-K)*(1-R2)         # adjusted-R^2
        # compute 2-sided pval using student-T distro
        pval = 2*stats.t.sf(np.abs(sigB),ndof)
        if s.verbose:
            header = '===== REGRESSION: '+yname+' vs '+','.join(xnames)+' ====='
            print '='*len(header)
            print header
            if quartile!=None:
                QMAP = {1:'1st',2:'2nd',3:'3rd',4:'4th'}
                print '      NOTE: only regressing %s quartile of %s'%(QMAP[quartile],yname)
            print '='*len(header)
            print 'N/K/NDF     ',N,K,ndof
            print 'R^2:        ','%.3f'%R2
            print 'adj. R^2:   ','%.3f'%R2a
            print 'std err:    ','%.3f'%np.sqrt(S2)
            print '-'*len(header)
            print 'coef. list: ',','.join(xnames)+',ConstTerm'
            print 'coef. (B):  ',B
            print 'error (sB): ',sB
            print 'B/sB:       ',sigB
            print 'pval(B=0):  ',pval
            print '-'*len(header)
        # cross-check via scipy
        if xcheck and len(xnames)==1:
            if _HAVE_STATS:
                print 'scipy cross-check: (slope,intercept,R^2,p-val,stderr)'
                slope, intercept, r_value, p_value, std_err =  stats.linregress(data[xnames[0]],data[yname])
                print '            ',slope, intercept, r_value**2, p_value, std_err
        return B,sB,R2,R2a,pval
    def predict(s,reg,indep,check=None):
        """ Predicts dependent variable from independent variable(s) for a given point
        Input: reg     - regression tuple, i.e. the output of Table::regress
        Input: indep   - a list of independent variables at a given point
        Input: [check] - actual value of dendent variable (to be compared with regressed value)
        Output: predicted dependent variable
        """
        C = reg[0]  # coefficients
        x = indep[:]+[1.0]  # add the constant term
        y = np.dot(C,x)
        if s.verbose:
            extra=''
            if check:
                extra=' expect=%.2f   diff=%.3f'%(check,y-check)
            print 'PREDICT: x=%s'%['%.2f'%v for v in indep],  '\t->  y=%.2f%s'%(y,extra)
        return y
    def plot_xy(s,xname,yname,regress=False,quart=False):
        """ Scatter plot of two columns: yname vs xname
        Input: [regress]   - overlay a numpy regression curve on top?
        Input: [quart]     - plot separately for each yname quartile?
        Note: each invocation creates a new figure
        """
        if not _HAVE_PYLAB: return
        assert xname in s.names,'ERROR: column %s does not exist'%xname
        assert yname in s.names,'ERROR: column %s does not exist'%yname
        pylab.figure()
        datas = [ s.data, ]
        if quart:
            datas = []
            datas = [ s.data_from_nth_quartile(yname,QT) for QT in (1,2,3,4) ]
            pylab.subplot(220+1)
        for iplt,data in enumerate(datas):
            if quart:
                pylab.subplot(220+iplt+1)
            x,y = data[xname], data[yname]
            pylab.plot(x, y, 'o', label='Original data', markersize=5)
            if regress:  # using numpy machinery
                A = np.vstack([x, np.ones(len(data))]).T
                m, c = np.linalg.lstsq(A, y)[0]
                pylab.plot(x, m*x + c, 'r', label='Fitted line')
            pylab.xlabel(xname)
            pylab.ylabel(yname)
            pylab.title('%s vs %s'%(yname,xname) if not quart else 'Quartile %d'%(iplt+1))
        pylab.tight_layout() 
        pylab.show(block=False)

if __name__ == '__main__':
    # a small option parser to control various things
    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option("-i", "--input",dest="input",
                      type="string", default='test.csv',
                      help="Input file name in comma-separated format")
    parser.add_option("-b", "--bounds",dest="bounds",
                      type="string", default='CollegeGPA:0.0:4.0,HighSchoolGPA:0.0:4.0,SATTotal:0:1600,LetterQuality:0:10',
                      help="Range bounds on each column formatted via: ColName1:min1:max1,ColName2:min2:max2")
    parser.add_option("--outliercols",dest="outliercols",
                      type="string", default='CollegeGPA,HighSchoolGPA,SATTotal,LetterQuality',
                      help="Remove outliers in given comma-separated columns, which are stdoutlier deviations away from the mean")
    parser.add_option("--outlierstd",dest="outlierstd",
                      type="float", default=5.0,
                      help="Number of standard deviations away from the mean to be called an outlier")
    parser.add_option("-q","--quiet", default=False,
                      action="store_true",dest="quiet",
                      help="Controls printing of verbose information")
    parser.add_option("-p","--plot", default=False,
                      action="store_true",dest="plot",
                      help="Do we want to make the plots? Needs Xserver to bring up the figures")
    parser.add_option("--pq", default=False,
                      action="store_true",dest="plotquartile",
                      help="Do we want to make the plots in each quartile?")
    (opts, args) = parser.parse_args()
    # (1) load the data
    a = Table(verbose = not opts.quiet)
    try:
        a.load_file_txt(opts.input)
    except:
        print 'ERROR: failed to parse the input from file %s.\nStack trace below:'%opts.input
        raise
    # (2) clean up the data by applying range bounds
    try:
        bdic = {}   # parse out --bounds string
        for col in opts.bounds.split(','):
            celms = col.split(':')
            assert len(celms)==3,'ERROR: --bounds string not formatted correctly'
            cname,cmin,cmax = celms[0],float(celms[1]),float(celms[2])
            bdic[cname] = ( float(cmin), float(cmax) )
        a.apply_bounds(bdic)
    except:
        print 'ERROR: failed to parse and apply a constraint string [%s].\nStack trace below:'%opts.bounds
        raise
    # (3) further clean up the data by removing outliers
    # PS: for the assigned problem, this doesn't do anything because outlierstd is chosen at 5-sigma
    try:
        ocols = opts.outliercols.split(',')  # list of column names to use for outlier search
        a.remove_outliers(ocols,opts.outlierstd)
    except:
        print 'ERROR: failed to remove outliers.\nStack trace below:'
        raise
    # (4) demonstrate the histogram-maker functionality
    #     we exercise auto-binning ability for GPA and SAT scores.
    #     however, for LetterQuality, we manually provide binning
    #     because it is hard to auto-bin extremely discrete data
    icol=0
    for x,y in a.histo(cols=None,nbins=[None,None,None,8]):
        print 'Histo[%s]:\t'%a.names[icol],'[%.1f]\t->'%(x[0]),y,'\t<-[%.1f]'%(x[-1])
        icol+=1
    # (5) demonstrate regression for various combinations of independent variables
    R1 = a.regress('CollegeGPA','HighSchoolGPA',xcheck=True)
    R2 = a.regress('CollegeGPA','SATTotal',xcheck=True)
    R3 = a.regress('CollegeGPA','LetterQuality',xcheck=True)
    R12 = a.regress('CollegeGPA',['HighSchoolGPA','SATTotal'])
    R13 = a.regress('CollegeGPA',['HighSchoolGPA','LetterQuality'])
    R23 = a.regress('CollegeGPA',['SATTotal','LetterQuality'])
    Rall = a.regress()
    # also re-run regressions in each CollegeGPA quartile
    RallQ = []
    for quartile in (1,2,3,4):
        RallQ.append( a.regress(quartile=quartile) )
    # (6) example of using regression results to make a prediction
    print
    print 'EXAMPLES OF USING REGRESSION RESULTS TO MAKE PREDICTIONS:'
    if len(a.data)>10:
        for ipt in range(10):
            a.predict(Rall,list(a.data[ipt])[1:],list(a.data[ipt])[0])
    print
    # (7) make some 1d plots - enable via --plot, also consider '--pq' flag to plot in quartiles
    if opts.plot:
        a.plot_xy('HighSchoolGPA','CollegeGPA',regress=True,quart=opts.plotquartile)
        a.plot_xy('SATTotal','CollegeGPA',regress=True,quart=opts.plotquartile)
        a.plot_xy('LetterQuality','CollegeGPA',regress=True,quart=opts.plotquartile)
    if False:
        # unit tests - sorry, didn't get a chance to put them in here.
        # as you can see above, i validated regression output by comparing my
        # regression function with the stock regression function in scipy (and got identical results)
        # BUT: here's an example of how I would go about creating small tests for each piece:
        import StringIO
        a.load_file_txt(    StringIO.StringIO('a,b,c\n1,2,3\n4,5,6')   )
        # <blabla>

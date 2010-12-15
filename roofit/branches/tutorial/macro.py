import ROOT

w = ROOT.RooWorkspace('w',ROOT.kTRUE)
w.factory('Gaussian::gauss(mes[5.2,5.3],mean[5.28,5.2,5.3],width[0.0027,0.001,1])')
w.factory('ArgusBG::argus(mes,5.291,argpar[-20,-100,-1])')
w.factory('SUM::sum(nsig[200,0,10000]*gauss,nbg[800,0,10000]*argus)')
w.defineSet('obs','mes')
w.defineSet('argset','argus')

#data = w.pdf('sum').generate(ROOT.RooArgSet(w.obj('mes')),2000)
data = w.pdf('sum').generate(w.set('obs'),2000)
w.pdf('sum').fitTo(data) ;

mesframe = w.var('mes').frame()
data.plotOn(mesframe)
w.pdf('sum').plotOn(mesframe)
w.pdf('sum').plotOn(mesframe,ROOT.RooFit.Components(w.set('argset')),ROOT.RooFit.LineStyle(ROOT.kDashed))
mesframe.Draw()



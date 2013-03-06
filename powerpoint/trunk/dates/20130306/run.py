#!/usr/bin/env python

bins = [0.0,0.21,0.42,0.63,0.84,1.05,1.37,1.52,1.74,1.95,2.18,2.4]

x=r"""
\slide{ $\mu^{%s}$: bin %d ($%.2f<\%s<%.2f$) }
{

\colb[T]

\column{.5\textwidth}
C-side (top: W; bottom: Z)
\centering
\includegraphics[width=0.66\textwidth]{dates/20130306/figures/etaphi/W_%d_C_stack_l_%s_%s} \\
\includegraphics[width=0.66\textwidth]{dates/20130306/figures/etaphi/Z_%d_C_stack_l%s_%s_ALL.pdf}

\column{.5\textwidth}
A-side (top: W; bottom: Z)
\centering
\includegraphics[width=0.66\textwidth]{dates/20130306/figures/etaphi/W_%d_A_stack_l_%s_%s} \\
\includegraphics[width=0.66\textwidth]{dates/20130306/figures/etaphi/Z_%d_A_stack_l%s_%s_ALL.pdf} 

\cole
}
"""

xE=r"""
\slide{ $\eta\ plots$ }
{
\Huge ETA plots
}
"""
xP=r"""
\slide{ $\phi\ plots$ }
{
\Huge PHI plots
}
"""

for var in ('eta','phi'):
    print (xE if var=='eta' else xP)
    for b in xrange(1,12):
        bmin,bmax = bins[b-1],bins[b]
        for q in (0,1):
            sgn = '+' if q==0 else '-'
            qW = 'POS' if q==0 else 'NEG'
            qZ = 'P' if q==0 else 'N'
            print x%(sgn,b,bmin,var,bmax,  b,var,qW,  b,qZ,var  ,  b,var,qW,  b,qZ,var)

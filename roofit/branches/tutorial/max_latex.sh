#!/bin/bash

out=/home/antonk/SupportingDocument/Wmunu/WmunuBackgroundTables.tex
out=bla.tex

#./max_latex.py OUT_01072013_ALL.v3.abseta.1D.pt25.root POS
./max_latex.py OUT_01072013_ALL.v3.abseta.2D.pt20.root NEG 1

#./max_latex.py 1 POS >  ${out} && ./max_latex.py 1 NEG >> ${out} && ./max_latex.py 2 POS >> ${out} && ./max_latex.py 2 NEG >> ${out} && echo DONE

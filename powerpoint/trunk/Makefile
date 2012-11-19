BASENAME = wzinclusive

default: quick

full:
	pdflatex  ${BASENAME}
	bibtex    ${BASENAME}
	pdflatex  ${BASENAME}
	pdflatex  ${BASENAME}

quick:
	pdflatex  ${BASENAME}

.PHONY: clean

clean:
	rm -f *.aux *.log *.bbl *.blg *.brf *.cb *.ind *.idx *.ilg  \
	      *.inx *.dvi *.toc *.out *~ ~* spellTmp


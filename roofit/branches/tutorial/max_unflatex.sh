#!/bin/bash

fout=/home/antonk/SupportingDocument/Wmunu/WmunuCrossSectionTables.tex

echo """\begin{table}
  \begin{center}
"
cat Wmn_UNF1D_Plus_UnfAbs_proj.tex
echo """\label{tab:Wenu:DiffXSPos}
\caption{Single-differential cross section in the \Wmunup\ channel.}
  \end{center}

\end{table}
"""

echo
echo

echo """\begin{table}
  \begin{center}
"
cat Wmn_UNF1D_Minus_UnfAbs_proj.tex
echo """\label{tab:Wenu:DiffXNeg}
\caption{Single-differential cross section in the \Wmunum\ channel.}
  \end{center}

\end{table}
"""


echo
echo

echo """\begin{table}
  \begin{center}
"""
head -n50  Wmn_UNF2D_Plus_UnfAbs_2d.tex
echo """\hline
\end{tabular}
\label{tab:Wmunu:DoubleDiffXSPos_1}
\caption{Double-differential cross section in the \Wmunup\ channel.}
\end{center}
\end{table}

\begin{table}
  \begin{center}
"""
head -n2 Wmn_UNF2D_Plus_UnfAbs_2d.tex
z=`expr $(wc -l <  Wmn_UNF2D_Plus_UnfAbs_2d.tex) - 50`
tail -n ${z} Wmn_UNF2D_Plus_UnfAbs_2d.tex
echo """

\label{tab:Wmunu:DoubleDiffXSPos_2}
\caption{Double-differential cross section in the \Wmunup\ channel.}
\end{center}
\end{table}
"""





echo
echo

echo """\begin{table}
  \begin{center}
"""
head -n50  Wmn_UNF2D_Minus_UnfAbs_2d.tex
echo """\hline
\end{tabular}
\label{tab:Wmunu:DoubleDiffXSNeg_1}
\caption{Double-differential cross section in the \Wmunum\ channel.}
\end{center}
\end{table}

\begin{table}
  \begin{center}
"""
head -n2 Wmn_UNF2D_Minus_UnfAbs_2d.tex
z=`expr $(wc -l <  Wmn_UNF2D_Minus_UnfAbs_2d.tex) - 50`
tail -n ${z} Wmn_UNF2D_Minus_UnfAbs_2d.tex
echo """

\label{tab:Wmunu:DoubleDiffXSNeg_2}
\caption{Double-differential cross section in the \Wmunum\ channel.}
\end{center}
\end{table}
"""

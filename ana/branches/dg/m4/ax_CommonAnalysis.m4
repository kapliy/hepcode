
dnl checks if external physics analysis packages are available under CommonAnalysis
dnl if yes, then COMMONANALYSIS_CFLAGS, COMMONANALYSIS_LDFLAGS
dnl are set appropriately.

AC_DEFUN([AX_COMMONANALYSIS],
[
  dnl retrieve path to CommonAnalysis base directory.
  user_commonanalysis="${PWD}/CommonAnalysis"
  AC_ARG_WITH(commonanalysis,
    AC_HELP_STRING([--with-commonanalysis the directory with RootCore packages]),[
    if test -d "$withval"; then
	user_commonanalysis=$withval
    fi
  ])

  COMMONANALYSIS_LDFLAGS=
  COMMONANALYSIS_CPPFLAGS=

dnl check that RootCore packages are in place, and that ROOTCOREDIR is available
  AC_MSG_CHECKING([for CommonAnalysis headers and libraries])
  if test -f "${user_commonanalysis}/RootCore/lib/libPileupReweighting.so" && test -f "${ROOTCOREDIR}/lib/libPileupReweighting.so"; then
	COMMONANALYSIS_LDFLAGS='$(shell $(ROOTCOREDIR)/scripts/get_ldflags.sh egammaAnalysisUtils MuonIsolationCorrection MuonMomentumCorrections MuonEfficiencyCorrections JetResolution JetUncertainties MV1Tagger ApplyJetCalibration ApplyJetResolutionSmearing TrigMuonEfficiency PileupReweighting GoodRunsLists MissingETUtility ScaleFactorProvider LineShapeTool)'
        COMMONANALYSIS_CPPFLAGS='-I$(ROOTCOREBIN)/include $(shell $(ROOTCOREDIR)/scripts/get_cxxflags.sh egammaAnalysisUtils MuonIsolationCorrection MuonMomentumCorrections MuonEfficiencyCorrections JetResolution JetUncertainties MV1Tagger ApplyJetCalibration ApplyJetResolutionSmearing TrigMuonEfficiency PileupReweighting GoodRunsLists MissingETUtility ScaleFactorProvider LineShapeTool)'
        AC_DEFINE(HAVE_COMMONANALYSIS,,[define if the CommonAnalysis package is available])
        AC_MSG_RESULT(yes)
   else
	AC_MSG_NOTICE([WARNING: building wihout CommonAnalysis/RootCore packages])
	AC_MSG_RESULT(no)
  fi
  AC_SUBST(COMMONANALYSIS_LDFLAGS)
  AC_SUBST(COMMONANALYSIS_CPPFLAGS)

])

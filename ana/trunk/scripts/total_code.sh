#!/bin/bash
CPPFILES=`find . -name "*.cpp"`
HPPFILES=`find . -name "*.hpp"`
CCFILES=`find . -name "*.cc"`
HHFILES=`find . -name "*.hh"`
CXXFILES=`find . -name "*.cxx"`
CFILES=`find . -name "*.C"`
HFILES=`find . -name "*.h"`
FILENAMES="${CPPFILES} ${HPPFILES} ${CCFILES} ${HHFILES} ${CXXFILES} ${CFILES} ${HFILES}"
echo Total lines of code:
wc ${FILENAMES}
echo Total size of code:
du -hc ${FILENAMES}

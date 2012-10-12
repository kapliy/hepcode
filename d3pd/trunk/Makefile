all:
	rm -f MissingETUtility/StandAlone/libMETUtility.so
	make -C MissingETUtility/cmt/ -f Makefile.Standalone clean
	make -C MissingETUtility/cmt/ -f Makefile.Standalone -j4
	rm -f MissingETUtility-00-02-14/StandAlone/libMETUtility.so
	make -C MissingETUtility-00-02-14/cmt/ -f Makefile.Standalone clean
	make -C MissingETUtility-00-02-14/cmt/ -f Makefile.Standalone -j4

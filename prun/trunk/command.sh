./prunftk.py --site=ANALY_MWT2 --nFilesPerJob=2 --nJobs 2 --exec "echo %IN; ls -lL > out_files.dat;" --inDS=user10.AntonKapliy.v14wh3e34ZEROREC.250evts.v1 --outputs out_files.dat --outDS user.kapliy.prun.staging.test.june21.2010.test06 --FTKRegions 0,1 --FTKSubRegions 0,1,2,3 --FTKDSs=IN2:user10.AntonKapliy.FtkV14_base4L11L_no4Lfilt_2010_05_12.v1:*sectors_raw_4L_1M_reg%REG_sub%SUB*:*corrgen_raw_11L_1M_reg%REG_sub%SUB*,IN3:user10.AntonKapliy.v14.raw_4L_20x156I300x25_1M_pgenonly_no4Lfilt.2010_05_12.v1:*patterns_raw_4L*1M_reg%REG_sub%SUB* --noSubmit

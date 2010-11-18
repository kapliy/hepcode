runsG="
                                                                          165591                                  
                                                                          165632                                  
                                                                          165703                                  
                                                                          165732                                  
                                                                          165767                                  
                                                                          165815                                  
                                                                          165817                                  
                                                                          165818                                  
                                                                          165821                                  
                                                                          165954                                  
                                                                          165956                                  
                                                                          166097                                  
                                                                          166142                                  
                                                                          166143                                  
                                                                          166198                                  
"
runsH="
                                                                          166466                                  
                                                                          166658                                  
                                                                          166786                                  
                                                                          166850                                  
                                                                          166856                                  
                                                                          166924                                  
                                                                          166927                                  
                                                                          166964                                  
"
runsI="
                                                                          167575                                  
                                                                          167576                                  
                                                                          167607                                  
                                                                          167661                                  
                                                                          167680                                  
                                                                          167776                                  

"

for p in {G,H,I}; do
    nn=runs${p}
    for r in ${!nn}; do
	echo asym /period${p}.root/dg/dg/RUNS/${r}/st_w_final/POS/lepton_pt /period${p}.root/dg/dg/RUNS/${r}/st_w_final/NEG/lepton_pt
	#echo asym /period${p}.root/dg/dg/RUNS/${r}/st_w_final/POS_BARREL/lepton_pt /period${p}.root/dg/dg/RUNS/${r}/st_w_final/NEG_BARREL/lepton_pt
	#echo asym /period${p}.root/dg/dg/RUNS/${r}/st_w_final/POS_ENDCAP/lepton_pt /period${p}.root/dg/dg/RUNS/${r}/st_w_final/NEG_ENDCAP/lepton_pt
    done
done

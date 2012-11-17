#include <iostream>
#include <PDFReweightTool.hpp>

/*
  Memory in default compile:
  00:00:21 298296 3150040
  Memory in optimized compile:
  00:00:08  9384 336016
*/

int main() {
  const std::string LHAPATH = "/share/ftkdata/software/pdfsets";
  PDFReweightTool *rw = new PDFReweightTool();
  rw->SetLHAPATH(LHAPATH);
  rw->AddDefaultPDFSets();
  rw->Initialize();

  double mcevt_pdf_scale = 0;
  int mcevt_id1=0; int mcevt_id2=0;
  double x1=0; double x2=0;
  double pdf1=0; double pdf2=0;
  bool verbose=true;

  // Perform a closure test (using actual Powheg+Pythia W+munu event from FlatNtupler)
  mcevt_pdf_scale=79.820602;mcevt_id1=2; mcevt_id2=21;x1=0.0891022;x2=0.0188473;pdf1=0.6118854;pdf2=5.0477380;

  for(int nset=1; nset<=4; nset++) {
    std::cout << "RUNNING: " << nset << std::endl;
    rw->GetEventWeight(nset,mcevt_pdf_scale,mcevt_id1,mcevt_id2,x1,x2,pdf1,pdf2,verbose);
  }

  std::cerr << "DONE! Going into an infinite loop..." << std::endl;
  while(true) continue;

  return 0;
}

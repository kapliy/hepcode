#include <iostream>
#include <PDFReweightTool.hpp>

int main() {
  PDFReweightTool *rw = new PDFReweightTool();
  if(true) {
    const std::string LHAPATH = "/share/ftkdata/software/pdfsets";
    std::cout << "Using custom $LHAPATH: " << LHAPATH << std::endl;
    rw->SetLHAPATH(LHAPATH);
    rw->AddDefaultPDFSets();
  } else {
    std::cout << "Taking $LHAPATH from athena" << std::endl;
    rw->AddPDFSet(10800,"CT10.LHgrid");
    rw->AddPDFSet(21100,"MSTW2008nlo68cl.LHgrid");
    rw->AddPDFSet(192800,"NNPDF21_100.LHgrid");
    // HERA 1.5 is only available in recent versions of athena
    // rw->AddPDFSet(60700,"HERAPDF15NLO_EIG.LHgrid");
  }
  rw->Initialize();

  double mcevt_pdf_scale = 0;
  int mcevt_id1=0; int mcevt_id2=0;
  double x1=0; double x2=0;
  double pdf1=0; double pdf2=0;
  bool verbose=true;

  // Perform a closure test
  mcevt_pdf_scale=79.820602;mcevt_id1=2; mcevt_id2=21;x1=0.0891022;x2=0.0188473;pdf1=0.6118854;pdf2=5.0477380;

  for(int nset=1; nset<=rw->NPDFSets(); nset++) {
    std::cout << "RUNNING PDFSET # " << nset << std::endl;
    rw->GetEventWeight(nset,mcevt_pdf_scale,mcevt_id1,mcevt_id2,x1,x2,pdf1,pdf2,verbose);
  }

  // while(true) continue;

  return 0;
}

#ifndef PDF_REWEIGHT_TOOL_H
#define PDF_REWEIGHT_TOOL_H

#include <string>
#include <vector>
#include <map>
#include <cmath>

#include <iostream>
#include <cassert>
#include <LHAPDF/LHAPDF.h>

class PDFReweightTool {
protected:
  bool m_ready;
  std::string m_LHAPATH;
  std::vector<std::string> m_pdflist;  // m_pdflist[0] = "LHAPDF_FILENAME.LHAgrid";
  std::map<int,int> m_pdfmap;          // m_pdfmap[LHAPDF_PDFID] = 0;
public:
  PDFReweightTool() : m_ready(false) , m_LHAPATH("") {}
  PDFReweightTool(const std::string& LHAPATH) : m_ready(false), m_LHAPATH(LHAPATH) {}
  ~PDFReweightTool() {}

  void SetLHAPATH(const std::string& LHAPATH) { m_LHAPATH = LHAPATH; }
  const std::string& GetLHAPATH() const { return m_LHAPATH; }
  
  void Initialize(bool verbose=true);
  void Info() const;
 
  unsigned int NPDFSets() const { return m_pdflist.size(); }
  unsigned int numberPDF(int nset) const { return LHAPDF::numberPDF(nset); }
  void AddPDFSet(const int& setid, const std::string& setname) {
    m_pdfmap[m_pdflist.size()] = setid;
    m_pdflist.push_back(setname);
  }

  void SetPDFSets(const std::vector<std::string> &pdflist , const std::map<int,int> &pdfmap) {
    m_pdflist = pdflist;
    m_pdfmap = pdfmap;
  }
  void AddDefaultPDFSets();
  void AddCT10PDFSet();
  
  double GetEventWeight(int nset, int member,                // nset start from 1, member start from 0
			double mcevt_pdf_scale,              // energy scale (Q)
			int mcevt_id1, int mcevt_id2,        // pdg id of partons
			double x1, double x2,                // bjorken momentum fraction of partons
			double pdf1, double pdf2,            // PDF value for partons (or LHAPDF set number in rare cases)
			bool verbose=false) const;
  // this version assumes member=-1 (don't change)
  double GetEventWeight(int nset,                            // nset start from 1
			double mcevt_pdf_scale,              // energy scale (Q)
			int mcevt_id1, int mcevt_id2,        // pdg id of partons
			double x1, double x2,                // bjorken momentum fraction of partons
			double pdf1, double pdf2,            // PDF value for partons (or LHAPDF set number in rare cases)
			bool verbose=false) const {
    return GetEventWeight(nset,-1,mcevt_pdf_scale,mcevt_id1,mcevt_id2,x1,x2,pdf1,pdf2,verbose);
  }

};

void PDFReweightTool::Initialize(bool verbose) {
  const int NSETS = m_pdflist.size();
  assert(NSETS<=6 && "PDFReweightTool: cannot initialize more than 6 PDF sets");
  if(!verbose) LHAPDF::setVerbosity(LHAPDF::SILENT);
  if(m_LHAPATH!="") LHAPDF::setPDFPath(m_LHAPATH);
  LHAPDF::initLHAPDF();
  for(int i=0; i<NSETS; i++) {
    // LHAPDF indices start at 1 (Fortran)
    LHAPDF::initPDFSet(i+1,m_pdflist[i].c_str());
  }
  m_ready = true;
  Info();
}

void PDFReweightTool::Info() const {
  std::cout <<   "PDFReweightTool::Info status - " << (m_ready ? "READY" : "NOT INITIALIZED") << std::endl;
  for(unsigned int i=0; i<m_pdflist.size(); i++) {
    const int nset = i+1;
    std::cout << "                  PDF #" << nset << ": " << m_pdflist[i];
    if(m_ready) std::cout << " with " << LHAPDF::numberPDF(nset) << " members";
    std::cout << std::endl;
  }
}

void PDFReweightTool::AddDefaultPDFSets() {
  assert(!m_ready && "ERROR: PDFReweightTool::GetWeight must be called before PDFReweightTool::Initialize");
  // the following PDF sets are recommended for 2011 W/Z paper
  AddPDFSet(10800,"CT10.LHgrid");
  //AddPDFSet(11000,"CT10nlo.LHgrid");
  AddPDFSet(21100,"MSTW2008nlo68cl.LHgrid");
  AddPDFSet(60700,"HERAPDF15NLO_EIG.LHgrid");     // not in 17.6.0
  AddPDFSet(229800,"NNPDF23_nlo_as_0118.LHgrid"); // not in 17.6.0
  AddPDFSet(42060,"abm11_5n_nlo.LHgrid");         // not in 17.6.0
}

void PDFReweightTool::AddCT10PDFSet() {
  assert(!m_ready && "ERROR: PDFReweightTool::GetWeight must be called before PDFReweightTool::Initialize");
  // the following PDF sets are recommended for 2011 W/Z paper
  AddPDFSet(10800,"CT10.LHgrid");
}

double PDFReweightTool::GetEventWeight(int nset, int member,
				       double mcevt_pdf_scale,
				       int mcevt_id1, int mcevt_id2,
				       double x1, double x2,
				       double pdf1, double pdf2,
				       bool verbose) const {
  assert(m_ready && "ERROR: PDFReweightTool::GetWeight must be called after PDFReweightTool::Initialize");
  const int NSETS = m_pdflist.size();
  // nset starts at 1!
  assert(nset>=1 && nset<=NSETS);
  //assert(member<=0 && "PDFReweightingTool: TrigFTKAna is configured with --enable-low-memory option, precluding the use of PDF members");
  // member starts at 0!
  if(member>=0) {
    assert(member <= LHAPDF::numberPDF(nset) );
    LHAPDF::usePDFMember(nset,member);
  }
  const double Q = mcevt_pdf_scale;
  const int id1 = mcevt_id1==21 ? 0 : mcevt_id1;
  const int id2 = mcevt_id2==21 ? 0 : mcevt_id2;
  int pdf1i = int(pdf1);
  // Most MC11 AOD samples already contain actual values of the PDF(x,Q,id)
  // But in rare cases, pdf1() encodes pdf1_id instead (e.g, PythiaBB)
  double porig = pdf1*pdf2;
  if (pdf1>10000 && (pdf2<=0 or pdf1==pdf2)) {
    if( m_pdfmap.find(pdf1i)!=m_pdfmap.end() ) {
      const int idx = m_pdfmap.find(pdf1i)->second;
      porig = LHAPDF::xfx(idx,x1,Q,id1) * LHAPDF::xfx(idx,x2,Q,id2);
    } else {
      // need to recompute probability for original PDF, but user didn't load it. bail out.
      porig = 0;
    }
  }
  const double pdf1_new = LHAPDF::xfx(nset,x1,Q,id1);
  const double pdf2_new = LHAPDF::xfx(nset,x2,Q,id2);
  const double pnew = pdf1_new * pdf2_new;
  if(verbose) {
    std::streamsize oldp = std::cout.precision();
    std::cout.precision(5);
    std::cout << "PDFReweightTool: ORIGINAL   = " << pdf1 << " * " << pdf2 << " = " << porig << " | "
	      << "Q=" << Q << " x1=" << x1 << " x2=" << x2 << " id1=" << id1 << " id2=" << id2 
	      << std::endl
	      << "                 RECOMPUTED = " << pdf1_new << " * " << pdf2_new << " = " << pnew
	      << "                 EQUAL? " << (std::abs(pnew-porig)<0.00005 ? "YES" : "NO" )
	      << std::endl;
    std::cout.precision(oldp);
  }
  return porig ? pnew/porig : 1.0;
}


#endif // PDF_REWEIGHT_TOOL_H

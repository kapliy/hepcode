 
#ifndef D3PDJETMETTREEBASE_HPP
#define D3PDJETMETTREEBASE_HPP

#include <Rtypes.h>

class TTree;

class
D3PDJetMETTreeBase
{
public:
  D3PDJetMETTreeBase() {}
  virtual ~D3PDJetMETTreeBase() {}
  virtual Int_t    GetEntry(Long64_t entry) = 0;
  virtual Long64_t LoadTree(Long64_t entry) = 0;
  virtual void     Init(TTree *tree) = 0;
};

#endif // D3PDJETMETTREEBASE_HPP

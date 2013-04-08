 
#ifndef D3PDWZTREEBASE_HPP
#define D3PDWZTREEBASE_HPP

#include <Rtypes.h>

class TTree;

class
D3PDWZTreeBase
{
public:
  D3PDWZTreeBase() {}
  virtual ~D3PDWZTreeBase() {}
  virtual Int_t    GetEntry(Long64_t entry) = 0;
  virtual Long64_t LoadTree(Long64_t entry) = 0;
  virtual void     Init(TTree *tree) = 0;
};

#endif // D3PDWZTREEBASE_HPP

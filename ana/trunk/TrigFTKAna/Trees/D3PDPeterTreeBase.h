 
#ifndef D3PDPeterTREEBASE_HPP
#define D3PDPeterTREEBASE_HPP

#include <Rtypes.h>

class TTree;

class
D3PDPeterTreeBase
{
public:
  D3PDPeterTreeBase() {}
  virtual ~D3PDPeterTreeBase() {}
  virtual Int_t    GetEntry(Long64_t entry) = 0;
  virtual Long64_t LoadTree(Long64_t entry) = 0;
  virtual void     Init(TTree *tree) = 0;
};

#endif // D3PDPeterTREEBASE_HPP

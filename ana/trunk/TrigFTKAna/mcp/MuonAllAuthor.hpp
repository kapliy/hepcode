#ifndef WRAP_MUONALLAUTHOR_HPP
#define WRAP_MUONALLAUTHOR_HPP

namespace
MuonAllAuthor
{
  // get all the authors of this Muon for example during overlap
  // checking, the same Muon may have been reconstructed by many
  // different algorithms.  the authors are 16-bit word, 1-bit
  // reserved for each muon Algorithm the positions of the 1-bit are
  // defined as follows:
  //
  //  Muonboy | STACO | MuTag | Muid SA | Muid Co | MuGirl | MuGirlLowBeta | CaloMuon | CaloTag | CaloLikelihood | MuTagIMO | MuonCombinedRefit | ExtrapolateMuonToIP
  //   x         x        x      x          x        x          x              x          x            x             x               x           x
  //
  //  each of these authors have 1 bit and the lower bits are Muonboy
  typedef enum {
    Muonboy = 0 ,
    STACO = 1 ,
    MuTag = 2 , 
    MuidSA = 3 ,
    MuidCo = 4 ,
    MuGirl = 5 ,
    MuGirlLowBeta = 6 ,
    CaloID = 7 ,
    CaloTag = 8 ,
    CaloLikelihood = 9 ,
    MuTagIMO = 10 ,
    MuonCombinedRefit = 11 ,
    ExtrapolatedMuonToIP = 12
  } MuonTypeEnum;
};

namespace
MuonPrimAuthor
{
  // Definitions of the primary author field
  // the primary author is the algorithm that first created the Muon
  typedef enum {
    unknown=0, 
    highPt=1, 
    lowPt=2, 
    mediumPt=3,
    MuonboySP=4, 
    Muonboy=5, 
    STACO=6, 
    MuTag=7, 
    MOORE=10,
    MuidSA=11,
    MuidCo=12,
    MuGirl=13,
    CaloMuonId=14,
    MuGirlLowBeta=15,
    CaloTag=16,
    CaloLikelihood=17,
    MuTagIMO=18,
    MuonCombinedRefit=19,
    ExtrapolateMuonToIP=20,
    IDtrack=21,
    NumberOfMuonAuthors=22 // increase this guy when adding
  } MuonTypeEnum;

};

#endif // WRAP_MUONALLAUTHOR_HPP

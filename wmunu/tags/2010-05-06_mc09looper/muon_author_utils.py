##  Muonboy | STACO | MuTag | Muid SA | Muid Co | MuGirl | MuGirlLowBeta | CaloMuon | CaloTag | CaloLikelihood | MuTagIMO

author_MuonBoy  = 1 << 0
author_STACO    = 1 << 1
author_MuTag    = 1 << 2
author_Muid_SA  = 1 << 3
author_Muid_Co  = 1 << 4
author_MuGirl   = 1 << 5

def has_MuonBoy(authorField):
    return (authorField & author_MuonBoy != 0)

def has_STACO(authorField):
    return (authorField & author_STACO != 0)

def has_MuTag(authorField):
    return (authorField & author_MuTag != 0)


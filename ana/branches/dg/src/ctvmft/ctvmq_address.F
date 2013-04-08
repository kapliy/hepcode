#include "TrigFTKAna/ctvmft/CtvmftDimensions.h"

      integer function ctvmq_address()
c
c    Get address of CTVMQ common
c
      implicit none
#include "TrigFTKAna/ctvmft/ctvmdi.h"
#include "TrigFTKAna/ctvmft/ctvmft.h"

C  CTC first approximation and track-vertex step parameters for CTVMFT
      DATA  DRMAX /  2.0 /  ! max XY separ., non-intersecting circles
      DATA  DZMAX / 20.0 /  ! max allowed track Z separation at the vtx
      DATA  RVMAX / 70.0 /  ! max allowed vertex radius
      DATA TRNMAX /  0.5 /  ! max permitted track turning angle to vtx
      DATA  DSMIN / -2.0 /  ! min permitted arc length, track to vtx
      DATA   CVTX /MAXVTX*0/
      
      integer address
      
      ctvmq_address=address(runnum)
      
      return
      end

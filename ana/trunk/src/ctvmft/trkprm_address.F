#include "TrigFTKAna/ctvmft/CtvmftDimensions.h"
      integer function trkprm_address()
c
c    Get address of TRKPRM common
c
      implicit none
#include "TrigFTKAna/ctvmft/ctvmdi.h"
#include "TrigFTKAna/ctvmft/ctvmtr.h"

      integer address
      
      trkprm_address = address(trhelix(1,1))

      return
      end

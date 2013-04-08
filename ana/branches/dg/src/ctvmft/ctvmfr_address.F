#include "TrigFTKAna/ctvmft/CtvmftDimensions.h"
      integer function ctvmfr_address()
c
c    Get address of CTVMFR common
c
      implicit none
#include "TrigFTKAna/ctvmft/ctvmdi.h"
#include "TrigFTKAna/ctvmft/ctvmft.h"

      integer address
      
      ctvmfr_address = address(vmat(1,1))

      return
      end

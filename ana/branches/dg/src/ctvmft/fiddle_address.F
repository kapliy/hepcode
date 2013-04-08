      integer function fiddle_address()
c
c    Get address of FIDDLE common
c
      implicit none
#include "TrigFTKAna/ctvmft/ctvmfi.h"      
      integer address

      fiddle_address = address(excuse)

      return
      end

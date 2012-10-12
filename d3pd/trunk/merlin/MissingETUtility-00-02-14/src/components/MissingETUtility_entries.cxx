//
//
//


#include "GaudiKernel/DeclareFactoryEntries.h"

#include "../METUtilityAthTool.h"
#include "../METUtilAlg.h"



using namespace MissingETUtility;

DECLARE_ALGORITHM_FACTORY(METUtilAlg)

DECLARE_TOOL_FACTORY(METUtilityAthTool)

DECLARE_FACTORY_ENTRIES(MissingETUtility) {

    DECLARE_ALGORITHM(METUtilAlg)

    DECLARE_TOOL(METUtilityAthTool)

}

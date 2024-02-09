/** @file scGenComp_PU_Tech.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scGenComp_PU_Tech.h"
// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"


// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing

//GenCompStates_Tech* TheGenCompStates_Tech;
    scGenComp_PU_Tech::
scGenComp_PU_Tech(sc_core::sc_module_name nm, int32_t No):
    scGenComp_PU_Abstract(nm)
     ,mNoOfArgs(No)
{
    typedef scGenComp_PU_Tech SC_CURRENT_USER_MODULE;
//    if(!TheGenCompStates_Tech)
  //      TheGenCompStates_Tech = new GenCompStates_Tech();
    //MachineState = TheGenCompStates_Tech;
    // ** Do not reimplement any of the xxx_method functions
    // until you know what you are doing
}

scGenComp_PU_Tech::
    ~scGenComp_PU_Tech(void)
{
}

// Puts the PU in its default state
void scGenComp_PU_Tech::
    Initialize_method(void)
{
                DEBUG_SC_EVENT_LOCAL(">>>   Intializing");
    scGenComp_PU_Abstract::Initialize_method();
//        MachineState->Initialize(this);   // Change status to 'Ready'
    Initialize_Do(); // Initialize the unit, HW and temporary variables
}


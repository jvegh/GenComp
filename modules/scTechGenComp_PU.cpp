/** @file scTechGenComp_PU.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scTechGenComp_PU.h"
// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing

// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing

TechGenCompState* TheTechGenCompState;
    scTechGenComp_PU::
scTechGenComp_PU(sc_core::sc_module_name nm, int32_t No):
    scAbstractGenComp_PU(nm)
     ,mNoOfArgs(No)
{
    typedef scTechGenComp_PU SC_CURRENT_USER_MODULE;
    if(!TheTechGenCompState)
        TheTechGenCompState = new TechGenCompState();
    MachineState = TheTechGenCompState;
}

scTechGenComp_PU::
    ~scTechGenComp_PU(void)
{
}

// Puts the PU in its default state
void scTechGenComp_PU::
    Initialize_method(void)
{
        DEBUG_SC_EVENT_LOCAL(">>>   Intializing");
        scAbstractGenComp_PU::Initialize_method();
        MachineState->Initialize(this);   // Change status to 'Initial'
        Initialize(); // Initialize the unit, HW and temporary variables
        // Put PU in its default state
        DEBUG_SC_EVENT_LOCAL("<<<   Intializing");
}


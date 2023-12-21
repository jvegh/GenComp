/** @file scBioGenComp_PU.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scBioGenComp_PU.h"
// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing
static BioGenCompState *TheBioGenCompState;

// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing
    scBioGenComp_PU::
scBioGenComp_PU(sc_core::sc_module_name nm):
    scAbstractGenComp_PU(nm)
{
        if(!TheBioGenCompState)
            TheBioGenCompState = new BioGenCompState();
    MachineState =  TheBioGenCompState;
}


    scBioGenComp_PU::
~scBioGenComp_PU(void)
{
}

// Puts the PU in its default state
void scBioGenComp_PU::
    Initialize_method(void)
{
    DEBUG_SC_EVENT("Intializing");
    DEBUG_SC_PRINT("Enter EVENT_BIOGenComp.Initialize");
    MachineState->Initialize(this);   // Change status to 'Initial'
    Initialize(); // Initialize the unit, HW and temporary variables
    // Put PU in its default state
   // DEBUG_PRINT_SC("Exit  EVENT_GenComp.Initialize");
}

    void scBioGenComp_PU::
Process_method(void)
{
 //   int32_t i = State_Get()->Flag_Get();
}

void scBioGenComp_PU::
    Initialize(void)
{
}


/** @file GenCompStates_Bio.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for biological generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scGenComp_PU_Bio.h"
#include "GenCompStates_Abstract.h"

// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing

// The units of biological general computing work in the same way, using general events
// The XXX_method() is activated by the event; DoXXX makes the activity, if the state is OK

GenCompStates_Bio::
    GenCompStates_Bio(void): GenCompStates_Abstract()
{}

GenCompStates_Bio::
    ~GenCompStates_Bio(void)
{
}


    void GenCompStates_Bio::
Deliver(scGenComp_PU_Abstract* PU)
{
    assert(gcsm_Processing == PU->StateFlag_Get());
    PU->StateFlag_Set(gcsm_Delivering);
}

//Can happen only in Processing state; passes to Relaxing state
void GenCompStates_Bio::
    Fail(scGenComp_PU_Abstract* PU)
{
    if(gcsm_Processing== PU->StateFlag_Get())
        PU->Failed_Do();
    // Otherwise neglect it

}


void GenCompStates_Bio::
    Process(scGenComp_PU_Abstract* PU)
{
    assert((gcsm_Processing == PU->StateFlag_Get()) || (gcsm_Ready == PU->StateFlag_Get())); // No other case allowed
        // Legally received a 'Begin processing' signal
    if(gcsm_Ready == PU->StateFlag_Get())
    {   // We are still in 'Ready' state, set the mode and
        PU->StateFlag_Set(gcsm_Processing);
    }
    // We are already in 'Processing'
}


void GenCompStates_Bio::
    Relax(scGenComp_PU_Abstract* PU)
{

}

// Put the PU electronics to low-power mode
// Must come from 'Ready' state; otherwise fail
    void GenCompStates_Bio::
Sleep(scGenComp_PU_Abstract* PU)
{
    assert(gcsm_Sleeping != PU->StateFlag_Get());
    PU->StateFlag_Set(gcsm_Sleeping);
}

void GenCompStates_Bio::
    Synchronize(scGenComp_PU_Abstract* PU)
{
}

    void GenCompStates_Bio::
State_Set(scGenComp_PU_Abstract* PU, GenCompStateMachineType_t& State)
{
    PU-> StateFlag_Set( State);
}



// Overload if want to use "dormant" state
void GenCompStates_Bio::
    Wakeup(scGenComp_PU_Abstract* PU)
{
    //   machine.WakeUp();
}



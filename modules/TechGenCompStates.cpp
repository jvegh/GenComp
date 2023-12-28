/** @file TechGenCompStates.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for technical generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scGenComp_PU_Tech.h"

// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
//#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing
// The units of general computing work in the same way, using general events
// The XXX_method() is activated by the event; XXX makes the activity, if the stae is OK

TechGenCompState::
    TechGenCompState(void):
    AbstractGenCompState()
{
}
TechGenCompState::
    ~TechGenCompState(void)
{
}

// Overload if want to use "dormant" state
   void TechGenCompState::
Wakeup(scGenComp_PU_Tech *PU)
{
}

    void TechGenCompState::
Deliver(scGenComp_PU_Tech *PU)
{
}

// Put the PU electronics to low-power mode
// Must come from 'Ready' state; otherwise fail
    void TechGenCompState::
Sleep(scGenComp_PU_Tech* PU)
{
    assert(gcsm_Sleeping != PU->StateFlag_Get());
    PU->StateFlag_Set(gcsm_Sleeping);
}

    void TechGenCompState::
Process(scGenComp_PU_Tech* PU)
{
//    State_Set(machine, new ProcessingGenCompState(name()));
}

    void TechGenCompState::
Relax(scGenComp_PU_Tech* PU)
{

    }

// Although a method thread, is invoked from the constructor
    void TechGenCompState::
Initialize(scGenComp_PU_Tech* PU)
{
    PU-> StateFlag_Set(gcsm_Ready);
}

/*
 *  The machine PU received new input, administer it
 *  Input can be received only in 'Ready' and 'Processing' states
 */
void TechGenCompState::
    InputReceived(scGenComp_PU_Tech* PU)
{
    if((gcsm_Ready == PU->StateFlag_Get()) || (gcsm_Processing== PU->StateFlag_Get()))
        PU->ReceiveInput();
    // Otherwise neglect it
}


void TechGenCompState::
    Synchronize(scGenComp_PU_Tech* PU)
{
}

//Can happen only in Processing state; passes to Relaxing state
    void TechGenCompState::
Fail(scGenComp_PU_Tech* PU)
{
    if(gcsm_Processing== PU->StateFlag_Get())
        PU->Fail();
    // Otherwise neglect it

}

    void TechGenCompState::
State_Set(scGenComp_PU_Tech* PU, GenCompStateMachineType_t& State)
{
    PU-> StateFlag_Set( State );
}

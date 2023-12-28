/** @file BioGenCompStates.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for biological generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scGenComp_PU_Bio.h"
#include "GenCompStates.h"

// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing


// The units of biological general computing work in the same way, using general events
// The XXX_method() is activated by the event; XXX makes the activity, if the stae is OK

BioGenCompState::
    BioGenCompState(void): AbstractGenCompState()
{}

BioGenCompState::
    ~BioGenCompState(void)
{
}

// Overload if want to use "dormant" state
   void BioGenCompState::
Wakeup(scGenComp_PU_Bio *PU)
{
 //   State_Set(machine, new ReadyGenCompState(name()));
 //   machine.WakeUp();
}

    void BioGenCompState::
Deliver(scGenComp_PU_Bio *PU)
{
}

// Put the PU electronics to low-power mode
// Must come from 'Ready' state; otherwise fail
    void BioGenCompState::
Sleep(scGenComp_PU_Bio* PU)
{
    assert(gcsm_Sleeping != PU->StateFlag_Get());
    PU->StateFlag_Set(gcsm_Sleeping);
}

    void BioGenCompState::
Process(scGenComp_PU_Bio* PU)
{
//    State_Set(machine, new ProcessingGenCompState(name()));
}

    void BioGenCompState::
Relax(scGenComp_PU_Bio* PU)
{

// Although a method thread, is invoked from the constructor
}
    void BioGenCompState::
Initialize(scGenComp_PU_Bio* PU)
{
    PU->StateFlag_Set(gcsm_Ready);
}

/*
 *  The machine PU received new input, administer it
 *  Input can be received only in 'Ready' and 'Processing' states
 */
void BioGenCompState::
    InputReceived(scGenComp_PU_Bio* PU)
{
    if((gcsm_Ready == PU->StateFlag_Get()) || (gcsm_Processing== PU->StateFlag_Get()))
    {   // Inputs are received only in 'processing' mode, otherwise we neglect it
        if(gcsm_Ready == PU->StateFlag_Get())
        {   // A new spike in 'Ready' state received; we start processing
            PU->StateFlag_Set(gcsm_Processing);
            PU->EVENT_GenComp.Begin_Computing.notify(SC_ZERO_TIME); // Issue 'Begin Computing'
            PU->EVENT_GenComp.HeartBeat.notify(BIO_HEARTBEAT_TIME); // Issue first heartbeat
        }
        PU->ReceiveInput(); // Make the administration of the received input
    }
 }


void BioGenCompState::
    Synchronize(scGenComp_PU_Bio* PU)
{
}

//Can happen only in Processing state; passes to Relaxing state
    void BioGenCompState::
Fail(scGenComp_PU_Bio* PU)
{
    if(gcsm_Processing== PU->StateFlag_Get())
        PU->Fail();
    // Otherwise neglect it

}

    void BioGenCompState::
State_Set(scGenComp_PU_Bio* PU, GenCompStateMachineType_t& State)
{
    PU-> StateFlag_Set( State);
}

/** @file BioGenCompStates.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for biological generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scBioGenComp_PU.h"
#include "GenCompStates.h"

// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
//#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing

// The units of general computing work in the same way, using general events
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
Wakeup(scBioGenComp_PU *PU)
{
 //   State_Set(machine, new ReadyGenCompState(name()));
 //   machine.WakeUp();
}

    void BioGenCompState::
Deliver(scBioGenComp_PU *PU)
{
}

// Put the PU electronics to low-power mode
// Must come from 'Ready' state; otherwise fail
    void BioGenCompState::
Sleep(scBioGenComp_PU* PU)
{
    assert(gcsm_Sleeping != PU->StateFlag_Get());
    PU->StateFlag_Set(gcsm_Sleeping);
}

    void BioGenCompState::
Process(scBioGenComp_PU* PU)
{
//    State_Set(machine, new ProcessingGenCompState(name()));
}

    void BioGenCompState::
Relax(scBioGenComp_PU* PU)
{

// Although a method thread, is invoked from the constructor
}
    void BioGenCompState::
Initialize(scBioGenComp_PU* PU)
{
    PU->StateFlag_Set(gcsm_Ready);
}

/*
 *  The machine PU received new input, administer it
 *  Input can be received only in 'Ready' and 'Processing' states
 */
void BioGenCompState::
    InputReceived(scBioGenComp_PU* PU)
{
    if((gcsm_Ready == PU->StateFlag_Get()) || (gcsm_Processing== PU->StateFlag_Get()))
        PU->ReceiveInput();
    // Otherwise neglect it
}


void BioGenCompState::
    Synchronize(scBioGenComp_PU* PU)
{
}

//Can happen only in Processing state; passes to Relaxing state
    void BioGenCompState::
Fail(scBioGenComp_PU* PU)
{
    if(gcsm_Processing== PU->StateFlag_Get())
        PU->Fail();
    // Otherwise neglect it

}

    void BioGenCompState::
State_Set(scBioGenComp_PU* PU, GenCompStateMachineType_t& State)
{
    PU-> StateFlag_Set( State);
 }
/*
    ReadyBioGenCompState::
     ReadyBioGenCompState:
    BioGenCompState()
{  }

    ReadyBioGenCompState::
~ReadyBioGenCompState(){}

    SleepingBioGenCompState::
        SleepingBioGenCompState:
        BioGenCompState()
{
    }
*/
    SleepingBioGenCompState::
~SleepingBioGenCompState(){}

    void SleepingBioGenCompState::
Process(scBioGenComp_PU* PU)
{
    assert(0);     // Process signal while sleeping
    // wake it up first
}


    void ReadyBioGenCompState::
Process(scBioGenComp_PU* PU)
 {
 }

    void ReadyBioGenCompState::
Deliver(scBioGenComp_PU* PU)
 {
 }

    void ReadyBioGenCompState::
 Relax(scBioGenComp_PU* PU)
 {
 }


    void ReadyBioGenCompState::
Reinitialize(scBioGenComp_PU* PU)
{
}


void ReadyBioGenCompState::
    Synchronize(scBioGenComp_PU* PU)
{
}


DeliveringBioGenCompState::
    DeliveringBioGenCompState(void):
    BioGenCompState()
{
 }


DeliveringBioGenCompState::
    ~DeliveringBioGenCompState(){}

ProcessingBioGenCompState::
    ProcessingBioGenCompState(void):
    BioGenCompState()
{ }

void ProcessingBioGenCompState::
    Process(scBioGenComp_PU* PU)
{
     assert(0);     // Process signal during processing
}

ProcessingBioGenCompState::
    ~ProcessingBioGenCompState(){}

RelaxingBioGenCompState::
    RelaxingBioGenCompState(void):
    BioGenCompState()
{   }

RelaxingBioGenCompState::
    ~RelaxingBioGenCompState(){}

FailedBioGenCompState::
    FailedBioGenCompState(void):
    BioGenCompState()
{
    //flag = gcsm_Ready;
}

FailedBioGenCompState::
    ~FailedBioGenCompState(){}

void FailedBioGenCompState::
    Process(scBioGenComp_PU* PU)
{
     assert(0);     // Process signal during processing
}

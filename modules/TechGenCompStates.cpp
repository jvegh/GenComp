/** @file TechGenCompStates.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for technical generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scTechGenComp_PU.h"

// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
//#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing
static TechGenCompState* TheTechGenCompState = new TechGenCompState();
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
Wakeup(scTechGenComp_PU *PU)
{
}

    void TechGenCompState::
Deliver(scTechGenComp_PU *PU)
{
}

// Put the PU electronics to low-power mode
// Must come from 'Ready' state; otherwise fail
    void TechGenCompState::
Sleep(scTechGenComp_PU* PU)
{
    assert(gcsm_Sleeping != PU->StateFlag_Get());
    PU->StateFlag_Set(gcsm_Sleeping);
}

    void TechGenCompState::
Process(scTechGenComp_PU* PU)
{
//    State_Set(machine, new ProcessingGenCompState(name()));
}

    void TechGenCompState::
Relax(scTechGenComp_PU* PU)
{

    }

// Although a method thread, is invoked from the constructor
    void TechGenCompState::
Initialize(scTechGenComp_PU* PU)
{
    PU-> StateFlag_Set(gcsm_Ready);
}

/*
 *  The machine PU received new input, administer it
 *  Input can be received only in 'Ready' and 'Processing' states
 */
void TechGenCompState::
    InputReceived(scTechGenComp_PU* PU)
{
    if((gcsm_Ready == PU->StateFlag_Get()) || (gcsm_Processing== PU->StateFlag_Get()))
        PU->ReceiveInput();
    // Otherwise neglect it
}


void TechGenCompState::
    Synchronize(scTechGenComp_PU* PU)
{
}

//Can happen only in Processing state; passes to Relaxing state
    void TechGenCompState::
Fail(scTechGenComp_PU* PU)
{
    if(gcsm_Processing== PU->StateFlag_Get())
        PU->Fail();
    // Otherwise neglect it

}

    void TechGenCompState::
State_Set(scTechGenComp_PU* PU, GenCompStateMachineType_t& State)
{
    PU-> StateFlag_Set( State );
}
/*
    ReadyTechGenCompState::
     ReadyTechGenCompState:
    TechGenCompState()
{  }

    ReadyTechGenCompState::
~ReadyTechGenCompState(){}

    SleepingTechGenCompState::
    SleepingTechGenCompState:
        TechGenCompState()
{
    }

    SleepingTechGenCompState::
~SleepingTechGenCompState(){}

    void SleepingTechGenCompState::
Process(void)
{
    assert(0);     // Process signal while sleeping
    // wake it up first
}

*/
    void ReadyTechGenCompState::
Process(void)
 {
  //  State_Set(machine, new ProcessingGenCompState(name()));
    // Do some processing
 }

    void ReadyTechGenCompState::
Deliver(void)
 {
 //    State_Set(machine, new DeliveringGenCompState(name()));
//     machine.Deliver();   //Must be implemented in AbstractGenComp_PU subclasses
 }

    void ReadyTechGenCompState::
 Relax()
 {
 }


    void ReadyTechGenCompState::
Reinitialize()
{
}


void ReadyTechGenCompState::
    Synchronize()
{
}


DeliveringTechGenCompState::
    DeliveringTechGenCompState(void):
    TechGenCompState()
{
 }


DeliveringTechGenCompState::
    ~DeliveringTechGenCompState(){}

ProcessingTechGenCompState::
    ProcessingTechGenCompState(void):
    TechGenCompState()
{ }

void ProcessingTechGenCompState::
    Process()
{
     assert(0);     // Process signal during processing
}

ProcessingTechGenCompState::
    ~ProcessingTechGenCompState(){}

RelaxingTechGenCompState::
    RelaxingTechGenCompState(void):
    TechGenCompState()
{   }

RelaxingTechGenCompState::
    ~RelaxingTechGenCompState(){}

FailedTechGenCompState::
    FailedTechGenCompState(void):
    TechGenCompState()
{
    //flag = gcsm_Ready;
}

FailedTechGenCompState::
    ~FailedTechGenCompState(){}

void FailedTechGenCompState::
    Process(void)
{
     assert(0);     // Process signal during processing
}

/** @file GenCompStates_Abstract.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  Working states of the archetypes of processing units
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/
#include "scGenComp_PU_Abstract.h"

// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
//#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing

string GenCompStatesString[]{"Sleeping", "Ready", "Processing", "Delivering", "Relaxing"};

// The units of general computing work in the same way, using general events
// The XXX_method() is activated by the event; XXX makes the activity, if the stae is OK

GenCompStates_Abstract::
    GenCompStates_Abstract(void)
{
}
GenCompStates_Abstract::
    ~GenCompStates_Abstract(void)
{
}


/*
 *  The systems must be in state 'Processing'
 */
    void GenCompStates_Abstract::
Deliver(scGenComp_PU_Abstract* PU)
{
    assert(gcsm_Processing == PU->StateFlag_Get()); // Make sure if it was in state 'Processing'
    PU->StateFlag_Set(gcsm_Delivering);
    PU->EVENT_GenComp.DeliveringBegin.notify(SC_ZERO_TIME);
  }

// Put the PU electronics to low-power mode
// Must come from 'Ready' state; otherwise fail
    void GenCompStates_Abstract::
Sleep(scGenComp_PU_Abstract* PU)
{
    assert(gcsm_Ready == PU->StateFlag_Get());
    PU->StateFlag_Set(gcsm_Sleeping);
    PU->EVENT_GenComp.SleepingBegin.notify(SC_ZERO_TIME);
}

// Must be in "Ready" state to pass to
    void GenCompStates_Abstract::
Process(scGenComp_PU_Abstract* PU)
{
    assert(gcsm_Ready == PU->StateFlag_Get()); // No other case allowed
    // Legally received a 'Begin processing' signal
}

// For relaxing, it must be in statio "Delivering"
    void GenCompStates_Abstract::
Relax(scGenComp_PU_Abstract* PU)
{
    assert(gcsm_Delivering == PU->StateFlag_Get()); // No other case allowed
    // Legally received a 'RelaxingBegin' signal
    PU->StateFlag_Set(gcsm_Relaxing);
        PU->EVENT_GenComp.RelaxingBegin.notify(SC_ZERO_TIME);
}

    void GenCompStates_Abstract::
Initialize(scGenComp_PU_Abstract* PU)
{
    PU-> StateFlag_Set(gcsm_Ready);
    PU->DoInitialize();
}

/*
 *  The machine PU received new input, administer it
 *  Input can be received only in 'Ready' and 'Processing' states
 */
void GenCompStates_Abstract::
    InputReceive(scGenComp_PU_Abstract* PU)
{
    if(gcsm_Processing != PU->StateFlag_Get())
    {
        DEBUG_SC_WARNING("Unit received input in state '" << GenCompStatesString[T] << "' ; neglected");
        return; // Neglect it
    }
    PU->DoInputReceive();
}


void GenCompStates_Abstract::
    Synchronize(scGenComp_PU_Abstract* PU)
{
 //   State_Set(machine, new ReadyGenCompState(name()));
//    machine.Synchronize();   //Must be implemented in scGenComp_PU_Abstract subclasses
}

//Can happen only in Processing state; passes to Relaxing state
    void GenCompStates_Abstract::
Fail(scGenComp_PU_Abstract* PU)
{
    if(gcsm_Processing== PU->StateFlag_Get())
        PU->Fail();
    // Otherwise neglect it

}
// Overload if want to use "dormant" state
void GenCompStates_Abstract::
    Wakeup(scGenComp_PU_Abstract *PU)
{
    //   State_Set(machine, new ReadyGenCompState(name()));
    //   machine.WakeUp();
}


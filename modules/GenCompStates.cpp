/** @file GenCompStates.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/
// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
//#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

#include "scGenComp_PU_Abstract.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing

string GenCompStates[]{"Sleeping", "Ready", "Processing", "Delivering", "Relaxing"};

// The units of general computing work in the same way, using general events
// The XXX_method() is activated by the event; XXX makes the activity, if the stae is OK

AbstractGenCompState::
    AbstractGenCompState(void)
{
}
AbstractGenCompState::
    ~AbstractGenCompState(void)
{
}

// Overload if want to use "dormant" state
   void AbstractGenCompState::
Wakeup(scGenComp_PU_Abstract *PU)
{
 //   State_Set(machine, new ReadyGenCompState(name()));
 //   machine.WakeUp();
}

/*
 *  Called twice:
 *  1st time, the system must be in state 'Processing',
 *  2nd time in state 'Delivering'
 */
    void AbstractGenCompState::
Deliver(scGenComp_PU_Abstract* PU)
{
    if((gcsm_Processing == PU->StateFlag_Get()) || (gcsm_Delivering== PU->StateFlag_Get()))
        PU->Deliver();
    else assert(0);
 }

// Put the PU electronics to low-power mode
// Must come from 'Ready' state; otherwise fail
    void AbstractGenCompState::
Sleep(scGenComp_PU_Abstract* PU)
{
    assert(gcsm_Sleeping != PU->StateFlag_Get());
    PU->StateFlag_Set(gcsm_Sleeping);
}

    void AbstractGenCompState::
Process(scGenComp_PU_Abstract* PU)
{
    if(gcsm_Ready == PU->StateFlag_Get())
    {   // Legally received a 'Begin computing' signal
        PU->StateFlag_Set(gcsm_Processing);
    }
    else if (gcsm_Processing == PU->StateFlag_Get())
    {   // Legally received an 'End computing' signal
        PU->StateFlag_Set(gcsm_Delivering);
    }
    else assert(0); // No other case allowed
}

    void AbstractGenCompState::
Relax(scGenComp_PU_Abstract* PU)
{
 //   State_Set(machine, new RelaxingGenCompState(name()));
 //   machine.Relax();  //Must be implemented in scGenComp_PU_Abstract subclasses
}
// Although a method thread, is invoked from the constructor
    void AbstractGenCompState::
Initialize(scGenComp_PU_Abstract* PU)
{
    PU-> StateFlag_Set(gcsm_Ready);
}

/*
 *  The machine PU received new input, administer it
 *  Input can be received only in 'Ready' and 'Processing' states
 */
void AbstractGenCompState::
    InputReceived(scGenComp_PU_Abstract* PU)
{
    DEBUG_SC_EVENT("   >>>",sc_time_stamp(),"");
    if((gcsm_Ready == PU->StateFlag_Get()) || (gcsm_Processing== PU->StateFlag_Get()))
        PU->ReceiveInput();
    // Otherwise neglect it
}


void AbstractGenCompState::
    Synchronize(scGenComp_PU_Abstract* PU)
{
 //   State_Set(machine, new ReadyGenCompState(name()));
//    machine.Synchronize();   //Must be implemented in scGenComp_PU_Abstract subclasses
}

//Can happen only in Processing state; passes to Relaxing state
    void AbstractGenCompState::
Fail(scGenComp_PU_Abstract* PU)
{
    if(gcsm_Processing== PU->StateFlag_Get())
        PU->Fail();
    // Otherwise neglect it

}
/*
    void AbstractGenCompState::
State_Set(scGenComp_PU_Abstract* PU, GenCompStateMachineType_t& State)
{
    PU-> StateFlag_Set(State);
}
*/

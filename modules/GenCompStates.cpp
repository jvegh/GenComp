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

#include "scAbstractGenComp_PU.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing

// The units of general computing work in the same way, using general events

AbstractGenCompState::
    AbstractGenCompState(void)
{
 //   SC_METHOD(Reinitialize_method);
 //   sensitive << EVENT_GenComp.GenComp_ReInitialize;
    //!! dont_initialize(); Should be initialized!!
}
AbstractGenCompState::
    ~AbstractGenCompState(void)
{
}

// Overload if want to use "dormant" state
   void AbstractGenCompState::
WakeUp(void)
{
 //   State_Set(machine, new ReadyGenCompState(name()));
 //   machine.WakeUp();
}

    void AbstractGenCompState::
Deliver(void)
{
 //   State_Set(machine, new DeliveringGenCompState(name()));
 //   machine.Deliver();   //Must be implemented in AbstractGenComp_PU subclasses
}

// Put the PU electronics to low-power mode
    void AbstractGenCompState::
Sleep(void)
{
 //   State_Set(machine, new DormantGenCompState(name()));
 }

    void AbstractGenCompState::
Process()
{
//    State_Set(machine, new ProcessingGenCompState(name()));
}

    void AbstractGenCompState::
Relax()
{
 //   State_Set(machine, new RelaxingGenCompState(name()));
 //   machine.Relax();  //Must be implemented in AbstractGenComp_PU subclasses
}
// Although a method thread, is invoked from the constructor
    void AbstractGenCompState::
Initialize(void)
{
 //   State_Set(machine, new ReadyGenCompState(name()));
 //   machine->Reinitialize();  //Must be implemented in AbstractGenComp_PU subclasses
//    wait(10,SC_NS);
 //   next_trigger(EVENT_GenComp.GenComp_ReInitialize);
}


    void AbstractGenCompState::
Synchronize()
{
 //   State_Set(machine, new ReadyGenCompState(name()));
//    machine.Synchronize();   //Must be implemented in AbstractGenComp_PU subclasses
}

    void AbstractGenCompState::
Fail()
{
//    State_Set(machine, new FailedGenCompState(name()));
}

    void AbstractGenCompState::
State_Set(scAbstractGenComp_PU* PU, GenCompStateMachineType_t& State)
{
    PU-> StateFlag = State;
 }

ReadyGenCompState::
    ReadyGenCompState(void):
    AbstractGenCompState()
{  }

    ReadyGenCompState::
~ReadyGenCompState(){}

    DormantGenCompState::
DormantGenCompState(void):
        AbstractGenCompState()
{  }

    DormantGenCompState::
~DormantGenCompState(){}

    void DormantGenCompState::
Process(void)
    {
    assert(0);     // Process signal while sleeping
    // wake it up first
    }


     void ReadyGenCompState::
Process(void)
 {
  //  State_Set(machine, new ProcessingGenCompState(name()));
    // Do some processing
 }

    void ReadyGenCompState::
Deliver(void)
 {
 //    State_Set(machine, new DeliveringGenCompState(name()));
//     machine.Deliver();   //Must be implemented in AbstractGenComp_PU subclasses
 }

 void ReadyGenCompState::
     Relax()
 {
  //   State_Set(machine, new RelaxingGenCompState(name()));
 //   machine.Relax();   //Must be implemented in AbstractGenComp_PU subclasses
 }


void ReadyGenCompState::
    Reinitialize()
{
 //    State_Set(machine, new RelaxingGenCompState(name()));
 //    machine.Reinitialize();   //Must be implemented in AbstractGenComp_PU subclasses
}


void ReadyGenCompState::
    Synchronize()
{
}


DeliveringGenCompState::
    DeliveringGenCompState(void):
    AbstractGenCompState()
{
 }


DeliveringGenCompState::
    ~DeliveringGenCompState(){}

ProcessingGenCompState::
    ProcessingGenCompState(void):
    AbstractGenCompState()
{ }

void ProcessingGenCompState::
    Process()
{
     assert(0);     // Process signal during processing
}

ProcessingGenCompState::
    ~ProcessingGenCompState(){}

RelaxingGenCompState::
    RelaxingGenCompState(void):
    AbstractGenCompState()
{   }

RelaxingGenCompState::
    ~RelaxingGenCompState(){}

FailedGenCompState::
    FailedGenCompState(void):
    AbstractGenCompState()
{
    //flag = gcsm_Ready;
}

FailedGenCompState::
    ~FailedGenCompState(){}

void FailedGenCompState::
    Process(void)
{
     assert(0);     // Process signal during processing
}

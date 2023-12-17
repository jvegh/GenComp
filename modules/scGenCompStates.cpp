/** @file scGenCompStates.cpp
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
    ~AbstractGenCompState(void)
{
}

// Overload if want to use "dormant" state
   void AbstractGenCompState::
WakeUp(AbstractGenComp_PU& machine)
{
    State_Set(machine, new ReadyGenCompState());
    machine.WakeUp();
}

    void AbstractGenCompState::
Deliver(AbstractGenComp_PU& machine)
{
    State_Set(machine, new DeliveringGenCompState());
    machine.Deliver();   //Must be implemented in AbstractGenComp_PU subclasses
}

// Put the PU electronics to low-power mode
    void AbstractGenCompState::
Sleep(AbstractGenComp_PU& machine)
{
    State_Set(machine, new DormantGenCompState());
    machine.Sleep();
}

    void AbstractGenCompState::
Process(AbstractGenComp_PU& machine)
{
    State_Set(machine, new ProcessingGenCompState());
    machine.Process();   //Must be implemented in AbstractGenComp_PU subclasses
}

    void AbstractGenCompState::
Relax(AbstractGenComp_PU& machine)
{
    State_Set(machine, new RelaxingGenCompState());
    machine.Relax();  //Must be implemented in AbstractGenComp_PU subclasses
}
    void AbstractGenCompState::
Reinitialize(AbstractGenComp_PU& machine)
{
    State_Set(machine, new ReadyGenCompState());
    machine.Reinitialize();  //Must be implemented in AbstractGenComp_PU subclasses
}

    void  AbstractGenCompState::
HeartBeat(AbstractGenComp_PU& machine)
{
    machine.HeartBeat();    //Must be implemented in AbstractGenComp_PU subclasses
}

    void AbstractGenCompState::
Synchronize(AbstractGenComp_PU& machine)
{
    State_Set(machine, new ReadyGenCompState());
    machine.Synchronize();   //Must be implemented in AbstractGenComp_PU subclasses
}

void AbstractGenCompState::
    Fail(AbstractGenComp_PU& machine)
{
    State_Set(machine, new FailedGenCompState());
    machine.Fail();   //Must be implemented in AbstractGenComp_PU subclasses
}

    void AbstractGenCompState::
State_Set(AbstractGenComp_PU& PU, AbstractGenCompState* state)
{
    AbstractGenCompState *aux = PU.state;
    PU.state = state;
    delete aux;
}

ReadyGenCompState::
    ReadyGenCompState()
{  flag = gcsm_Ready;}

    ReadyGenCompState::
~ReadyGenCompState(){}

    DormantGenCompState::
DormantGenCompState()
{  flag = gcsm_Dormant;}

    DormantGenCompState::
~DormantGenCompState(){}

    void DormantGenCompState::
Process(AbstractGenComp_PU& machine)
    {
    assert(0);     // Process signal while sleeping
    // wake it up first
    }


     void ReadyGenCompState::
Process(AbstractGenComp_PU& machine)
 {
     State_Set(machine, new ProcessingGenCompState());
    // Do some processing
     machine.Process();
 }

    void ReadyGenCompState::
Deliver(AbstractGenComp_PU& machine)
 {
     State_Set(machine, new DeliveringGenCompState());
     machine.Deliver();   //Must be implemented in AbstractGenComp_PU subclasses
 }

 void ReadyGenCompState::
     Relax(AbstractGenComp_PU& machine)
 {
     State_Set(machine, new RelaxingGenCompState());
     machine.Relax();   //Must be implemented in AbstractGenComp_PU subclasses
 }

void ReadyGenCompState::
    Reinitialize(AbstractGenComp_PU& machine)
{
     State_Set(machine, new RelaxingGenCompState());
     machine.Reinitialize();   //Must be implemented in AbstractGenComp_PU subclasses
}

void ReadyGenCompState::
    Synchronize(AbstractGenComp_PU& machine)
{
     State_Set(machine, new RelaxingGenCompState());
     machine.Relax();   //Must be implemented in AbstractGenComp_PU subclasses
}

void ReadyGenCompState::
    HeartBeat(AbstractGenComp_PU& machine)
{
}


DeliveringGenCompState::
    DeliveringGenCompState()
{
     flag = gcsm_Delivering;
}


DeliveringGenCompState::
    ~DeliveringGenCompState(){}

void DeliveringGenCompState::
    HeartBeat(AbstractGenComp_PU& machine)
{
}


ProcessingGenCompState::
    ProcessingGenCompState()
{ flag = gcsm_Processing;}

void ProcessingGenCompState::
    Process(AbstractGenComp_PU& machine)
{
     assert(0);     // Process signal during processing
}

ProcessingGenCompState::
    ~ProcessingGenCompState(){}

RelaxingGenCompState::
    RelaxingGenCompState()
{   flag = gcsm_Relaxing;}

RelaxingGenCompState::
    ~RelaxingGenCompState(){}

FailedGenCompState::
    FailedGenCompState()
{
    flag = gcsm_Ready;
}

FailedGenCompState::
    ~FailedGenCompState(){}

void FailedGenCompState::
    Process(AbstractGenComp_PU& machine)
{
     assert(0);     // Process signal during processing
}

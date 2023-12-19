/** @file scAbstractGenComp_PU.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scAbstractGenComp_PU.h"
// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
//#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing

static AbstractGenCompState TheAbstractGenCompState = AbstractGenCompState();
// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing

    scAbstractGenComp_PU::
scAbstractGenComp_PU(sc_core::sc_module_name nm): sc_core::sc_module( nm)
    ,MachineState (&TheAbstractGenCompState)
    ,StateFlag(gcsm_Ready)
{
    typedef scAbstractGenComp_PU SC_CURRENT_USER_MODULE;
    // Intialize the module with generating an event
    SC_METHOD(Initialize_method);
    sensitive << EVENT_GenComp.Initialize;
    dont_initialize();
    EVENT_GenComp.Initialize.notify(SC_ZERO_TIME);
    /*
    // Operating
    SC_METHOD(Process_method);
    sensitive << EVENT_GenComp.Process;
    SC_METHOD(Deliver_method);
    dont_initialize();
    sensitive << EVENT_GenComp.Deliver;
    SC_METHOD(Heartbeat_method);
    sensitive << EVENT_GenComp.HeartBeat;
    dont_initialize();
    SC_METHOD(Relax_method);
    dont_initialize();
    // Power handling
 */   SC_METHOD(Sleep_method);
    sensitive << EVENT_GenComp.Sleep;
    dont_initialize();
     /*
    SC_METHOD(Wakeup_method);
    sensitive << EVENT_GenComp.Wakeup;
    dont_initialize();
    SC_METHOD(Fail_method);
    sensitive << EVENT_GenComp.Fail;
    dont_initialize();
    SC_METHOD(Synchronize_method);
    sensitive << EVENT_GenComp.Synchronize;
    dont_initialize();
*/
 }
scAbstractGenComp_PU::
~scAbstractGenComp_PU(void)
{
}

// Puts the PU in its default state
void scAbstractGenComp_PU::
    Initialize_method(void)
{
    MachineState->Initialize(this);   // Change status to 'Initial'
    Initialize(); // Initialize the unit, HW and temporary variables
    // Put PU in its default state
}

void scAbstractGenComp_PU::
    Process_method(void)
{
    //    MachineState->Sleep();   // Change status to 'Initial'
}

void scAbstractGenComp_PU::
    Deliver_method(void)
{
    //    MachineState->Sleep();   // Change status to 'Initial'
}


void scAbstractGenComp_PU::
   Relax_method(void)
{
    //    MachineState->Sleep();   // Change status to 'Initial'
}

void scAbstractGenComp_PU::
    Synchronize_method(void)
{
    //    MachineState->Sleep();   // Change status to 'Initial'
}

void scAbstractGenComp_PU::
    Heartbeat_method(void)
{
    //    MachineState->Sleep();   // Change status to 'Initial'
}


void scAbstractGenComp_PU::
    Sleep_method(void)
{
    MachineState->Sleep(this);   // Change status to 'Ready'
}

void scAbstractGenComp_PU::
    Wakeup_method(void)
{
    MachineState->Wakeup(this);   // Change status to 'Ready'
}

void scAbstractGenComp_PU::
    Fail_method(void)
{
    MachineState->Fail();   // Change status to 'Initial'
}


void scAbstractGenComp_PU::
    Initialize(void)
{
}

    scBioGenComp_PU::
scBioGenComp_PU(sc_core::sc_module_name nm):
    scAbstractGenComp_PU(nm)
{
}


    scBioGenComp_PU::
~scBioGenComp_PU(void)
{
}

    void scBioGenComp_PU::
Process_method(void)
{
 //   int32_t i = State_Get()->Flag_Get();
}

    scTechGenComp_PU::
scTechGenComp_PU(sc_core::sc_module_name nm, int32_t No):
    scAbstractGenComp_PU(nm),
    mNoOfArgs(No)
{
}

scTechGenComp_PU::
    ~scTechGenComp_PU(void)
{
}

void scTechGenComp_PU::
    Process_method(void)
{
 //   int32_t i = State_Get()->Flag_Get();
}

/** @file scAbstractGenComp_PU.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

extern bool UNIT_TESTING;	// Whether in course of unit testing; considered in unit testing

// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug event for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"
#include "scAbstractGenComp_PU.h"

// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing

AbstractGenCompState* TheAbstractGenCompState;
    scAbstractGenComp_PU::
scAbstractGenComp_PU(sc_core::sc_module_name nm): sc_core::sc_module( nm)
    ,StateFlag(gcsm_Ready)
{
        if(!TheAbstractGenCompState)
            TheAbstractGenCompState = new AbstractGenCompState();
        MachineState = TheAbstractGenCompState;
    typedef scAbstractGenComp_PU SC_CURRENT_USER_MODULE;
    // Intialize the module with generating an event
    SC_METHOD(Initialize_method);
    sensitive << EVENT_GenComp.Initialize;
//    dont_initialize(); //Simply execute initialization, without an event
//    EVENT_GenComp.Initialize.notify(SC_ZERO_TIME);
    SC_METHOD(InputReceived_method);
    sensitive << EVENT_GenComp.InputReceived;
    dont_initialize();
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
*/
#ifdef USE_PU_HWSLEEPING
    // Power handling
    SC_METHOD(Sleep_method);
    sensitive << EVENT_GenComp.Sleep;
    dont_initialize();  
    SC_METHOD(Wakeup_method);
    sensitive << EVENT_GenComp.Wakeup;
    dont_initialize();
#endif// USE_PU_HWSLEEPING

/*    SC_METHOD(Fail_method);
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
    DEBUG_SC_EVENT("Intializing");
    DEBUG_SC_PRINT("Enter EVENT_GenComp.Initialize");
    MachineState->Initialize(this);   // Change status to 'Initial'
    Initialize(); // Initialize the unit, HW and temporary variables
    // Put PU in its default state
                     DEBUG_PRINT_SC("Exit  EVENT_GenComp.Initialize");
}


void scAbstractGenComp_PU::
    InputReceived_method(void)
{
                     DEBUG_PRINT_SC("Enter EVENT_GenComp.InputReceived");
    MachineState->InputReceived(this);
                     DEBUG_PRINT_SC("Exit  EVENT_GenComp.InputReceived");
}

// This routine make actual input processing, although most o fthe job is done in Process nad Heartbeat
void scAbstractGenComp_PU::
    ReceiveInput(void)
{

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

#ifdef USE_PU_HWSLEEPING
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
    Wakeup(void)
{
// Make actual waking-up
// Ends with issuing event 'Awaken'
}
void scAbstractGenComp_PU::
    Clock_method(void)
{
}
#endif // USE_PU_HWSLEEPING
void scAbstractGenComp_PU::
    Fail_method(void)
{
    MachineState->Fail(this);   // Change status to 'Initial'
}
void scAbstractGenComp_PU::
    Fail(void)
{
}


void scAbstractGenComp_PU::
    Initialize(void)
{
}


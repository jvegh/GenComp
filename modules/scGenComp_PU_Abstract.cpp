/** @file scGenComp_PU_Abstract.cpp
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
#include "scGenComp_PU_Abstract.h"

// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing

AbstractGenCompState* TheAbstractGenCompState;
    scGenComp_PU_Abstract::
scGenComp_PU_Abstract(sc_core::sc_module_name nm): sc_core::sc_module( nm)
    ,mStateFlag(gcsm_Ready)
    ,mLocalTimeBase(sc_core::SC_ZERO_TIME)
    ,mGenCompPUOperatingBits(gcob_ObserveModule)
{
        if(!TheAbstractGenCompState)
            TheAbstractGenCompState = new AbstractGenCompState();
        MachineState = TheAbstractGenCompState;
    typedef scGenComp_PU_Abstract SC_CURRENT_USER_MODULE;
    // Intialize the module with generating an event
    SC_METHOD(Initialize_method);
    sensitive << EVENT_GenComp.Initialize;
    dont_initialize(); //Simply execute initialization, without an event
    EVENT_GenComp.Initialize.notify(SC_ZERO_TIME);
    SC_METHOD(InputReceived_method);
    sensitive << EVENT_GenComp.InputReceived;
    dont_initialize();
    // Operating
    SC_METHOD(Relax_method);
    sensitive << EVENT_GenComp.Relax;
    dont_initialize();
#ifdef USE_PU_HWSLEEPING
    // Power handling
    SC_METHOD(Sleep_method);
    sensitive << EVENT_GenComp.Sleep;
    dont_initialize();  
    SC_METHOD(Wakeup_method);
    sensitive << EVENT_GenComp.Wakeup;
    dont_initialize();
#endif// USE_PU_HWSLEEPING

    SC_METHOD(Fail_method);
    sensitive << EVENT_GenComp.Fail;
    dont_initialize();
    SC_METHOD(Synchronize_method);
    sensitive << EVENT_GenComp.Synchronize;
    dont_initialize();
 }
scGenComp_PU_Abstract::
~scGenComp_PU_Abstract(void)
{
}

// Puts the PU in its default state
void scGenComp_PU_Abstract::
    Initialize_method(void)
{
            DEBUG_SC_EVENT_LOCAL(">>>   ");
    MachineState->Initialize(this);   // Change status to 'Ready'
    Initialize(); // Initialize the unit, HW and temporary variables
    // Put PU in its default state
            DEBUG_SC_EVENT_LOCAL("<<<   ");
}


void scGenComp_PU_Abstract::
    InputReceived_method(void)
{
    if(OperatingBit_Get(gcob_ObserveModule) && OperatingBit_Get(gcob_ObserveInput))
        DEBUG_SC_PRINT_LOCAL ("Input observed");
    DEBUG_SC_EVENT_LOCAL(">>>   ");
    MachineState->InputReceived(this);
    // The input is legal, continue receiving it
    ReceiveInput();
    DEBUG_SC_EVENT_LOCAL("<<<   ");
}

// This routine makes actual input processing, although most of the job is done in Process and Heartbeat
void scGenComp_PU_Abstract::
    ReceiveInput(void)
{
            DEBUG_SC_EVENT_LOCAL("   >>>");
    Inputs.push_back(NoOfInputsReceived_Get());
}


void scGenComp_PU_Abstract::
    Deliver()
{
    DEBUG_SC_EVENT_LOCAL("   >>>");
    if(gcsm_Processing == StateFlag_Get())
    {   // We are at the end of Processing phase, the phase 'Delivering' follows
            DEBUG_SC_EVENT_LOCAL("   >>>");
            StateFlag_Set(gcsm_Delivering);    // Now delivering
    }
    else
    {   // We are at the end of phase 'Delivering'
//            next_trigger(EVENT_GenComp.Relax);
            DEBUG_SC_EVENT_LOCAL("   >>>");
            DEBUG_SC_EVENT_LOCAL("SENT EVENT_GenComp.Relax");
            EVENT_GenComp.Relax.notify(SC_ZERO_TIME);
    }
    DEBUG_SC_EVENT_LOCAL("   <<<");
}

void scGenComp_PU_Abstract::
   Relax_method(void)
{
    //    MachineState->Sleep();   // Change status to 'Initial'
}

void scGenComp_PU_Abstract::
    Synchronize_method(void)
{
    //    MachineState->Sleep();   // Change status to 'Initial'
}


#ifdef USE_PU_HWSLEEPING
void scGenComp_PU_Abstract::
    Sleep_method(void)
{
    MachineState->Sleep(this);   // Change status to 'Ready'
}

void scGenComp_PU_Abstract::
    Wakeup_method(void)
{
    MachineState->Wakeup(this);   // Change status to 'Ready'
}
void scGenComp_PU_Abstract::
    Wakeup(void)
{
// Make actual waking-up
// Ends with issuing event 'Awaken'
}
void scGenComp_PU_Abstract::
    Clock_method(void)
{
}
#endif // USE_PU_HWSLEEPING
void scGenComp_PU_Abstract::
    Fail_method(void)
{
    DEBUG_SC_EVENT_LOCAL(">>>   ");
    MachineState->Fail(this);   // Change status to 'Initial'
    DEBUG_SC_EVENT_LOCAL("<<<   ");
}
void scGenComp_PU_Abstract::
    Fail(void)
{
    DEBUG_SC_EVENT_LOCAL("   ---");
}

/*
 * Initialize the GenComp unit
 */
void scGenComp_PU_Abstract::
    Initialize(void)
{
    Inputs.clear();
 }

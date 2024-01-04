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

GenCompStates_Abstract* TheGenCompStates_Abstract;
    scGenComp_PU_Abstract::
scGenComp_PU_Abstract(sc_core::sc_module_name nm): sc_core::sc_module( nm)
    ,mStateFlag(gcsm_Ready)
    ,mGenCompPUOperatingBits(gcob_ObserveModule)
 {
        if(!TheGenCompStates_Abstract)
            TheGenCompStates_Abstract = new GenCompStates_Abstract();
        MachineState = TheGenCompStates_Abstract;
    typedef scGenComp_PU_Abstract SC_CURRENT_USER_MODULE;
    // Intialize the module with generating an event
    SC_METHOD(Initialize_method);
    sensitive << EVENT_GenComp.Initialize;
    dont_initialize();
        // Operating
    SC_METHOD(InputReceived_method);
    sensitive << EVENT_GenComp.InputReceived;
    dont_initialize();
    SC_METHOD(ProcessingBegin_method);
    sensitive << EVENT_GenComp.ProcessingBegin;
    dont_initialize();
    SC_METHOD(ProcessingEnd_method);
    sensitive << EVENT_GenComp.ProcessingEnd;
    dont_initialize();
    SC_METHOD(DeliveringBegin_method);
    sensitive << EVENT_GenComp.DeliveringBegin;
    dont_initialize();
    SC_METHOD(DeliveringEnd_method);
    sensitive << EVENT_GenComp.DeliveringEnd;
    dont_initialize();
    SC_METHOD(RelaxingBegin_method);
    sensitive << EVENT_GenComp.RelaxingBegin;
    dont_initialize();
    SC_METHOD(RelaxingEnd_method);
    sensitive << EVENT_GenComp.RelaxingEnd;
    dont_initialize();
    SC_METHOD(Heartbeat_method);
    sensitive << EVENT_GenComp.Heartbeat;
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
    MachineState->Initialize(this);   // Change status to 'Ready' and call DoInitialize()
    DEBUG_SC_EVENT_LOCAL("<<<   ");
}

/*
 * Initialize the GenComp unit. Usually called by the state machine
 */
void scGenComp_PU_Abstract::
    DoInitialize(void)
{
    DEBUG_SC_EVENT_LOCAL(">>>   ");
    mLocalTimeBase = sc_time_stamp();
    Inputs.clear();
    DEBUG_SC_EVENT_LOCAL("<<<   ");
}



// The physical delivery
void scGenComp_PU_Abstract::
    Deliver()
{
    DEBUG_SC_EVENT_LOCAL("   >>>");
    if(gcsm_Processing == StateFlag_Get())
    {   // We are at the end of Processing phase, the phase 'Delivering' follows
        StateFlag_Set(gcsm_Delivering);    // Now delivering
    }
    else
    {   // We are at the end of phase 'Delivering'
        DEBUG_SC_EVENT_LOCAL("SENT EVENT_GenComp.Relax");
        //??           EVENT_GenComp.Relax.notify(SC_ZERO_TIME);
    }
    DEBUG_SC_EVENT_LOCAL("   <<<");
}

// Called when the state 'processing' begins
void scGenComp_PU_Abstract::
    DeliveringBegin_method()
{
    DEBUG_SC_EVENT_LOCAL("Delivering started");
}


// Called when the state 'processing' ends
void scGenComp_PU_Abstract::
    DeliveringEnd_method()
{
    DEBUG_SC_EVENT_LOCAL("Delivering finished");
    MachineState->Relax(this);    // Pass to "Relaxing"
}


// Called when the state 'processing' ends
void scGenComp_PU_Abstract::
    Heartbeat_method()
{
    if(OperatingBit_Get(gcob_ObserveModule) && OperatingBit_Get(gcob_ObserveHeartbeat))
        DEBUG_SC_PRINT_LOCAL ("Heartbeat observed");
    DEBUG_SC_EVENT_LOCAL("In state '" << GenCompStatesString[mStateFlag]);

    switch(StateFlag_Get())
    {
    case gcsm_Ready: Heartbeat_Ready(); break;
    case gcsm_Processing: Heartbeat_Processing(); break;
    case gcsm_Delivering: Heartbeat_Delivering(); break;
    case gcsm_Relaxing: Heartbeat_Relaxing(); break;
    default: ; assert(0); break; // do nothing
    }
}



void scGenComp_PU_Abstract::
    InputReceived_method(void)
{
    DEBUG_SC_EVENT("SENT EVENT_GenComp.InputReceived");
    if(OperatingBit_Get(gcob_ObserveModule) && OperatingBit_Get(gcob_ObserveInput))
        DEBUG_SC_PRINT_LOCAL ("Input observed");
    // The input is legal, continue receiving it
    DoInputReceive();

}

// This routine makes actual input processing, although most of the job is done in Process and Heartbeat
void scGenComp_PU_Abstract::
    DoInputReceive(void)
{
     if(OperatingBit_Get(gcob_ObserveModule) && OperatingBit_Get(gcob_ObserveInput))
            DEBUG_SC_PRINT_LOCAL ("Input observed");

     DEBUG_SC_EVENT_LOCAL("Received input #" << NoOfInputsReceived_Get());
     DEBUG_SC_EVENT("Received input #" << NoOfInputsReceived_Get());
    Inputs.push_back(NoOfInputsReceived_Get());
}


// Called when the state 'processing' begins
void scGenComp_PU_Abstract::
    ProcessingBegin_method()
{
    ProcessingBegin();
}

void scGenComp_PU_Abstract::
    ProcessingBegin()
{
    scLocalTime_Set(sc_time_stamp());      // The clock is synchronized to the beginning of processing
    Inputs.clear();
    DEBUG_SC_EVENT_LOCAL("Processing started");
}
/*
PU->EVENT_GenComp.Heartbeat.notify(SC_ZERO_TIME);
DEBUG_SC_PRINT("SENT EVENT_GenComp.Heartbeat");
PU->EVENT_GenComp.ProcessingBegin.notify(SC_ZERO_TIME);
DEBUG_SC_PRINT("SENT EVENT_GenComp.ProcessingBegin");
*/
// Called when the state 'processing' ends
void scGenComp_PU_Abstract::
    ProcessingEnd_method()
{
    ProcessingEnd();
}

void scGenComp_PU_Abstract::
    ProcessingEnd()
{
    DEBUG_SC_EVENT_LOCAL("Processing finished");
    MachineState->Deliver(this);    // Pass to "Delivering
}


// Called when the state 'processing' begins
void scGenComp_PU_Abstract::
    RelaxingBegin_method()
{
    MachineState->Relax(this);    // Go to "Relaxsing"
    DEBUG_SC_EVENT_LOCAL("Relaxing started");
}

// Called when the state 'processing' ends
void scGenComp_PU_Abstract::
    RelaxingEnd_method()
{
    DEBUG_SC_EVENT_LOCAL("Relaxing finished");
    MachineState->Initialize(this);    // Pass to "Ready"
}


// After the result delivered internally, reset the processing unit
void scGenComp_PU_Abstract::
   Relax(void)
{
    DEBUG_SC_EVENT_LOCAL("   >>>");
    if(gcsm_Delivering == StateFlag_Get())
    {   // We are at the end of Delivering phase, the phase 'Relaxing' follows
        StateFlag_Set(gcsm_Relaxing);    // Now Relaxing
    }
    else
    {   // We are at the end of phase 'Relaxing'
        DEBUG_SC_EVENT_LOCAL("SENT EVENT_GenComp.Ready");
        EVENT_GenComp.Initialize.notify(SC_ZERO_TIME);
    }
    DEBUG_SC_EVENT_LOCAL("   <<<");
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
    assert(gcsm_Ready != StateFlag_Get());
    // Be sure we were in 'Ready'
    StateFlag_Set(gcsm_Sleeping);
}

void scGenComp_PU_Abstract::
    Wakeup_method(void)
{
    assert(gcsm_Sleeping != StateFlag_Get());
    EVENT_GenComp.Initialize.notify(SC_ZERO_TIME);
 }
void scGenComp_PU_Abstract::
    Wakeup(void)
{
// Make actual waking-up
// Ends with issuing event 'Awaken'
}

#endif // USE_PU_HWSLEEPING
void scGenComp_PU_Abstract::
    Fail_method(void)
{
    DEBUG_SC_EVENT_LOCAL(">>>   ");
    EVENT_GenComp.Initialize.notify(SC_ZERO_TIME);
    DEBUG_SC_EVENT_LOCAL("<<<   ");
}
void scGenComp_PU_Abstract::
    Fail(void)
{
    DEBUG_SC_EVENT_LOCAL("   ---");
}



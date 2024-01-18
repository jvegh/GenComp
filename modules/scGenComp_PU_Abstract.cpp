/** @file scGenComp_PU_Abstract.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

extern bool UNIT_TESTING;	// Whether in course of unit testing; considered in unit testing
/*
 *  If this macro is not defined, no code is generated;
        the variables, however, must be defined (although they will be
        optimized out as unused ones).
        Alternatively, the macros may be protected with "\#ifdef MAKE_TIME_BENCHMARKING".
        The macros have source-module scope. All variables must be passed by reference.
*/


// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug event for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"
#include "scGenComp_PU_Abstract.h"
#include "scGenComp_Simulator.h"

string GenCompObserveStrings[]{"Group","Module","BrocessingBegin","ProcessingEnd",
                               "Heartbeat","Input","Initialize"};

// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing

GenCompStates_Abstract* TheGenCompStates_Abstract;

    scGenComp_PU_Abstract::
scGenComp_PU_Abstract(sc_core::sc_module_name nm): sc_core::sc_module( nm)
    ,mStateFlag(gcsm_Ready)
    ,mHeartbeat(HEARTBEAT_TIME_DEFAULT)
    ,mHeartbeatDivisions(16)
    ,mLastProcessingTime(SC_ZERO_TIME) ///< Remember last time duration  (the result)
    ,mLastIdleTime(SC_ZERO_TIME) ///< Remember the beginning of the 'Idle' period
    ,mLastRelaxingEndTime(sc_core::sc_time_stamp())  ///< Remember the beginning of the 'Idle' period
{
    if(!TheGenCompStates_Abstract)
        TheGenCompStates_Abstract = new GenCompStates_Abstract();
    MachineState = TheGenCompStates_Abstract;
    mObservedBits[gcob_ObserveGroup] = true;   // Enable this module for its group observing by default
    mObservedBits[gcob_ObserveModule] = true;   // Enable module observing by default
    // The stuff below in the consructor are SystemC specific, do not touch!
    typedef scGenComp_PU_Abstract SC_CURRENT_USER_MODULE;

    // Operating
    SC_METHOD(Initialize_method);
    sensitive << EVENT_GenComp.Initialize;
    dont_initialize();
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

    SC_METHOD(Failed_method);
    sensitive << EVENT_GenComp.Failed;
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
    Initialize_Do();
}


/*
 * Initialize the GenComp unit. Usually called by the state machine
 */
void scGenComp_PU_Abstract::
    Initialize_Do(void)
{
                //DEBUG_SC_EVENT_LOCAL("");
    // The input is legal, continue receiving it
    ObserverNotify(gcob_ObserveInitialize);
    mLocalTimeBase = sc_time_stamp();
    Inputs.clear();
            DEBUG_SC_EVENT_LOCAL("Initialized for abstract mode");
}

// Cancel all possible events in the air
void scGenComp_PU_Abstract::
    CancelEvents(void)
{
    EVENT_GenComp.DeliveringBegin.cancel();
    EVENT_GenComp.DeliveringEnd.cancel();
    EVENT_GenComp.ProcessingBegin.cancel();
    EVENT_GenComp.ProcessingEnd.cancel();
    EVENT_GenComp.RelaxingBegin.cancel();
    EVENT_GenComp.RelaxingEnd.cancel();
    EVENT_GenComp.SleepingBegin.cancel();
    EVENT_GenComp.SleepingEnd.cancel();
    EVENT_GenComp.TransmittingBegin.cancel();
    EVENT_GenComp.TransmittingEnd.cancel();
    EVENT_GenComp.Failed.cancel();
    EVENT_GenComp.Heartbeat.cancel();
    EVENT_GenComp.InputReceived.cancel();
    EVENT_GenComp.Synchronize.cancel();
    EVENT_GenComp.Wakeup.cancel();
}


// Called when the state 'delivering' begins
void scGenComp_PU_Abstract::
    DeliveringBegin_method()
{
    ObserverNotify(gcob_ObserveDeliveringBegin);
                DEBUG_SC_EVENT_LOCAL("Delivering started");
}


// Called when the state 'processing' ends
void scGenComp_PU_Abstract::
    DeliveringEnd_method()
{
                DEBUG_SC_EVENT_LOCAL("Delivering finished");
//    MachineState->Relax(this);    // Pass to "Relaxing"
                ObserverNotify(gcob_ObserveDeliveringBegin);
}


// Called when the state 'processing' ends
void scGenComp_PU_Abstract::
    Heartbeat_method()
{
    ObserverNotify(gcob_ObserveHeartbeat);
    DEBUG_SC_EVENT_LOCAL("In state '" << GenCompStatesString[mStateFlag] << "'");

    switch(StateFlag_Get())
    {
        case gcsm_Ready:      Heartbeat_Ready_Do(); break;
        case gcsm_Processing: Heartbeat_Processing_Do(); break;
        case gcsm_Delivering: Heartbeat_Delivering_Do(); break;
        case gcsm_Relaxing:   Heartbeat_Relaxing_Do(); break;
        default: ; assert(0); break; // do nothing
    }
}



void scGenComp_PU_Abstract::
    InputReceived_method(void)
{
    InputReceived_Do();
}

// This routine makes actual input processing, although most of the job is done in Process and Heartbeat
void scGenComp_PU_Abstract::
    InputReceived_Do(void)
{
     Inputs.push_back(NoOfInputsReceived_Get());
}


// Called when the state 'processing' begins
void scGenComp_PU_Abstract::
    ProcessingBegin_method()
{
    ObserverNotify(gcob_ObserveProcessingBegin);
    // For statistic, remember the duration of 'Idle' state
    mLastIdleTime = sc_core::sc_time_stamp() - mLastRelaxingEndTime;
    StateFlag_Set(gcsm_Processing);
    scLocalTime_Set(sc_time_stamp());      // The clock is synchronized to the beginning of processing
    ProcessingBegin_Do();
    DEBUG_SC_EVENT_LOCAL("Processing started");
}

void scGenComp_PU_Abstract::
    ProcessingBegin_Do()
{
    Inputs.clear();
}

// Called when the state 'processing' ends
void scGenComp_PU_Abstract::
    ProcessingEnd_method()
{
    ProcessingEnd_Do();
    StateFlag_Set(gcsm_Delivering);   // Pass to "Delivering"
    mLastProcessingTime = sc_core::sc_time_stamp()-scLocalTime_Get();    // Remember when we were ready
    ObserverNotify(gcob_ObserveProcessingEnd);
                DEBUG_SC_EVENT_LOCAL("Processing finished");
    EVENT_GenComp.DeliveringBegin.notify(SC_ZERO_TIME);
                DEBUG_SC_EVENT_LOCAL("SENT EVENT_GenComp.Ready");
}

void scGenComp_PU_Abstract::
    ProcessingEnd_Do()
{
}


// Called when the state 'Relaxing' begins
void scGenComp_PU_Abstract::
    RelaxingBegin_method()
{
    MachineState->Relax(this);    // Go to "Relaxing"
                DEBUG_SC_EVENT_LOCAL("Relaxing started");
}



/**
     * @brief Relaxing has finished
     *
     * Usually activated by    EVENT_GenComp.RelaxingEnd,            // End restoring the 'Ready' state
     * After delivered the result internally to the 'output section', resetting begins
     */
void scGenComp_PU_Abstract::
    RelaxingEnd_method()
{
    RelaxingEnd_Do();
    mLastRelaxingEndTime = sc_core::sc_time_stamp();    // Remember when we were ready
    mLastProcessingTime = sc_core::sc_time_stamp()-scLocalTime_Get();    // Remember when we were ready
                DEBUG_SC_EVENT_LOCAL("Relaxing finished");
}

void scGenComp_PU_Abstract::
    RelaxingEnd_Do()
{
}


/* After the result delivered internally, reset the processing unit
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
}*/

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
    Failed_method(void)
{
    DEBUG_SC_EVENT_LOCAL(">>>   ");
    EVENT_GenComp.Initialize.notify(SC_ZERO_TIME);
    DEBUG_SC_EVENT_LOCAL("<<<   ");
}
void scGenComp_PU_Abstract::
    Failed_Do()
{
    DEBUG_SC_EVENT_LOCAL("   ---");
}

void scGenComp_PU_Abstract::
    RegisterSimulator(scGenComp_Simulator* Observer)
{
    MySimulator = Observer;
}

void scGenComp_PU_Abstract::
    ObserverNotify(GenCompPUObservingBits_t  ObservedBit)
{
    if(!MySimulator) return; // Module not registered
    if(ObservingBit_Get(ObservedBit))
        MySimulator->Observe(this,ObservedBit);
}


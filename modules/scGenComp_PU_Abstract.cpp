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
string GenCompStatesString[]{"Sleeping", "Ready", "Processing", "Delivering", "Relaxing"};

    scGenComp_PU_Abstract::
scGenComp_PU_Abstract(sc_core::sc_module_name nm): sc_core::sc_module( nm)
    ,mHeartbeatDivisions(8)                         ///< Reduce update frequency
    ,mLocalTimeBase(sc_core::sc_time_stamp())       ///< Remember the beginning of this computation
    ,mHeartbeat(HEARTBEAT_TIME_DEFAULT)             ///< Remember heartbeat time
    ,mLastOperatingTime(sc_core::sc_time_stamp())   ///< Remember last time duration  (the result)
    ,mLastRelaxingEndTime(sc_core::sc_time_stamp()) ///< Remember the beginning of the 'Idle' period
    ,mLastIdleTime(SC_ZERO_TIME)                    ///< Remember the beginning of the 'Idle' period
    ,mLastTransmissionTime(sc_core::sc_time_stamp())///< Remember last spiking time
{
    mObservedBits[gcob_ObserveGroup] = true;   // Enable this module for its group observing by default
    mObservedBits[gcob_ObserveModule] = true;   // Enable module observing by default

    // *** The stuff below in the constructor are SystemC specific, do not touch!
    typedef scGenComp_PU_Abstract SC_CURRENT_USER_MODULE;
    Initialize_method();    // initialize without issuing EVENT_GenComp.Initialize

    // Operating
    SC_METHOD(DeliveringBegin_method);
    sensitive << EVENT_GenComp.DeliveringBegin;
    dont_initialize();
    SC_METHOD(DeliveringEnd_method);
    sensitive << EVENT_GenComp.DeliveringEnd;
    dont_initialize();
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
    SC_METHOD(Ready_method);
    sensitive << EVENT_GenComp.Ready;
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
// Usually triggered by EVENT_GenComp.Initialize
// but called from the contructor, too
    void scGenComp_PU_Abstract::
    Initialize_method(void)
{
    ObserverNotify(gcob_ObserveInitialize);
    StateFlag_Set(gcsm_Ready);   // Pass to "Ready"
    mLocalTimeBase = sc_time_stamp();
    mOperationCounter = 0;    // Count the actions the unit makes
    Initialize_Do();
            DEBUG_SC_EVENT_LOCAL("---Initialized");
}


/*
 * Initialize the GenComp PU.
 */
void scGenComp_PU_Abstract::
    Initialize_Do(void)
{
     Inputs.clear();
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
    assert(StateFlag_Get() == gcsm_Delivering); // Must be set by 'ProcessingEnd'
    DEBUG_SC_EVENT_LOCAL(">>Delivering");
    ObserverNotify(gcob_ObserveDeliveringBegin);
    DeliveringBegin_Do();
 }


void scGenComp_PU_Abstract::
    DeliveringEnd_method()
{
    DeliveringEnd_Do();
    mLastTransmissionTime = sc_core::sc_time_stamp();    // Remember
    StateFlag_Set(gcsm_Relaxing);   // Pass to "Relaxing"
    ObserverNotify(gcob_ObserveDeliveringEnd);
    EVENT_GenComp.RelaxingBegin.notify(SC_ZERO_TIME);
                DEBUG_SC_EVENT_LOCAL("SENT EVENT_GenComp.RelaxingBegin");
                DEBUG_SC_EVENT_LOCAL("<<<Delivering");
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
                DEBUG_SC_EVENT_LOCAL("Input received");
    ObserverNotify(gcob_ObserveInput);
    InputReceived_Do();
}

// This routine makes actual input processing, although most of the job is done in Process and Heartbeat
void scGenComp_PU_Abstract::
    InputReceived_Do(void)
{
    if(StateFlag_Get()==gcsm_Processing)
    {
        Inputs.push_back(NoOfInputsReceived_Get());
    }
    else
    {
                DEBUG_SC_WARNING_LOCAL("Omitted input received in state " << GenCompStatesString[StateFlag_Get()]);
    }
}


// Called when the state 'processing' begins
void scGenComp_PU_Abstract::
    ProcessingBegin_method()
{
                DEBUG_SC_EVENT_LOCAL(">>>Processing");
    ObserverNotify(gcob_ObserveProcessingBegin);
    // For statistic, remember the duration of 'Idle' state
    if(!mOperationCounter++)mLastRelaxingEndTime = sc_core::sc_time_stamp();
    mLastIdleTime = sc_core::sc_time_stamp() - mLastRelaxingEndTime; // Remember idle time before this processing
    mLastOperatingTime = sc_core::sc_time_stamp();  // Remember beginning of processing
    mLocalTimeBase = mLastOperatingTime;            // The clock is synchronized to the beginning of processing
    StateFlag_Set(gcsm_Processing);                 // Passing to statio 'Processing'
    ProcessingBegin_Do();         // Now perform the activity in the derived classes
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
    ProcessingEnd_Do();          // Now perform the activity in the derived classes
    ObserverNotify(gcob_ObserveProcessingEnd);
    mLastResultTime = scLocalTime_Get();
    StateFlag_Set(gcsm_Delivering);             // Pass to "Delivering"
    EVENT_GenComp.DeliveringBegin.notify(SC_ZERO_TIME);
                DEBUG_SC_EVENT_LOCAL("SENT EVENT_GenComp.DeliveringBegin");
                DEBUG_SC_EVENT_LOCAL("<<<Processing");
}


// Called when back to the "Ready" statio
void scGenComp_PU_Abstract::
    Ready_method()
{
    ObserverNotify(gcob_ObserveReady);
    Ready_Do();
}

// Called when the state 'Relaxing' begins
void scGenComp_PU_Abstract::
    RelaxingBegin_method()
{
            DEBUG_SC_EVENT_LOCAL(">>>Relaxing");
    ObserverNotify(gcob_ObserveRelaxingBegin);
    RelaxingBegin_Do();
}

// Relaxing/refractory has finished
void scGenComp_PU_Abstract::
    RelaxingEnd_method()
{
    RelaxingEnd_Do();
    mLastOperatingTime = scLocalTime_Get();    // Remember when we were ready
    mLastRelaxingEndTime = sc_core::sc_time_stamp();    // Remember when we were ready
    StateFlag_Set(gcsm_Ready);   // Pass to "Delivering"
    EVENT_GenComp.Ready.notify(SC_ZERO_TIME);
                DEBUG_SC_EVENT_LOCAL("<<<Relaxing");
}

// External synchronization
void scGenComp_PU_Abstract::
    Synchronize_method(void)
{
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
                DEBUG_SC_EVENT_LOCAL("Failed");
    EVENT_GenComp.Initialize.notify(SC_ZERO_TIME);
}
void scGenComp_PU_Abstract::
    Failed_Do()
{
    DEBUG_SC_EVENT_LOCAL("   ---");
}

// A backlink from the registered units to the simulator
void scGenComp_PU_Abstract::
    RegisterSimulator(scGenComp_Simulator* Observer)
{
    MySimulator = Observer;
}

// Handles the observed events
void scGenComp_PU_Abstract::
    ObserverNotify(GenCompPUObservingBits_t  ObservedBit)
{
    if(!MySimulator) return; // Module not registered
    if(ObservingBit_Get(ObservedBit))
        MySimulator->Observe(this,ObservedBit);
}


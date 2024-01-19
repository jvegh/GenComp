/** @file scGenComp_PU_AbstractDEMO.cpp
 *  @ingroup GENCOMP_MODULE_DEMO
 *  @brief  The DEMO processing unit for abstract generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#include "scGenComp_PU_AbstractDEMO.h"
// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing


scGenComp_PU_AbstractDEMO::
    scGenComp_PU_AbstractDEMO(sc_core::sc_module_name nm   // Module name
                        ,sc_core::sc_time Heartbeat ):  // Heartbeat time
    scGenComp_PU_Abstract(nm)
 {   // Needed to avoid using SystemC specific syntax
    typedef scGenComp_PU_AbstractDEMO SC_CURRENT_USER_MODULE;
    // This routine will be called after initalizations but before starting simulation
    SC_THREAD(InitializeForDemo_method);
    sensitive << Demo_Event;
    //dont_initialize(); // A simple method for starting the demo

    // ** Do not reimplement any of the xxx_method functions
    // until you know what you are doing
};

// For demonstration purposes, drives generic 'abstract' PU through all states
// Prepare events for the demo unit; runs before the other 'method's
// at 120 ms finishes 'Processing'
void scGenComp_PU_AbstractDEMO::
    InitializeForDemo_method()
{
    // Set up which events are to be monitored
    // group and module observing are enabled by default
    ObservingBit_Set(gcob_ObserveHeartbeat, true);
    ObservingBit_Set(gcob_ObserveInput, true);
    ObservingBit_Set(gcob_ObserveInitialize, true);
    ObservingBit_Set(gcob_ObserveProcessingBegin, true);
    ObservingBit_Set(gcob_ObserveProcessingBegin, true);
    ObservingBit_Set(gcob_ObserveDeliveringBegin, true);
    ObservingBit_Set(gcob_ObserveDeliveringEnd, true);
    ObservingBit_Set(gcob_ObserveReady, true);
    ObservingBit_Set(gcob_ObserveRelaxingBegin, true);
    ObservingBit_Set(gcob_ObserveRelaxingBegin, true);
    EVENT_GenComp.Initialize.notify(SC_ZERO_TIME);
    EVENT_GenComp.Ready.notify(SC_ZERO_TIME);
    DEBUG_SC_PRINT("Will issue 'ProcessingBegin' @ 10 ms SIMULATED time");
    EVENT_GenComp.ProcessingBegin.notify(sc_core::sc_time(10,SC_MS));
}

// Here overload default processing: just wait and issue event 'ProcessingEnd'
void scGenComp_PU_AbstractDEMO::
    ProcessingBegin_Do()
{
    scGenComp_PU_Abstract::ProcessingBegin_Do(); // Reset input processing
    // Replace actual processing activity with a delay
    EVENT_GenComp.ProcessingEnd.notify(sc_core::sc_time(250,SC_US));
}

// Here overload default processing: just wait and issue event 'ProcessingEnd'
void scGenComp_PU_AbstractDEMO::
    DeliveringBegin_Do()
{
    scGenComp_PU_Abstract::DeliveringBegin_Do();
    // Replace actual delivering activity with a delay
    EVENT_GenComp.DeliveringEnd.notify(sc_core::sc_time(150,SC_US));
}

void scGenComp_PU_AbstractDEMO::
    Ready_Do()
{
            DEBUG_SC_PRINT("Ready to go!");

}

void scGenComp_PU_AbstractDEMO::
    RelaxingBegin_Do()
{
    // Replace actual relaxing activity with a delay
    EVENT_GenComp.RelaxingEnd.notify(sc_core::sc_time(100,SC_US));
}

void scGenComp_PU_AbstractDEMO::
    RelaxingEnd_Do()
{
    DEBUG_SC_LOG_LOCAL("Processing time was: " << sc_time_String_Get(mLastProcessingTime,SC_TIME_UNIT_DEFAULT));
    DEBUG_SC_LOG_LOCAL("Idle time was: " << sc_time_String_Get(mLastIdleTime,SC_TIME_UNIT_DEFAULT));
}

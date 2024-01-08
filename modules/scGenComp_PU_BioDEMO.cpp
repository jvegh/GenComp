/** @file scGenComp_PU_BioDEMO.cpp
 *  @ingroup GENCOMP_MODULE_DEMO
 *  @brief  The DEMO processing unit for biological generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#include "scGenComp_PU_BioDEMO.h"
// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing
extern GenCompStates_Bio *TheGenCompStates_Bio;

scGenComp_PU_BioDEMO::
    scGenComp_PU_BioDEMO(sc_core::sc_module_name nm):
    scGenComp_PU_Bio(nm)

{   mHeartbeat = HEARTBEAT_TIME_DEFAULT_BIO;
    // Needed to avoid using SystemC specific syntax
    typedef scGenComp_PU_BioDEMO SC_CURRENT_USER_MODULE;
    // This routine is called after initalizations but before starting simulation
    SC_THREAD(InitializeForDemo_method);
    sensitive << Demo_Event;
    // ** Do not reimplement any of the xxx_method functions
    // until you know what you are doing
};

// Prepare events for the demo unit; runs before the other 'method's
// at 120 ms finishes 'Processing'
void scGenComp_PU_BioDEMO::
    InitializeForDemo_method()
{
    // Set up which events are to be monitored
    // group and module observing are enabled by default
//    EVENT_GenComp.Initialize.notify(SC_ZERO_TIME);
    // Not really needed: done in constructor
    ObservingBit_Set(gcob_ObserveInput, true);
    ObservingBit_Set(gcob_ObserveInitialize, true);
    ObservingBit_Set(gcob_ObserveProcessingBegin, true);
    ObservingBit_Set(gcob_ObserveHeartbeat, true);
    // The unit is 'Ready', expected to 'live' at 120 us
            DEBUG_SC_PRINT("Will issue 'InputReceived' @ 10 ms");
    wait(10,SC_MS);
    EVENT_GenComp.InputReceived.notify(SC_ZERO_TIME);
            DEBUG_SC_EVENT("DEMO_DRIVER SENT #1 EVENT_GenComp.InputReceived  @10,000 (000) us");
            DEBUG_SC_EVENT("DEMO_DRIVER XPCT EVENT_GenComp.BeginProcessing");
    // Receiving an input, also starts 'Processing'
    // The BPU starts to receive spikes
    wait(200,SC_US);
     EVENT_GenComp.InputReceived.notify(SC_ZERO_TIME);
            DEBUG_SC_EVENT("DEMO_DRIVER SENT #2 EVENT_GenComp.InputReceived  @10,200 (200) us");
    wait(500,SC_US);
    EVENT_GenComp.InputReceived.notify(SC_ZERO_TIME);
            DEBUG_SC_EVENT("DEMO_DRIVER SENT #3 InputReceived  @10,500 (500) us");
     // Now we sent 3 spikes
    // At local time 900, the membrane threshold potential exceeded
            DEBUG_SC_PRINT("Another 'InputReceived' @10,500 (500) us");
    wait(200,SC_US);
            EVENT_GenComp.Synchronize.notify(SC_ZERO_TIME);
            DEBUG_SC_EVENT("DEMO_DRIVER SENT EVENT_GenComp.Synchronized");
            DEBUG_SC_PRINT("Another 'Syncronized' @10,900 (+900 us");

    // The BPU starts to receive spikes
    wait(50,SC_US);
            DEBUG_SC_EVENT("Now processing must be over");
}


    /*
    * Handle heartbeats in 'Processing' mode
    */
    void scGenComp_PU_BioDEMO::
    Heartbeat_Processing()
{
    HeartbeatRecalculateMembranePotential();
    if (MembraneThresholdExceeded_Processing())
        {   // We are about finishing processing
            EVENT_GenComp.ProcessingEnd.notify(SC_ZERO_TIME);
                DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.ProcessingEnd");
            EVENT_GenComp.Heartbeat.cancel();
                DEBUG_SC_EVENT_LOCAL("CNCL    EVENT_GenComp.Heartbeat");
        }
    else
        {   // We are still processing; re-issue the heartbeat
            // if the limit is not yet reached
            EVENT_GenComp.Heartbeat.notify(BIO_HEARTBEAT_TIME);
                DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with " << sc_time_String_Get(BIO_HEARTBEAT_TIME, SC_TIME_UNIT_DEFAULT) << " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT]);
        }
 }

 // NO PDE solving, just exceeded some characteristic tims
bool scGenComp_PU_BioDEMO::
     MembraneThresholdExceeded_Processing(void)
{
    return scLocalTime_Get() >= sc_core::sc_time(600,SC_US);
}


// Recalculate the membrane potential for the last BIO_HEARTBEAT_TIME period
void scGenComp_PU_BioDEMO::
    HeartbeatRecalculateMembranePotential()
{
                DEBUG_SC_PRINT_LOCAL("Calculate membrane potential");
}

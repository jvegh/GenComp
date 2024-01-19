/** @file scGenComp_PU_Bio_IzhikevichDEMO.cpp
 *  @ingroup GENCOMP_MODULE_DEMO
 *  @brief  The DEMO processing unit for biological generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#include "scGenComp_PU_Bio_IzhikevichDEMO.h"
// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing
//extern GenCompStates_Bio *TheGenCompStates_Bio;


scGenComp_PU_Bio_IzhikevichDEMO::
    scGenComp_PU_Bio_IzhikevichDEMO(sc_core::sc_module_name nm   // Module name
                        ,sc_core::sc_time HeartBeat ):  // Heartbeat time
   scGenComp_PU_Bio_Izhikevich(nm,HeartBeat)

{   // Needed to avoid using SystemC specific syntax
    typedef scGenComp_PU_Bio_IzhikevichDEMO SC_CURRENT_USER_MODULE;
    // This routine will be called after initalizations but before starting simulation
    SC_THREAD(InitializeForDemo_method);
    sensitive << Demo_Event;
    //dont_initialize(); // A simple method for starting the demo

    // ** Do not reimplement any of the xxx_method functions
    // until you know what you are doing
};

// For demonstration purposes, drives generic 'Bio' PU through all states
// Prepare events for the demo unit; runs before the other 'method's
// at 120 ms finishes 'Processing'
void scGenComp_PU_Bio_IzhikevichDEMO::
    InitializeForDemo_method()
{
    InputCurrent_Set(5000); // Use 5 nA constant input current

    // Set up which events are to be monitored
    // group and module observing are enabled by default
    ObservingBit_Set(gcob_ObserveInput, true);
    ObservingBit_Set(gcob_ObserveInitialize, true);
    ObservingBit_Set(gcob_ObserveProcessingBegin, true);
    ObservingBit_Set(gcob_ObserveProcessingBegin, true);
    ObservingBit_Set(gcob_ObserveHeartbeat, true);
    wait(313,SC_US);

    //    EVENT_GenComp.Initialize.notify(SC_ZERO_TIME);
//    Heartbeat_Set(BIO_DEMO_HEARTBEAT_TIME); // Just to speed up demo
    HeartbeatTime_Set(sc_core::sc_time(256,SC_US));
    DEBUG_SC_PRINT_LOCAL("Heartbeat is set to: "  << sc_time_String_Get(mHeartbeat, SC_TIME_UNIT_DEFAULT)<< " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT]
                                                 << ", in " << HeartbeatDivisions_Get() << " divisions");
//??    EVENT_GenComp.InputReceived.notify(20,SC_MS);
    // Not really needed: done in constructor
//??            DEBUG_SC_PRINT("Will issue 'InputReceived' @ 10 ms");
//??    wait(10,SC_MS);
    EVENT_GenComp.InputReceived.notify();
            DEBUG_SC_EVENT("DEMO_DRIVER SENT #0 EVENT_GenComp.InputReceived  @0,313 (+000) us");
            DEBUG_SC_EVENT("DEMO_DRIVER XPCT EVENT_GenComp.BeginProcessing");
    // Receiving an input, also starts 'Processing'
    // The BPU starts to receive spikes
    wait(200,SC_US);
     EVENT_GenComp.InputReceived.notify(SC_ZERO_TIME);
            DEBUG_SC_EVENT("DEMO_DRIVER SENT #1 EVENT_GenComp.InputReceived  @0,313 (+200) us");
    wait(500,SC_US);
    EVENT_GenComp.InputReceived.notify(SC_ZERO_TIME);
            DEBUG_SC_EVENT("DEMO_DRIVER SENT #2 InputReceived  @0,313 (+700) us");
     // Now we sent 3 spikes
    // At local time 900, the membrane threshold potential exceeded
    wait(200,SC_US);
            // Now we are at @10,900 (900) us
/*            EVENT_GenComp.Synchronize.notify(SC_ZERO_TIME);
            DEBUG_SC_EVENT("DEMO_DRIVER SENT EVENT_GenComp.Synchronized");
            DEBUG_SC_PRINT("Another 'Syncronized' @10,900 (+900 us");
*/
    // The BPU starts to receive spikes
    wait(50,SC_US);
            DEBUG_SC_EVENT("Now processing must be over");
}


/*
    * Handle heartbeats in 'Processing' mode
    */
#if 0

void scGenComp_PU_Bio_IzhikevichDEMO::
    Heartbeat_Processing_Do()
{
 //   HeartbeatRecalculateMembranePotential();
    if (Processing_Finished())
    {   // We are about finishing processing
        EVENT_GenComp.ProcessingEnd.notify(SC_ZERO_TIME);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.ProcessingEnd");
        /*    EVENT_GenComp.Heartbeat.cancel();
                DEBUG_SC_EVENT_LOCAL("CNCL    EVENT_GenComp.Heartbeat");  obsolete*/
    }
    else
    {   // We are still processing; re-issue the heartbeat
        // if the limit is not yet reached
        EVENT_GenComp.Heartbeat.notify( mHeartbeat);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with " << sc_time_String_Get(mHeartbeat, SC_TIME_UNIT_DEFAULT) << " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT]);
    }
}
#endif //0


#if 0

 // NO PDE solving, just exceeded some characteristic tims
bool scGenComp_PU_Bio_IzhikevichDEMO::
    Processing_Finished(void)
{
    return scLocalTime_Get() >= sc_core::sc_time(900,SC_US);
}


// Recalculate the membrane potential for the last BIO_HEARTBEAT_TIME period
void scGenComp_PU_Bio_IzhikevichDEMO::
    HeartbeatRecalculateMembranePotential()
{
                DEBUG_SC_PRINT_LOCAL("Calculate membrane potential");
}

#endif

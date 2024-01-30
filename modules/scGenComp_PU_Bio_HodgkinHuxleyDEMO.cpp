/** @file scGenComp_PU_Bio_HodgkinHuxleyDEMO.cpp
 *  @ingroup GENCOMP_MODULE_DEMO
 *  @brief  The DEMO processing unit for HodgkinHuxley-type biological generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#include "scGenComp_PU_Bio_HodgkinHuxleyDEMO.h"
// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"


scGenComp_PU_Bio_HodgkinHuxleyDEMO::
    scGenComp_PU_Bio_HodgkinHuxleyDEMO(sc_core::sc_module_name nm   // Module name
                        ,bool AsPublished // if without statios 'Delivering and 'Relaxing'
                        ,sc_core::sc_time HeartBeat ):  // Heartbeat time
   scGenComp_PU_Bio_HodgkinHuxley(nm,AsPublished,HeartBeat)

{   // Needed to avoid using SystemC specific syntax
    typedef scGenComp_PU_Bio_HodgkinHuxleyDEMO SC_CURRENT_USER_MODULE;
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
void scGenComp_PU_Bio_HodgkinHuxleyDEMO::
    InitializeForDemo_method()
{
    // Set up which events are to be monitored
    // group and module observing are enabled by default
    ObservingBit_Set(gcob_ObserveInput, true);
    ObservingBit_Set(gcob_ObserveInitialize, true);
    ObservingBit_Set(gcob_ObserveProcessingBegin, true);
    ObservingBit_Set(gcob_ObserveProcessingBegin, true);
    ObservingBit_Set(gcob_ObserveHeartbeat, true);
    wait(313,SC_US);

     HeartbeatTime_Set(sc_core::sc_time(128,SC_US));
    DEBUG_SC_PRINT_LOCAL("Heartbeat is set to: "  << sc_time_String_Get(mHeartbeat, SC_TIME_UNIT_DEFAULT)<< " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT]
                                                 << ", in " << HeartbeatDivisions_Get() << " divisions");
    EVENT_GenComp.InputReceived.notify(SC_ZERO_TIME);
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
    wait(500,SC_US);
            DEBUG_SC_EVENT("Waiting to finish, no more events");
}





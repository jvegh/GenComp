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
    // ** Do not reimplement any of the xxx_method functions
    // until you know what you are doing
};

// Prepare events for the demo unit; runs before the other 'method's

// at 20 finishes 'Processing'
void scGenComp_PU_BioDEMO::
    InitializeForDemo_method()
{
    // The unit is 'Ready', expected to 'live' at 12 us
            DEBUG_SC_PRINT("Will issue 'InputReceived' @ 120 us");
    wait(120,SC_US);
            DEBUG_SC_EVENT("SENT EVENT_GenComp.InputReceived");
    EVENT_GenComp.InputReceived.notify(SC_ZERO_TIME);
    // Receiving an input, also starts 'Processing'
    //  at 170, BPU  receives its second spike
    // The BPU starts to receive spikes
    wait(50,SC_US);
            DEBUG_SC_PRINT("Another 'InputReceived' @50 us");
            DEBUG_SC_EVENT("DEMO_DRIVER SENT EVENT_GenComp.InputReceived");
    EVENT_GenComp.InputReceived.notify(SC_ZERO_TIME);
    // Now we sent 2 spikes
    // At local time 10, the membrane threshold potential exceeded

    // The BPU starts to receive spikes
    wait(120,SC_US);
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

bool scGenComp_PU_BioDEMO::
     MembraneThresholdExceeded_Processing(void)
{
    return scLocalTime_Get() >= sc_time(3*BIO_HEARTBEAT_TIME);
}


// Recalculate the membrane potential for the last BIO_HEARTBEAT_TIME period
void scGenComp_PU_BioDEMO::
    HeartbeatRecalculateMembranePotential()
{
                DEBUG_SC_PRINT_LOCAL("Calculate membrane potential");
}

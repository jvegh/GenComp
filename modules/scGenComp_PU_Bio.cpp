/** @file scGenComp_PU_Bio.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#include "scGenComp_PU_Bio.h"
// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing
static GenCompStates_Bio *TheGenCompStates_Bio;

// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing
    scGenComp_PU_Bio::
scGenComp_PU_Bio(sc_core::sc_module_name nm):
    scGenComp_PU_Abstract(nm)
{
    typedef scGenComp_PU_Bio SC_CURRENT_USER_MODULE;
    if(!TheGenCompStates_Bio)
        TheGenCompStates_Bio = new GenCompStates_Bio(); // We need one singleton copy of state machine
    MachineState =  TheGenCompStates_Bio;     // However, the state flag is stored per PU object
    // ** Do not reimplement any of the xxx_method functions
    // until you know what you are doing
}

    scGenComp_PU_Bio::
~scGenComp_PU_Bio(void)
{
}



void scGenComp_PU_Bio::
    Deliver()
{   // The state machine ensures that we are in phases either 'Processing' or 'Delivering'
    DEBUG_SC_EVENT_LOCAL("   ---");
    if(gcsm_Processing == StateFlag_Get())
    {   // We are at the beginning of the 'Delivering' phase
        StateFlag_Set(gcsm_Delivering);
    }
    else
    {   // We are at the end of the 'Delivering' phase
        EVENT_GenComp.DeliveringEnd.notify(SC_ZERO_TIME);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.End_Delivering");
        MachineState->Relax(this);
    }
}

// Heartbeat distributin happens in scGenComp_PU_Abstract
/**
     *
     * Handle heartbeats in 'Delivering' mode
     */
void scGenComp_PU_Bio::
    Heartbeat_Delivering()
{
    if (scLocalTime_Get() < sc_time(8*HEARTBEAT_TIME_DEFAULT_BIO))
    {   // We are still processing; re-issue the heartbeat
        // if the limit is not yet reached
        EVENT_GenComp.Heartbeat.notify(HEARTBEAT_TIME_DEFAULT_BIO);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with BIO_HEARTBEAT_TIME");
    }
    else
    {   // We are about finishing processing
        EVENT_GenComp.ProcessingEnd.notify(SC_ZERO_TIME);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.DeliveringEnd");
    }
}
    /**
     *
     * Handle heartbeats in 'Processing' mode
     */
void scGenComp_PU_Bio::
    Heartbeat_Processing()
{
    CalculateMembranePotential();
    if (scLocalTime_Get() < sc_time(5*HEARTBEAT_TIME_DEFAULT_BIO))
        {   // We are still processing; re-issue the heartbeat
            // if the limit is not yet reached
            EVENT_GenComp.Heartbeat.notify(HEARTBEAT_TIME_DEFAULT_BIO);
            DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with BIO_HEARTBEAT_TIME");
        }
    else
        {   // We are about finishing processing
            EVENT_GenComp.ProcessingEnd.notify(SC_ZERO_TIME);
            DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.ProcessingEnd");
            EVENT_GenComp.Heartbeat.cancel();
            DEBUG_SC_EVENT_LOCAL("CNCL    EVENT_GenComp.Heartbeat");
        }
}
    /**
     *
     * Handle heartbeats in 'Ready' mode
     */
void scGenComp_PU_Bio::
    Heartbeat_Ready()
{
    if (scLocalTime_Get() < sc_time(2*HEARTBEAT_TIME_DEFAULT_BIO))
    {   // We are still processing; re-issue the heartbeat
        // if the limit is not yet reached
        EVENT_GenComp.Heartbeat.notify(HEARTBEAT_TIME_DEFAULT_BIO);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with BIO_HEARTBEAT_TIME");
    }
    else
    {   // We are about finishing processing
        EVENT_GenComp.ProcessingEnd.notify(SC_ZERO_TIME);
        //Do not send an event; just  wait
        //        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.End_Computing");
    }
}
    /**
     *
     * Handle heartbeats in 'Relaxing' mode
     */
void scGenComp_PU_Bio::
    Heartbeat_Relaxing()
{
    if (scLocalTime_Get() < sc_time(8*HEARTBEAT_TIME_DEFAULT_BIO))
    {   // We are still processing; re-issue the heartbeat
        // if the limit is not yet reached
        EVENT_GenComp.Heartbeat.notify(HEARTBEAT_TIME_DEFAULT_BIO);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with BIO_HEARTBEAT_TIME");
    }
    else
    {   // We are about finishing processing
        EVENT_GenComp.RelaxingEnd.notify(SC_ZERO_TIME);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.RelaxingEnd");
    }

}

/*

// Puts the PU in its default state
void scGenComp_PU_Bio::
    Initialize_method(void)
{
    DoInitialize();
}
*/

/**
 * A spike arrived, store spike parameters;
 * If it was the first spike, issue 'Begin_Computing'
 */
void scGenComp_PU_Bio::
    InputReceived_method()
{
    DEBUG_SC_EVENT("RCVD EVENT_GenComp.InputReceived");
    // In bio mode, any input causes passing to 'Processing' statio
    if(!NoOfInputsReceived_Get())
        {   // This is the first input we received, change the state first
            MachineState->Process(this);
            ProcessingBegin();
            DEBUG_SC_EVENT("SENT implicite 'EVENT_GenComp.ProcessingBegin'");
        }
    // The input is legal, statio is OK, continue receiving it
    MachineState->InputReceive(this);
}


/*
 * This routine makes actual input processing, although most of the job is done in Process() and Heartbeat()
 * It is surely called in state 'Processing'
 */
void scGenComp_PU_Bio::
    DoInputReceive(void)
{
    scGenComp_PU_Abstract::DoInputReceive();
}


void scGenComp_PU_Bio::
    ProcessingBegin()
{
    scGenComp_PU_Abstract::ProcessingBegin();  // Make default processing
    EVENT_GenComp.Heartbeat.notify(SC_ZERO_TIME);
                DEBUG_SC_EVENT_LOCAL("SENT 'EVENT_GenComp.HeartBeat' with zero");
}

// Called when the state 'processing' ends
void scGenComp_PU_Bio::
    ProcessingEnd_method()
{
    DEBUG_SC_EVENT_LOCAL("RCVD    EVENT_GenComp.ProcessingEnd");
    MachineState->Deliver(this);

    DEBUG_SC_EVENT_LOCAL("Processing finished");
}

void scGenComp_PU_Bio::
    CalculateMembranePotential()
{
    int i=1;
}

void scGenComp_PU_Bio::
    Relax()
{   // The state machine ensures that we are in phases either 'Delivering' or 'Relaxing'
     if(gcsm_Processing == StateFlag_Get())
    {   // We are at the beginning of the 'Delivering' phase
        StateFlag_Set(gcsm_Relaxing);
    }
    else
    {   // We are at the end of the 'Delivering' phase
        EVENT_GenComp.DeliveringEnd.notify(SC_ZERO_TIME);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.End_Delivering");
        MachineState->Initialize(this);
        StateFlag_Set(gcsm_Ready);
    }
}

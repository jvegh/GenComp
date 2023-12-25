/** @file scBioGenComp_PU.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scBioGenComp_PU.h"
// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing
static BioGenCompState *TheBioGenCompState;

// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing
    scBioGenComp_PU::
scBioGenComp_PU(sc_core::sc_module_name nm):
    scAbstractGenComp_PU(nm)
{
    typedef scBioGenComp_PU SC_CURRENT_USER_MODULE;
    if(!TheBioGenCompState)
        TheBioGenCompState = new BioGenCompState(); // We need one singleton copy of state machine
    MachineState =  TheBioGenCompState;     // However, the state flag is stored per PU object
    SC_METHOD(Heartbeat_method);
    sensitive << EVENT_GenComp.HeartBeat;
    dont_initialize();
    SC_METHOD(InputReceived_method);
    sensitive << EVENT_GenComp.InputReceived;
    dont_initialize();
}

    scBioGenComp_PU::
~scBioGenComp_PU(void)
{
}

// Puts the PU in its default state
void scBioGenComp_PU::
    Initialize_method(void)
{
            DEBUG_SC_LOCAL_EVENT(">>>   ",mLocalTimeBase,"");
    MachineState->Initialize(this);   // Change status to 'Initial'
    Initialize(); // Initialize the unit, HW and temporary variables
    // Put PU in its default state
            DEBUG_SC_LOCAL_EVENT("<<<   ",mLocalTimeBase,"");
}

/* The local neurons keep their local time
 * A time base is set by 'Begin Computing' or 'Begin Sleeping'
 *
 */
void scBioGenComp_PU::
    Initialize(void)
{
    mLocalTimeBase = sc_time_stamp();
}

/**
 * A spike arrived, store spike parameters;
 * If it was the first spike, issue 'Begin_Computing'
 */
void scBioGenComp_PU::
    InputReceived_method()
{
            DEBUG_SC_LOCAL_EVENT(">>>   ", mLocalTimeBase, "Input");
    MachineState->InputReceived(this);
            DEBUG_SC_LOCAL_EVENT("<<<   ",mLocalTimeBase, "Input");
}


/*
 * This routine makes actual input processing, although most of the job is done in Process and Heartbeat
 * It is surely called in state 'Processing'
 */
void scBioGenComp_PU::
    ReceiveInput(void)
{
           // DEBUG_SC_LOCAL_EVENT(">>>   ", mLocalTimeBase, Inputs.size());
    Inputs.push_back(Inputs.size()); // TEMPORARY, Just store the index
    if(1 == Inputs.size())
    {   // Start the HeartBeat processing instantly
        StateFlag_Set(gcsm_Processing);
        EVENT_GenComp.HeartBeat.notify(SC_ZERO_TIME);
            DEBUG_SC_LOCAL_EVENT("",mLocalTimeBase,"SENT    EVENT_GenComp.HeartBeat with zero");
        EVENT_GenComp.Begin_Computing.notify(SC_ZERO_TIME);
            scLocalTime_Set();      // The clock is synchronized to the beginning of processing
    }
    // DEBUG_SC_LOCAL_EVENT("<<<   ", mLocalTimeBase, Inputs.size());
}

void scBioGenComp_PU::
    Heartbeat_method()
{
    Heartbeat();    // Calculate the state at the new time
    if (scLocalTime_Get() < sc_time(50,SC_US))
    {   // We are still processing
        EVENT_GenComp.HeartBeat.notify(BIO_HEARTBEAT_TIME);
            DEBUG_SC_LOCAL_EVENT("",mLocalTimeBase,"SENT    EVENT_GenComp.HeartBeat with BIO_HEARTBEAT_TIME");
    }
    else
    {   // We are finishing processing
            EVENT_GenComp.End_Computing.notify(SC_ZERO_TIME);
            MachineState->Deliver(this);

    }
             DEBUG_SC_LOCAL_EVENT("<<<   ",mLocalTimeBase,"");

}
// The state of the biological computing is re-calculated (as the simulation time passes)
//
//
void scBioGenComp_PU::
    Heartbeat()
{
            DEBUG_SC_LOCAL_EVENT("   ---",mLocalTimeBase,"");
}


void scBioGenComp_PU::
    Deliver()
{   // The state machine ensures that we are in phases either 'Processing' or 'Delivering'
            DEBUG_SC_LOCAL_EVENT("   ---",mLocalTimeBase,"");
            DEBUG_SC_EVENT("...   ");
            DEBUG_SC_PRINT("Test again");
    if(gcsm_Processing == StateFlag_Get())
    {   // We are at the beginning of the 'Delivering' phase
            StateFlag_Set(gcsm_Delivering);
    }
    else
    {   // We are at the beginning of the 'Delivering' phase
    }
}

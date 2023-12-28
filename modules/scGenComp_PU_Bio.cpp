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
static BioGenCompState *TheBioGenCompState;

// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing
    scGenComp_PU_Bio::
scGenComp_PU_Bio(sc_core::sc_module_name nm):
    scGenComp_PU_Abstract(nm)
{
    typedef scGenComp_PU_Bio SC_CURRENT_USER_MODULE;
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

    scGenComp_PU_Bio::
~scGenComp_PU_Bio(void)
{
}

// Puts the PU in its default state
void scGenComp_PU_Bio::
    Initialize_method(void)
{
            DEBUG_SC_EVENT_LOCAL(">>>   ");
    MachineState->Initialize(this);   // Change status to 'Initial'
    Initialize(); // Initialize the unit, HW and temporary variables
    // Put PU in its default state
            DEBUG_SC_EVENT_LOCAL("<<<   ");
}

/* The local neurons keep their local time
 * A time base is set by 'Begin Computing' or 'Begin Sleeping'
 *
 */
void scGenComp_PU_Bio::
    Initialize(void)
{
    mLocalTimeBase = sc_time_stamp();
}

/**
 * A spike arrived, store spike parameters;
 * If it was the first spike, issue 'Begin_Computing'
 */
void scGenComp_PU_Bio::
    InputReceived_method()
{
            DEBUG_SC_EVENT_LOCAL(">>>  Input");
    MachineState->InputReceived(this);
            DEBUG_SC_EVENT_LOCAL("<<<   Input");
}


/*
 * This routine makes actual input processing, although most of the job is done in Process and Heartbeat
 * It is surely called in state 'Processing'
 */
void scGenComp_PU_Bio::
    ReceiveInput(void)
{
           // DEBUG_SC_EVENT_LOCAL(">>>   ", mLocalTimeBase, Inputs.size());
    Inputs.push_back(Inputs.size()); // TEMPORARY, Just store the index
    if(1 == NoOfInputsReceived_Get())
    {   // Start the HeartBeat processing instantly
        StateFlag_Set(gcsm_Processing);
        EVENT_GenComp.HeartBeat.notify(SC_ZERO_TIME);
            DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with zero");
        EVENT_GenComp.Begin_Computing.notify(SC_ZERO_TIME);
            scLocalTime_Set();      // The clock is synchronized to the beginning of processing
    }
    // DEBUG_SC_EVENT_LOCAL("<<<   ", mLocalTimeBase, Inputs.size());
}

void scGenComp_PU_Bio::
    Heartbeat_method()
{
    if(OperatingBit_Get(gcob_ObserveModule) & OperatingBit_Get(gcob_ObserveInput))
        DEBUG_SC_PRINT_LOCAL ("Input observed");

    DEBUG_SC_EVENT_LOCAL("RCVD   EVENT_GenComp.HeartBeat");
    sc_process_handle h2 = sc_get_current_process_handle(); // Returns a handle to process Run
    sc_object* parent = dynamic_cast<scGenComp_PU_Bio*>(h2.get_parent_object());
            DEBUG_SC_PRINT_LOCAL(parent->name());
    Heartbeat();    // Calculate the state at the new time
    if (scLocalTime_Get() < sc_time(50,SC_US))
    {   // We are still processing
        EVENT_GenComp.HeartBeat.notify(BIO_HEARTBEAT_TIME);
            DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with BIO_HEARTBEAT_TIME");
    }
    else
    {   // We are finishing processing
            EVENT_GenComp.End_Computing.notify(SC_ZERO_TIME);
            MachineState->Deliver(this);

    }
 //            DEBUG_SC_EVENT_LOCAL("<<<   ");
}
// The state of the biological computing is re-calculated (as the simulation time passes)
//
//
void scGenComp_PU_Bio::
    Heartbeat()
{
 /*           DEBUG_SC_EVENT_LOCAL("   ---");
    sc_process_handle h2 = sc_get_current_process_handle(); // Returns a handle to process Run
    sc_object* parent = dynamic_cast<scGenComp_PU_Bio*>(h2.get_parent_object());
    DEBUG_SC_PRINT_LOCAL(parent->name());*/
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
    {   // We are at the beginning of the 'Delivering' phase
    }
}

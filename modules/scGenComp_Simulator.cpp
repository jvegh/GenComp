/** @file scGenComp_Simulator.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The SystemC based simulator for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/
#include "scGenComp_Simulator.h"

// Take care: SC_MAKE_TIME_BENCHMARKING and MAKE_TIME_BENCHMARKING
// may be either defined or undefined
#define MAKE_TIME_BENCHMARKING
#define SC_MAKE_TIME_BENCHMARKING

string GenCompSimulatorModesStrings[]{};
scGenComp_Simulator::scGenComp_Simulator(sc_core::sc_module_name nm)
    : sc_core::sc_module( nm)
    ,mToReset(true)             //
    ,mMoreEvents(true)
{   // Initialize CLOCK and SIMULATED time counters
    SC_THREAD(Reset_method);
    sensitive << Enable;
    dont_initialize(); // The event 'enable' starts simulator
 }

/* Reset the simulator for a new processing
     */
void scGenComp_Simulator::
    Reset_method()
{
    BENCHMARK_TIME_RESET(&t,&x,&s); // Total time spent in simulator
    BENCHMARK_TIME_RESET(&D_t,&D_x,&D_s);   // Total time spent with the display/update state
    SC_BENCHMARK_TIME_RESET(&SC_t,&SC_x,&SC_s);
    mToReset = false;
    mResetTime = sc_core::sc_time_stamp();
    for(vector<scGenComp_PU_Abstract*>::size_type i = 0; i != mWatchedPUs.size(); i++)
    {
        mWatchedPUs[i]->CancelEvents(); // Cancel all pending event for this module
        mWatchedPUs[i]->Initialize_Do(); // Initialize
    }
    DEBUG_SC_EVENT("Simulator reset");
 }


bool scGenComp_Simulator::Run(GenCompSimulatorModes_t SimulatorMode, int32_t SimulationUnit)
{
    BENCHMARK_TIME_BEGIN(&t,&x);
    SC_BENCHMARK_TIME_BEGIN(&SC_t,&SC_x);
    SC_time = sc_core::sc_time_stamp();    // Remember the time of calling, for 'gcsm_Timed' mode
    if(mToReset)
    {
        DEBUG_SC_PRINT("SC_Start");
        sc_start(SC_ZERO_TIME);  // We are at the beginning, just make a call to set up the SystemC engine
        Reset_method();
        mMoreEvents = true;
    }
    bool MoreCycles = true; // Anyhow, we make one cycle
    while(MoreCycles && mMoreEvents)  // Until cycling allowed and have ecents
    {   // We are in the process of simulating
        sc_start( sc_time_to_pending_activity());   // Run up to the next activity
        mMoreEvents = sc_pending_activity();    // Check if we have anything to do
        if((MoreCycles))
        {   // We have still more events, see which mode requested
            switch(SimulatorMode)
            {
                default:
                case gcsm_Continuous:
                    MoreCycles = false;
                    break;        ///< Runs to the end

                case gcsm_Eventwise:         ///< Stops after a certain number of events
                    MoreCycles = (--SimulationUnit > 0);    // Count events
                    break;

                case gcsm_Timed:
                    MoreCycles = sc_core::sc_time_stamp() - SC_time <= SimulationUnit*HEARTBEAT_TIME_DEFAULT_BIO;
                    break;
            }
        }
    }
    BENCHMARK_TIME_END(&t,&x,&s);
    SC_BENCHMARK_TIME_END(&SC_t,&SC_x,&SC_s);
    return mMoreEvents;  // If we shall come back again
}

// Register only already created modules !!!
void scGenComp_Simulator::RegisterPU(scGenComp_PU_Abstract* Module)
{
    scGenComp_PU_Abstract* Valid = dynamic_cast<scGenComp_PU_Abstract*>(Module);
    assert(Valid);    // Attempting to register something wrong
    mWatchedPUs.push_back(Module);
    Module->RegisterSimulator(this);
            DEBUG_SC_PRINT("Module " << Module->name() << " registered");
}

void  scGenComp_Simulator::Update(void)
{
    BENCHMARK_TIME_BEGIN(&D_t,&D_x);
    if(!mUpdateUnit)
    {   //DEBUG_SC_EVENT("Nothing to update");
        return;    // We  have no new update request
    }
    switch(mUpdateObservingBit) // See if the activity is observed
        {
        default: assert(0); break;
        case gcob_ObserveProcessingBegin: //Watch 'Begin Computing'
            UpdateProcessingBegin(mUpdateUnit);
            break;
        case gcob_ObserveDeliveringBegin: //Watch 'Begin Delivering'
            UpdateDeliveringBegin(mUpdateUnit);
            break;
        case gcob_ObserveDeliveringEnd: //Watch 'End Delivering'
            UpdateDeliveringEnd(mUpdateUnit);
            break;
        case gcob_ObserveProcessingEnd:  // Watch 'End Computing'
            UpdateProcessingEnd(mUpdateUnit);
            break;
        case gcob_ObserveHeartbeat:     // Observe 'Heartbeat's of the PU
            UpdateHeartbeat(mUpdateUnit);
            break;
        case gcob_ObserveInput:         // Observe 'Input's of the PU
            UpdateInput(mUpdateUnit);
            break;
        case gcob_ObserveInitialize:         // Observe 'Initialize's of the PU
            UpdateInitialize(mUpdateUnit);
            break;
        case gcob_ObserveReady: //Watch 'Ready'
            UpdateReady(mUpdateUnit);
            break;
        case gcob_ObserveRelaxingBegin: //Watch 'Begin Relaxing'
            UpdateRelaxingBegin(mUpdateUnit);
            break;
        case gcob_ObserveRelaxingEnd: //Watch 'End Relaxing'
            UpdateRelaxingEnd(mUpdateUnit);
            break;
        }
        mUpdateUnit = (scGenComp_PU_Abstract*)NULL;
        BENCHMARK_TIME_END(&D_t,&D_x,&D_s);
}

// Update input information in simulator
void  scGenComp_Simulator::UpdateInput(scGenComp_PU_Abstract* PU)
{
    DEBUG_SC_PRINT(PU->name() << " received " << PU->NoOfInputsReceived_Get() << " inputs ");
}

// Update initialize information in simulator
void  scGenComp_Simulator::UpdateInitialize(scGenComp_PU_Abstract* PU)
{
    DEBUG_SC_PRINT(PU->name() << " Inited");
}
// Update ''ProcessingBegin' information in simulator
void  scGenComp_Simulator::UpdateProcessingBegin(scGenComp_PU_Abstract* PU)
{
    DEBUG_SC_PRINT(PU->name()<< " ProcessingBegin");
}
// Update ''Ready' information in simulator
void  scGenComp_Simulator::UpdateReady(scGenComp_PU_Abstract* PU)
{
    DEBUG_SC_PRINT(PU->name()<< " Ready");
}
// Update ''RelaxingBegin' information in simulator
void  scGenComp_Simulator::UpdateRelaxingBegin(scGenComp_PU_Abstract* PU)
{
    DEBUG_SC_PRINT(PU->name()<< " RelaxingBegin");
}

// Update 'DeliveringBegin' information in simulator
void  scGenComp_Simulator::UpdateDeliveringBegin(scGenComp_PU_Abstract* PU)
{
    DEBUG_SC_PRINT(PU->name()<< " DeliveringBegin");
}

// Update 'DeliveringEnd' information in simulator
void  scGenComp_Simulator::UpdateDeliveringEnd(scGenComp_PU_Abstract* PU)
{
    DEBUG_SC_PRINT(PU->name()<< " DeliveringEnd");
}

// Update ''ProcessingEnd' information in simulator
void  scGenComp_Simulator::UpdateProcessingEnd(scGenComp_PU_Abstract* PU)
{
    DEBUG_SC_PRINT(PU->name() << " ProcessingEnd");
}
// Update ''RelaxingEnd' information in simulator
void  scGenComp_Simulator::UpdateRelaxingEnd(scGenComp_PU_Abstract* PU)
{
    DEBUG_SC_PRINT(PU->name() << " ProcessingEnd");
}
// Update 'Heartbeat' information in simulator
void  scGenComp_Simulator::UpdateHeartbeat(scGenComp_PU_Abstract* PU)
{
    DEBUG_SC_PRINT(PU->name() << " Heartbeat");
}
void scGenComp_Simulator::
    Observe(scGenComp_PU_Abstract* PU, GenCompPUObservingBits_t B)
{
    BENCHMARK_TIME_BEGIN(&D_t,&D_x);
    assert(PU);     // not installed
    assert(B<gcob_Max); // wrong bit
    if(PU->ObservingBit_Get(gcob_ObserveModule))
    {   // Everything OK, the module is observed:
        // Save the unit pointer and bit for later use in another thread
        mUpdateUnit = PU; mUpdateObservingBit = B;
    }
    else
    {   //No, the module is not observed
        mUpdateUnit = (scGenComp_PU_Abstract*) NULL;
    }
    BENCHMARK_TIME_END(&D_t,&D_x,&D_s);
}
#undef MAKE_TIME_BENCHMARKING
#undef SC_MAKE_TIME_BENCHMARKING

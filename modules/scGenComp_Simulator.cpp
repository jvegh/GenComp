/** @file scGenComp_Simulator.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The SystemC based simulator for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scGenComp_Simulator.h"
// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing


scGenComp_Simulator::scGenComp_Simulator(sc_core::sc_module_name nm)
    : sc_core::sc_module( nm)
    ,mLocalTimeBase(sc_core::SC_ZERO_TIME)
    ,mMoreEvents(false)
{   // Initialize CLOCK and SIMULATED time counters
    BENCHMARK_TIME_RESET(&t,&x,&s);
    SC_BENCHMARK_TIME_RESET(&SC_t,&SC_x,&SC_s);
}

bool scGenComp_Simulator::Run()
{
    BENCHMARK_TIME_BEGIN(&t,&x);
    SC_BENCHMARK_TIME_BEGIN(&SC_t,&SC_x);
    // We are in the process of simulating
    // Run up to the next activity}
    sc_start( sc_time_to_pending_activity() );
    sc_pause(); // Be sure there is no processing
    mMoreEvents = sc_pending_activity();    // Set flag if more to do
    int32_t AA = -12;
    mWatchedPUs[0]->GetData(AA);
    DEBUG_SC_PRINT(" Running continues= " << mMoreEvents);
    BENCHMARK_TIME_END(&t,&x,&s);
    SC_BENCHMARK_TIME_END(&SC_t,&SC_x,&SC_s);
    return mMoreEvents;
}

// Register only already created modules !!!
void scGenComp_Simulator::RegisterPU(scGenComp_PU_Bio* Module)
{
    scGenComp_PU_Abstract* Valid = dynamic_cast<scGenComp_PU_Abstract*>(Module);
    if(!Valid) return;    // Attempting to regisster something wrong
    mWatchedPUs.push_back(Module);
    DEBUG_SC_PRINT("Module " << Module->name() << " registered");
}

void  scGenComp_Simulator::Update(void)
{
    int32_t AA = 0;
    mWatchedPUs[0]->GetData(AA);
}

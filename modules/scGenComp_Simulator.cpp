/** @file scGenComp_Simulator.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The SystemC based simulator for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scGenComp_Simulator.h"


extern bool UNIT_TESTING;	// Whether in course of unit testing

string GenCompSimulatorModesStrings[]{};
scGenComp_Simulator::scGenComp_Simulator(sc_core::sc_module_name nm)
    : sc_core::sc_module( nm)
    ,mLocalTimeBase(sc_core::SC_ZERO_TIME)
    ,mToReset(true)             //
{   // Initialize CLOCK and SIMULATED time counters
    BENCHMARK_TIME_RESET(&t,&x,&s);
    SC_BENCHMARK_TIME_RESET(&SC_t,&SC_x,&SC_s);
}


bool scGenComp_Simulator::Run(GenCompSimulatorModes_t SimulatorMode, int32_t SimulationUnit)
{
    if(mToReset)
    {
        mToReset = false;
        sc_start(SC_ZERO_TIME);  // We are at the beginning, just make a call to set up the SystemC engine

    }
    scLocalTime_Set();
    bool ToContinue = true; // Anyhow, process at least once
    bool MoreEvents = true;
    while(ToContinue)
    {
         // We are in the process of simulating
        // Run up to the next activity}
        sc_start( sc_time_to_pending_activity() );
        sc_pause(); // Be sure there is no processing
        MoreEvents = sc_pending_activity();    // Set flag if more to do
        int32_t AA = -12;
        mWatchedPUs[0]->GetData(AA);
        ToContinue = MoreEvents;

        BENCHMARK_TIME_BEGIN(&t,&x);
        SC_BENCHMARK_TIME_BEGIN(&SC_t,&SC_x);
/*#ifdef SC_BENCHMARK_TIME_BEGIN
        DEBUG_SC_EVENT("Simulator time stamp at begin " << sc_time_String_Get(sc_time_stamp()));
        DEBUG_SC_EVENT("Simulator time stamp " << sc_time_String_Get(sc_time_stamp()));
        DEBUG_SC_EVENT("Simulator begin " << sc_time_String_Get(SC_t));
        DEBUG_SC_EVENT("Simulator length " << sc_time_String_Get(SC_x));
#endif*/
        if(ToContinue)
        {   // We have still more events, see which mode requested
            switch(SimulatorMode)
            {
                default:
                case gcsm_Continuous:
                    break;        ///< Runs to the end
                case gcsm_Eventwise:         ///< Stops after a certain number of events
                  ToContinue = (--SimulationUnit > 0);    // Count events
                    break;

                case gcsm_Timed:
 /*                   DEBUG_SC_EVENT("Simulator time stamp " << sc_time_String_Get(sc_time_stamp()));
                    DEBUG_SC_EVENT("Simulator local time t " <<  sc_time_String_Get(sc_time_stamp()-SC_t));
                    DEBUG_SC_EVENT("Simulator local time x " <<  sc_time_String_Get(sc_time_stamp()-SC_x));
                    DEBUG_SC_EVENT("Simulator time step " << sc_time_String_Get(SimulationUnit*HEARTBEAT_TIME_DEFAULT_BIO));
*/
                     ToContinue = sc_core::sc_time_stamp() - SC_t < SimulationUnit*HEARTBEAT_TIME_DEFAULT_BIO;
                    break;
            }
//                DEBUG_SC_EVENT("Simulator local time x " <<  sc_time_String_Get(sc_time_stamp()-SC_x));
        }
        BENCHMARK_TIME_END(&t,&x,&s);
        SC_BENCHMARK_TIME_END(&SC_t,&SC_x,&SC_s);
//        DEBUG_SC_EVENT("Simulator local time x " <<  sc_time_String_Get(sc_time_stamp()-SC_x));

//                DEBUG_SC_EVENT("Simulator step done") ;
    }

#ifdef SC_BENCHMARK_TIME_BEGIN
/*    DEBUG_SC_EVENT("Simulator time stamp at end " << sc_time_String_Get(sc_time_stamp()));
    DEBUG_SC_EVENT("Simulator begin " << sc_time_String_Get(SC_t));
    DEBUG_SC_EVENT("Simulator length " << sc_time_String_Get(SC_x));
    std::cerr  << "  -Elapsed for running the simulator: " << x.count()/1000/1000. << " msec CLOCK time" << "//" << sc_time_String_Get(SC_s, SC_TIME_UNIT_DEFAULT) << " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT] << " SIMULATED time" << endl;
 */   if (MoreEvents)
        DEBUG_SC_PRINT(" Running continued in period [" << sc_time_String_Get(SC_t) << "," << sc_time_String_Get(SC_x) << "]");
#endif

    return MoreEvents;  // If we shall come back again
}

// Register only already created modules !!!
void scGenComp_Simulator::RegisterPU(scGenComp_PU_Bio* Module)
{
    scGenComp_PU_Abstract* Valid = dynamic_cast<scGenComp_PU_Abstract*>(Module);
    if(!Valid) return;    // Attempting to register something wrong
    mWatchedPUs.push_back(Module);
    DEBUG_SC_PRINT("Module " << Module->name() << " registered");
}

void  scGenComp_Simulator::Update(void)
{
    int32_t AA = 0;
    mWatchedPUs[0]->GetData(AA);
}

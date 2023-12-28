/** @file scGenComp_Simulator.h
 *  @ingroup GENCOMP_MODULE_PROCESS

 *  @brief The SystemC simulator for simulating scGenComp_PU units
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef SCGENCOMPSIMULATOR_H
#define SCGENCOMPSIMULATOR_H
/** @addtogroup GENCOMP_MODULE_PROCESS
 *  @{
 */

#include "scAbstractGenComp_PU.h"
#include <chrono>

#define MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros
#include "MacroTimeBenchmarking.h"    // Must be after the define to have its effect
#define SC_MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros
#include "scMacroTimeBenchmarking.h"    // Must be after the define to have its effect

/*
 * \class scGenComp_Simulator
 * \brief  A simple abstract class to implement the operation of a system comprising PUs
 *
 * Operating principle of event processing simulation:
 * The PUs are registered by the simulator.
 * The simulator runs in a cycle, stopping at every single event.
 * The PUs can set a flag that an event happened (they can decide it that is to be observed),
 * and if something important was observed, the simulator updates the displayed state.
 *
 */

/*!
 * \class scGenComp_Simulator
 * \brief  Implements a general biological-type computing PU
 *
 */
#include "scBioGenComp_PU.h"


extern string GenCompStates[];   // Just for debugging


class scGenComp_Simulator : public sc_core::sc_module
{
public:
    SC_HAS_PROCESS(scGenComp_Simulator);
    scGenComp_Simulator(sc_core::sc_module_name nm);
    /**
     * @brief Run until the next simulated event
     * @return
     */
    bool Run();
    /**
      * @brief Register
      * @param Module the activity of which must be watched
      */
    void RegisterPU(scBioGenComp_PU* Module);
    /**
     * @brief scLocalTime_Set
     * @param T The beginning of the simulated time of the recent operation
     */
    void scLocalTime_Set(sc_core::sc_time T = sc_core::sc_time_stamp()){    mLocalTimeBase = T;}
    sc_core::sc_time scTimeBase_Get(void){return mLocalTimeBase;}
    bool HasMoreToDo(void){ return mMoreEvents;}
    void Update();
    sc_core::sc_time scTime_Get(void){ return SC_s;}
    std::chrono::duration<int64_t, nano> Time_Get(void)    {return s;}
protected:
    sc_core::sc_time mLocalTimeBase;    // The beginning of the local computing
    bool mMoreEvents; ///< If we have more events to simulate
    vector<scAbstractGenComp_PU*> mWatchedModules;   /// Store the registered objects here
    chrono::steady_clock::time_point t;
    std::chrono::duration<int64_t, nano> x,s=(std::chrono::duration< int64_t, nano>)0;
    sc_core::sc_time SC_t, SC_x, SC_s;
};


/** @}*/

#endif // SCGENCOMPSIMULATOR_H

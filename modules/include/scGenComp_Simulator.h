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

#include "scGenComp_PU_Abstract.h"
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
#include "scGenComp_PU_Bio.h"


extern string GenCompStates[];   // Just for debugging


extern string GenCompSimulatorModesStrings[];
/*! \var typedef GenCompSimulatorModes_t
 * \brief the names of the bits in the bitset describing scGenComp_PU_Abstract
 */
typedef enum
{
 //   gcsm_Stopped,       ///< Simulator is not (yet) started of stopped
    gcsm_Continuous,   ///< Runs to the end
    gcsm_Eventwise,      ///< Stops after every single  event
    gcsm_Timed          ///< Runs to
} GenCompSimulatorModes_t;


class scGenComp_Simulator : public sc_core::sc_module
{
public:
    SC_HAS_PROCESS(scGenComp_Simulator);
    scGenComp_Simulator(sc_core::sc_module_name nm);
    /**
     * @brief Run until the next simulated event
     * @return
     */
    bool Run(GenCompSimulatorModes_t SimulatorMode, int32_t SimulatorUnit);

    /**
      * @brief Register
      * @param Module the activity of which must be watched
      */
    void RegisterPU(scGenComp_PU_Bio* Module);
    /**
     * @brief scLocalTime_Set
     * @param T The beginning of the simulated time of the recent operation
     */
    void scLocalTime_Set(sc_core::sc_time T = sc_core::sc_time_stamp()){    mLocalTimeBase = T;}
    sc_core::sc_time scTimeBase_Get(void){return mLocalTimeBase;}
    bool HasMoreToDo(void){ return mMoreEvents;}
    void Reset() {mToReset = true;}
    void Update();
    sc_core::sc_time scTime_Get(void){ return SC_s;}
    std::chrono::duration<int64_t, nano> Time_Get(void)    {return s;}
    GenCompSimulatorModes_t SimulationMode_Get(void){return mSimulationMode;}
    void SimulationMode_Set(GenCompSimulatorModes_t SM){ mSimulationMode = SM;}
protected:
    sc_core::sc_time mLocalTimeBase;    // The beginning of the local computing
    bool mMoreEvents; ///< If we have more events to simulate
    vector<scGenComp_PU_Abstract*> mWatchedPUs;   /// Store the registered objects here
    chrono::steady_clock::time_point t;
    std::chrono::duration<int64_t, nano> x,s=(std::chrono::duration< int64_t, nano>)0;
    sc_core::sc_time SC_t, SC_x, SC_s;
    GenCompSimulatorModes_t mSimulationMode;    ///< Which mode the simulator runs
    int32_t mSimulationUnit;    ///< Unit belonging to the simulation session
    bool mToReset;              ///< If to reset the simulator befor running
};


/** @}*/

#endif // SCGENCOMPSIMULATOR_H

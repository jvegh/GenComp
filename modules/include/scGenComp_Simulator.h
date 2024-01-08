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
#define MAKE_TIME_BENCHMARKING  // uncomment to measure the CLOCK time with benchmarking macros
#include "MacroTimeBenchmarking.h"    // Must be after the define to have its effect
#define SC_MAKE_TIME_BENCHMARKING  // uncomment to measure the SIMULATED time with benchmarking macros
#include "scMacroTimeBenchmarking.h"    // Must be after the define to have its effect

// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

/*
 * \class scGenComp_Simulator
 * \brief  A simple abstract class to implement the operation of a system comprising PUs
 *
 * Operating principle of event processing simulation, in modes
 * - gcsm_Continuous : The simulator runs event-by-event, one event at-a-time
 * - gcsm_Eventwise : process the number of events provided by the numeric parameter
 * - gcsm_Timed : process until the simulated time exceeded (not exact time!)
 *
 * The PUs must be registered by the simulator.
 * The simulator runs in a cycle, stopping at every single event.
 * The PUs can set a flag that an event happened (they can decide if that is to be observed),
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
 //   gcsm_Stopped,         ///< Simulator is not (yet) started of stopped
    gcsm_Continuous,        ///< Runs to the end
    gcsm_Eventwise,         ///< Stops after a certain number of events
    gcsm_Timed,             ///< Runs to the defined time
} GenCompSimulatorModes_t;


class scGenComp_Simulator : public sc_core::sc_module
{
public:
    SC_HAS_PROCESS(scGenComp_Simulator);
    scGenComp_Simulator(sc_core::sc_module_name nm);
    /**
     * @brief Run simulation as controlled by its parameters
     * @param[in] SimulatorMode mode, one of GenCompSimulatorModes_t modes
     * @param[in] SimulatorUnit a numeric parameter for SimulatorMode
     * @return true if more events are pending
     */
    bool Run(GenCompSimulatorModes_t SimulatorMode, int32_t SimulatorUnit);

    /**
      * @brief Register
      * @param Module the activity of which must be watched
      */
    void RegisterPU(scGenComp_PU_Bio* Module);

    /**
     * @brief Reset the simulator
     *  - simulator's internal variables
     *  - cancels event pending for the !registered! modules
     *  - initializes the !registered! modules
     */
    void Reset(void);

    /**
     * @brief Return true if more events are pending in the simulation system
     */
    bool HasMoreToDo(void){ return mMoreEvents;}

     void Update();
    /**
     * @brief Update the input information in the simulator
     * @param PU the HW
     */
    void UpdateInput(scGenComp_PU_Abstract* PU);
    void UpdateHeartbeat(scGenComp_PU_Abstract* PU);

    void UpdateInitialize(scGenComp_PU_Abstract* PU);
    void UpdateProcessingBegin(scGenComp_PU_Abstract* PU);
    void UpdateProcessingEnd(scGenComp_PU_Abstract* PU);
    void Observe(scGenComp_PU_Abstract* PU, GenCompPUObservingBits_t B);

    std::chrono::duration<int64_t, nano> Time_Get(void)    {return s;}
    /** Return the last reset time */
    sc_core::sc_time scResetTime_Get(){ return mResetTime;}
    /** Return the time since the last reset */
    sc_core::sc_time scTime_Get(){ return sc_core::sc_time_stamp()- mResetTime;}
protected:
    vector<scGenComp_PU_Abstract*> mWatchedPUs;   /// Store the registered objects here
#ifdef BENCHMARK_TIME_BEGIN
    chrono::steady_clock::time_point t;
    std::chrono::duration<int64_t, nano> x,s=(std::chrono::duration< int64_t, nano>)0;
#endif
#ifdef SC_BENCHMARK_TIME_BEGIN
    sc_core::sc_time SC_t, SC_x, SC_s;//, SC_OldT;
#else
    sc_core::sc_time SC_t;
#endif
    sc_core::sc_time
        SC_time,    ///< Needed for the mode 'gcsm_Timed'
        mResetTime; ///< The time of last Reset
    bool mToReset;              ///< If to reset the simulator befor running
    bool mMoreEvents;           ///< If the simulation is still active
    scGenComp_PU_Abstract* mUpdateUnit; ///< Store the unit where event happened
    GenCompPUObservingBits_t mUpdateObservingBit; ///< Stor event mask bit
 };


/** @}*/

#endif // SCGENCOMPSIMULATOR_H

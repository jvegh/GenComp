/** @file scGenComp_PU_Bio.h
 *  @ingroup GENCOMP_MODULE_PROCESS

 *  @brief Function prototypes for the computing module
 *  It is just event handling, no modules
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef SCBIOGENCOMP_H
#define SCBIOGENCOMP_H
/** @addtogroup GENCOMP_MODULE_PROCESS
 *  @{
 */

#include "GenCompStates_Bio.h"
#include "scGenComp_PU_Abstract.h"

#define HEARTBEAT_TIME_DEFAULT_BIO sc_core::sc_time(100,SC_US)

/*
 * \class scGenComp_PU_Bio
 * \brief  A simple abstract class to implement the operation of a general bio computing unit
 *
 * Operating principle of event processing:
 *
 * "XXX_moethod"s are "sensitive" to specific events. After receiving such an an event,
 * the corresponding virtual routine of AbstractGenCompState is called.
 * If the unit's state and the call are not compatible, that routine asserts. (wrong event)
 * Following a normal return, the HW specific operation of the PU is executed,
 * the time delay of the operation is simulated by issuing a wait() call to SystemC kernel.
 * Optionally, also the next state is invoked by issuing a EVENT_GenComp.XXX.notify() notification.
 * The description is valid for methods
 * - Deliver
 * - Process
 * - Relax
 * - Synchronize
 *

 * @see AbstractGenCompState
 *
 * The rest of methods are of technical nature.
 * - HeartBeat: technical signal to update PU's state (e.g. integrate a signal)
 * - Fail: sometheing went wrong, retry
 * - Sleep: if the unit is unused, sends it logically to sleep
 * - Wakeup: awake it if it was sleeping
 */

/*!
 * \class scGenComp_PU_Bio
 * \brief  Implements a general biological-type computing PU
 *
 */
class scGenComp_PU_Bio : public scGenComp_PU_Abstract
{
  public:
    /*!
     * \brief Creates a biological general computing unit
     * @param nm the SystemC name of the module
      *
     * Creates an abstract biological computing unit
     */

    scGenComp_PU_Bio(sc_core::sc_module_name nm);
    virtual ~scGenComp_PU_Bio(void); // Must be overridden
    /**
     * @brief A new spike received
     *
     * A spike arrived, store spike parameters;
     * If it was the first spike, issue 'Begin_Computing'
     *
     * Reimplemented given that the first input also starts processing
     */

    virtual void InputReceived_Do();
    /**
     * @brief Receving an input a momentary action, just administer its processing.
     * It is possible only in 'Ready' and 'Processing' states
     */
//    virtual void DoInputReceive_Do();
     /**
     * @brief Called when the state 'processing' begins
     *
     * The unit passes to phase 'Processing'
     */
//    void ProcessingBegin_method();
    virtual void ProcessingBegin_Do();
    /**
     * @brief Called when the state 'processing' ends
     *
     * The unit passes to phase 'Delivering'
     */
    virtual void ProcessingEnd_Do();
 //   void ProcessingEnd_method();
    /**
     * @brief Deliver
     *
     * Called when the state
     */
    virtual void Deliver();

    /**
     * @brief Relax
     *
     *
     */
    virtual void Relax();


    /**
     * @brief Heartbeat_method
     *
     * A periodic  signal as a timebase for solving differential equations.
     *
     * - The unit receives a signal EVENT_GenComp.HeartBeat and handles it
     *   differently in different modes
     * - In 'Processing' mode, re-calculates membrane's charge-up potential
     * - In 'Ready' mode, re-calculates membrane's decay potential
     * - In 'Delivering' mode, re-calculates membrane's decay potential
      */
//    virtual void Heartbeat_method();
/*     virtual void Synchronize(){assert(0);}
    virtual void Fail(){assert(0);}
*/
  protected:
    /**
     * @brief Puts the PU to its default state (just the HW).
     * Usually called by Initialize_method, but also  by other methods
    */
    virtual void Initialize_Do();

//    virtual bool CanBeginProcessing(void);
    /**
     * @brief Heartbeat
     *
      * Generate
     */
    void Heartbeat();
    /**
     *
     * Handle heartbeats in 'Ready' mode
     */
    void Heartbeat_Ready();
    /**
     * @brief Handle heartbeats in 'Processing' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential reaches its threshold value
     * -  Starts to receive heartbeats after EVENT_GenComp.ProcessingBegin arrives
     * -  Stops generating heartbeats after EVENT_GenComp.ProcessingEnd  arrivies (when reaching the threshold potential
     *    then issues EVENT_GenComp.DeliveringBegin
     * -  Issues EVENT_GenComp.Fail whey the membrane potential decaya near to its threshold
     */
    virtual void Heartbeat_Processing();
    /**
     * @brief Handle heartbeats in 'Delivering' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential at hillock reaches its threshold value
     * -  Starts to receive heartbeats after EVENT_GenComp.DeliveringBegin arrives
     * -  Stops generating heartbeats after EVENT_GenComp.DeliveringEnd arrives
     *     then issues EVENT_GenComp.RelaxingBegin
     */
    void Heartbeat_Delivering();
    /**
     * @brief Handle heartbeats in 'Relaxing' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential starts to increase
     * -  Starts to receive heartbeats after EVENT_GenComp.RelaxingBegin arrives
     * -  Stops generating heartbeats after EVENT_GenComp.DeliveringEnd  arrives
     *     then issues EVENT_GenComp.Initializing
     */
     void Heartbeat_Relaxing();
    /**
     * @brief Calculate_Membrane_Potential
     *
     *  The state of the biological computing is re-calculated (as the simulation time passes)
     *  (solve the differential equation at this time)
     */
    void CalculateMembranePotential();
    /** Checks if membrane is completely depolarized during  'Processing'
     *  return true if depolarized
     */
     bool MembraneThresholdExceeded_Processing(void){return false;}
  };// of class scGenComp_PU_Bio
/** @}*/

#endif // SCBIOGENCOMP_H

/** @file scGenComp_PU_Bio.h
 *  @ingroup GENCOMP_MODULE_BIOLOGY

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

//#include "GenCompStates_Bio.h"
#include "scGenComp_PU_Abstract.h"

#define HEARTBEAT_TIME_DIVISIONS_BIO 8
#define HEARTBEAT_TIME_DEFAULT_BIO sc_core::sc_time(HEARTBEAT_TIME_DIVISIONS_BIO*16,SC_US)

/*
 * \class scGenComp_PU_Bio
 * \brief  A simple abstract class to implement the operation of a general bio computing unit
 *
 * Operating principle of event processing:
 *
 * The non-virtual "XXX_method"s are "sensitive" to specific events. After receiving such an an event,
 * the corresponding virtual routine of "XXX_Do" is called.
 * If the unit's state and the call are not compatible (programming mistake), that routine asserts.
 * (wrong event)
 * The time delay of the operation is simulated by issuing a wait() call to SystemC kernel.
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
 * - Fail: something went wrong, retry
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
     * @param[in] Heartbeat the state refresh time
      *
     * Creates an abstract biological computing unit
     */
    scGenComp_PU_Bio(sc_core::sc_module_name nm   // Module name
                   ,sc_core::sc_time Heartbeat = HEARTBEAT_TIME_DEFAULT_BIO);  // Heartbeat time
    virtual ~scGenComp_PU_Bio(void); // Must be overridden

    /**
     * @brief A new spike received; only in 'Ready' and 'Processing' states
     *
     * A spike arrived, store spike parameters. Receving an input a momentary action, just administer its processing.
     * Most of the job is done in methods Heartbeat_Ready() and Heartbeat_Processing().
     * If it was the first spike, issue 'Begin_Computing'
     *
     * Called by scGenComp_PU_Abstract::InputReceived_method
     * Reimplemented given that in biology the first input also starts processing
     */
    virtual void InputReceived_Do();

    /**
     * @brief Called when the state 'processing' begins
     *
     * The unit passes to phase 'Processing'
     */
    virtual void ProcessingBegin_Do();

    /* *
     * @brief Called when the state 'processing' ends
     *
     * The unit passes to phase 'Delivering'
     */
    //virtual void ProcessingEnd_Do();

    /**
     * @brief Called when the state 'Relaxing' begins
     *
      */
    virtual void RelaxingBegin_Do(){}

    /**
     * @brief Called when the state 'Relaxing' ends
     *
     * The unit passes to phase 'Ready'
     */
    virtual void RelaxingEnd_Do(){}

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
/*     virtual void Synchronize(){assert(0);}
    virtual void Fail(){assert(0);}
*/
    /**
     * @brief IdleTime_Get
     * @return
     */
    sc_core::sc_time LastIdleTime_Get(void)
    { return sc_core::sc_time_stamp() - mLastRelaxingEndTime;}   ///< Remember the beginning of the 'Idle' period
    sc_core::sc_time mLastProcessingTime; ///< Remember last time duration  (the result)

  protected:
    /**
     * @brief Puts the PU to its default state (just the HW).
     * Usually called by Initialize_method, but also  by other methods
    */
    virtual void Initialize_Do();

    /**
     *
     * Handle heartbeats in 'Ready' mode
     */
    void Heartbeat_Ready_Do();
    /**
     * @brief Handle heartbeats in 'Processing' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential reaches its threshold value
     * -  Starts to receive heartbeats after EVENT_GenComp.ProcessingBegin arrives
     * -  Stops generating heartbeats after EVENT_GenComp.ProcessingEnd  arrivies (when reaching the threshold potential
     *    then issues EVENT_GenComp.DeliveringBegin
     * -  Issues EVENT_GenComp.Fail whey the membrane potential decaya near to its threshold
     */
    virtual void Heartbeat_Processing_Do();
    /**
     * @brief Handle heartbeats in 'Delivering' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential at hillock reaches its threshold value
     * -  Starts to receive heartbeats after EVENT_GenComp.DeliveringBegin arrives
     * -  Stops generating heartbeats after EVENT_GenComp.DeliveringEnd arrives
     *     then issues EVENT_GenComp.RelaxingBegin
     */
    void Heartbeat_Delivering_Do();
    /**
     * @brief Handle heartbeats in 'Relaxing' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential starts to increase
     * -  Starts to receive heartbeats after EVENT_GenComp.RelaxingBegin arrives
     * -  Stops generating heartbeats after EVENT_GenComp.DeliveringEnd  arrives
     *     then issues EVENT_GenComp.Initializing
     */
     void Heartbeat_Relaxing_Do();
    /* *
     * @brief SolvePDE
     *
     *  The state of the biological computing is re-calculated (as the simulation time passes)
     *  (solve the differential equation at this time; reset takes place in RelaxingBegin_Do )
     *  @see scGenComp_PU_Bio#RelaxingBegin_Do
     *
     */
 //    virtual void SolvePDE(){};
    /**
     * @brief Processing_Finished
     * @return true if processing finished and 'Delivering' follows
     *
     * true if membrane potential reached its threshold value
     *
     * (return 'true' @500 us)
     */
    virtual bool Processing_Finished(void);
  };// of class scGenComp_PU_Bio
/** @}*/

#endif // SCBIOGENCOMP_H

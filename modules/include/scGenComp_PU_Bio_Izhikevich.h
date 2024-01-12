/** @file scGenComp_PU_Bio_Izhikevich.h
 *  @ingroup GENCOMP_MODULE_BIOLOGY

 *  @brief Function prototypes for the computing module
 *  Implements Izkievits's model
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef SCBIOGENCOMPIZHIKEVICH_H
#define SCBIOGENCOMPIZHIKEVICH_H
/** @addtogroup GENCOMP_MODULE_PROCESS
 *  @{
 */

#include "GenCompStates_Bio.h"
#include "scGenComp_PU_Bio.h"


/*
 * \class scGenComp_PU_Bio_Izhikevich
 * \brief  A simple abstract class to implement the operation of a general bio computing unit
 *
 * Operating principle @see scGenComp_PU_Bio
 *
 * Some virtual methods overwrite the general "BO-Computing" behavior, according to the Izkievitch model
 *
 * @see AbstractGenCompState
 *
 * The rest of methods are of technical nature.
 * - HeartBeat: integrate Izkievits's PDE
  */

/*!
 * \class scGenComp_PU_Bio_Izhikevich
 * \brief  Implements the Izhikevich-type biological computing PU
 *
 */
class scGenComp_PU_Bio_Izhikevich : public scGenComp_PU_Bio
{
  public:
    /*!
     * \brief Creates an Izhikevich-type biological general computing unit
     * @param nm the SystemC name of the module
     * @param[in] Heartbeat the state refresh time
     *
     * Creates  an Izhikevich-type biological computing unit
     *
     *  Izkievitch model:
     *  \f[ \frac{dv}{dt} = 0.04*v^2 + 5*v + 140 - u + I \f]
     *  \f[ \frac{du}{dt} = a( b*v - u) \f]
     *   - \f$ v \f$ the membrane potential (in mV)
     *   - \f$ u \f$ the membrane recovery potential (in mV)
     *   - \f$ t \f$ time (in msec, the local time)
     *   - \f$ a \f$ the time scale of the recovery variable \f$ u \f$. (0.02) Smaller values result in slower recovery.
     *   - \f$ b \f$ the sensitivity of the recovery variable \f$ u \f$ to the subthreshold fluctuations of the membrane potential \f$ v \f$
     *   - \f$ c \f$ after-spike reset value of the membrane potential v. (c = 065 mV)
     *   - \f$ d \f$ after-spike reset of the recovery variable of\f$ u \f$ (d=2)
     */
    scGenComp_PU_Bio_Izhikevich(sc_core::sc_module_name nm   // Module name
                   ,sc_core::sc_time Heartbeat);  // Heartbeat time
    virtual ~scGenComp_PU_Bio_Izhikevich(void); // Must be overridden

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

    /* *
     *
     * Handle heartbeats in 'Ready' mode
     */
 //   void Heartbeat_Ready_Do();
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
    /* *
     * @brief Handle heartbeats in 'Delivering' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential at hillock reaches its threshold value
     * -  Starts to receive heartbeats after EVENT_GenComp.DeliveringBegin arrives
     * -  Stops generating heartbeats after EVENT_GenComp.DeliveringEnd arrives
     *     then issues EVENT_GenComp.RelaxingBegin
     */
 //   void Heartbeat_Delivering_Do();
    /**
     * @brief Handle heartbeats in 'Relaxing' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential starts to increase
     * -  Starts to receive heartbeats after EVENT_GenComp.RelaxingBegin arrives
     * -  Stops generating heartbeats after EVENT_GenComp.DeliveringEnd  arrives
     *     then issues EVENT_GenComp.Initializing
     */
     void Heartbeat_Relaxing_Do();
    /**
     * @brief SolvePDE : solve the differential equations for the actual time increment
     *
     *  Izhikevich model:
     *  \f[ \frac{dv}{dt} = 0.04*v^2 + 5*v + 140 - u + I \f]
     *  \f[ \frac{du}{dt} = a( b*v - u) \f]
     *   - \f$ v \f$ the membrane potential (in mV)
     *   - \f$ u \f$ the membrane recovery potential (in mV)
     *   - \f$ t \f$ time (in msec, the local time)
     *   - \f$ a \f$ the time scale of the recovery variable \f$ u \f$. (0.02) Smaller values result in slower recovery.
     *   - \f$ b \f$ the sensitivity of the recovery variable \f$ u \f$ to the subthreshold fluctuations of the membrane potential \f$ v \f$
     *   - \f$ c \f$ after-spike reset value of the membrane potential v. (c = 065 mV)
     *   - \f$ d \f$ after-spike reset of the recovery variable of\f$ u \f$ (d=2)

     *
     *  The state of the biological computing is re-calculated (as the simulation time passes)
     *  (solve the differential equation at this time; reset takes place in RelaxingBegin_Do )
     *  @see scGenComp_PU_Bio#RelaxingBegin_Do
     */
    virtual void SolvePDE();
    /**
     * @brief Processing_Finished
     * @return true if processing finished and 'Delivering' follows
     *
     * true if membrane potential reached its threshold value
     *
     * (return 'true' @500 us
     */
    virtual bool Processing_Finished(void);

  };// of class scGenComp_PU_Bio_Izhikevich
/** @}*/

#endif // SCBIOGENCOMPIZHIKEVICH_H

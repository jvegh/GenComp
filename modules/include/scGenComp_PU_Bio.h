/** @file scBioGenComp_PU.h
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

#include "BioGenCompStates.h"
#include "scGenComp_PU_Abstract.h"

#define BIO_HEARTBEAT_TIME sc_core::sc_time(10,SC_US)

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
    virtual void Initialize_method();
    /**
     * @brief A new spike received
     *
     * A spike arrived, store spike parameters;
     * If it was the first spike, issue 'Begin_Computing'
     */
    virtual void InputReceived_method();
    /**
     * @brief Receving an input a momentary action, just administer its processing.
     * It is possible only in 'Ready' and 'Processing' states
     */
    virtual void ReceiveInput();
    /**
     * @brief Deliver
     */
    virtual void Deliver();

    /**
     * @brief Heartbeat_method
     *
     * A periodic  signal as a timebase for solving differential equations.
     *
     * - To economize resources, a central signal is used, but it is NOT a central clock signal
     * - The unit receives a signal EVENT_GenComp.HeartBeat
     * - If in 'Processing' mode
     *   - The sensitivity is ON from EVENT_GenComp.Begin_Computing in 'Processing' mode
     *   - The sensitivity if OFF from EVENT_GenComp.Relaxed in "Ready" mode
     */
    virtual void Heartbeat_method();
/*    virtual void Relax(){assert(0);}
    virtual void Reinitialize(){assert(0);}
    virtual void Synchronize(){assert(0);}
    virtual void Fail(){assert(0);}
*/
  protected:
    /**
     * @brief Puts the PU to its default state (just the HW).
    */
    virtual void Initialize();
//    virtual bool CanBeginProcessing(void);
    /**
     * @brief Heartbeat
     *
     * The state of the biological computing is re-calculated (as the simulation time passes)
     * - If called in 'Processing' mode
     *   Re-issues EVENT_GenComp.Heartbeat until membrane potential reaches its threshold value
     *   Issues EVENT_GenComp.Begin_Computing upon the arrival of the first spike
     *   Issues EVENT_GenComp.End_Computing upon arriving the threshold potential (and passes to 'Delivering')
     *   Issues EVENT_GenComp.Fail upon decreasing the membrane potential
     * - if called in 'Ready' mode
     *   Re-issues EVENT_GenComp.Hearbeat until membrane potential decreased to its resting value
     * Generate
     */
    virtual void Heartbeat();
  };// of class scGenComp_PU_Bio
/** @}*/

#endif // SCBIOGENCOMP_H

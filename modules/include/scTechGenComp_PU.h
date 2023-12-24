/** @file scTechGenComp_PU.h
 *  @ingroup GENCOMP_MODULE_PROCESS

 *  @brief Function prototypes for the computing module
 *  It is just event handling, no modules
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef SCTECHGENCOMP_H
#define SCTECHGENCOMP_H
/** @addtogroup GENCOMP_MODULE_PROCESS
 *  @{
 */
#include "scAbstractGenComp_PU.h"
#include "TechGenCompStates.h"
static vector<AbstractGenCompState*> TechPU_StateVector;

/* !
 * \class scTechGenComp_PU
 * \brief  A simple abstract class to deal with the operation using states of a general computing unit
 *
 * Operating principle of event processing:
 *
 * "XXX_thread" methods are "sensitive" to specific events. After receiving an event,
 * the corresponding virtual routine of AbstractGenCompState is called.
 * If the old and the new states are not compatible, that routine asserts. (wrong event)
 * Following a normal return, the HW specific operation of the PU is executed,
 * the time delay of the operation is simulated by issuing a wait() call to SystemC kernel.
 * Optionally, also the next state is invoked by issuing a EVENT_GenComp.XXX.notify() notification.
 * The description is valid for methods
 * - Deliver
 * - Process
 * - Relax
 * - Synchronize
 *
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
 * \class scTechGenComp_PU
 * \brief  Implements a general technical-type computing
 *
 * The technical PUs need all arguments at the beginning
 */
class scTechGenComp_PU : public scAbstractGenComp_PU
{
  public:
    /*!
     * \brief Implements a general technical-type computing
     * @param nm the SystemC name of the module
     * @param No Number of parameters to compute with
     *
     */

    scTechGenComp_PU(sc_core::sc_module_name nm, int32_t No);
    virtual ~scTechGenComp_PU(); // Must be overridden
    virtual void Initialize_method();
    /**
      * @brief Check if we can begin processing
      * @return true if we can begin processing
      */
    virtual bool CanBeginProcessing(void);

  protected:
    int32_t mNoOfArgs;    // The number of args before computation can start
 };// of class scTechGenComp_PU

/** @}*/

#endif // SCTECHGENCOMP_H

/** @file scGenComp_PU_Tech.h
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
#include "scGenComp_PU_Abstract.h"
//#include "GenCompStates_Tech.h"
//static vector<GenCompStates_Abstract*> TechPU_StateVector;

/* !
 * \class scGenComp_PU_Tech
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
 * \class scGenComp_PU_Tech
 * \brief  Implements a general technical-type computing
 *
 * The technical PUs need all arguments at the beginning
 */
class scGenComp_PU_Tech : public scGenComp_PU_Abstract
{
  public:
    /*!
     * \brief Implements a general technical-type computing
     * @param nm the SystemC name of the module
     * @param No Number of parameters to compute with
     *
     */

    scGenComp_PU_Tech(sc_core::sc_module_name nm, int32_t No);
    virtual ~scGenComp_PU_Tech(); // Must be overridden
    /**
     * @brief Initialize the technical PU
     *
     *
     */
    virtual void Initialize_method();

  protected:
    int32_t mNoOfArgs;    // The number of args before computation can start
 };// of class scGenComp_PU_Tech

/** @}*/
#undef SC_MAKE_TIME_BENCHMARKING
#undef MAKE_TIME_BENCHMARKING

#endif // SCTECHGENCOMP_H

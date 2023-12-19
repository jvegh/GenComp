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
#include "scAbstractGenComp_PU.h"

static vector<AbstractGenCompState*> BioPU_StateVector;

/*
 * \class scAbstractGenComp_PU
 * \brief  A simple abstract class to deal  with states of a general computing unit
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

 * @see AbstractGenCompState
 *
 * The rest of methods are of technical nature.
 * - HeartBeat: technical signal to update PU's state (e.g. integrate a signal)
 * - Fail: sometheing went wrong, retry
 * - Sleep: if the unit is unused, sends it logically to sleep
 * - Wakeup: awake it if it was sleeping
 */

/*!
 * \class scBioGenComp_PU
 * \brief  Implements a general biological-type computing PU
 *
 */
class scBioGenComp_PU : public scAbstractGenComp_PU
{
  public:
    /*!
     * \brief Creates a biological abstract processing unit
     * @param nm the SystemC name of the module
      *
     * Creates an abstract biological computing unit
     */

    scBioGenComp_PU(sc_core::sc_module_name nm);
    virtual ~scBioGenComp_PU(void); // Must be overridden
/*    virtual void Deliver(){assert(0);}
    virtual void HeartBeat(){assert(0);}*/
    /**
     * @brief Process
     *
     * In biological computing,
     */
    virtual void Process_method();
/*    virtual void Relax(){assert(0);}
    virtual void Reinitialize(){assert(0);}
    virtual void Synchronize(){assert(0);}
    virtual void Fail(){assert(0);}
*/
  protected:
 };// of class scBioGenComp_PU
/** @}*/

#endif // SCBIOGENCOMP_H

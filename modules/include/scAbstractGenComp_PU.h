/** @file scAbstractGenComp_PU.h
 *  @ingroup GENCOMP_MODULE_PROCESS

 *  @brief Function prototypes for the computing module
 *  It is just event handling, no modules
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef SCABSTRACTGENCOMP_H
#define SCABSTRACTGENCOMP_H
/** @addtogroup GENCOMP_MODULE_PROCESS
 *  @{
 */
#include "GenCompStates.h"

#include <systemc>
using namespace sc_core; using namespace sc_dt;
 using namespace std;
static vector<AbstractGenCompState*> PU_StateVector;

#ifndef SCBIOGENCOMP_H
#ifndef SCTECHGENCOMP_H // Just to exclude for Doxygen

/*!
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
 *
 * @see AbstractGenCompState
 *
 * The rest of methods are of technical nature.
 * - HeartBeat: technical signal to update PU's state (e.g. integrate a signal)
 * - Fail: sometheing went wrong, retry
 * - Sleep: if the unit is unused, sends it logically to sleep
 * - Wakeup: awake it if it was sleeping
 */
#endif //SCTECHGENCOMP_H
#endif //SCBIOGENCOMP_H
class scAbstractGenComp_PU: public sc_core::sc_module
{
    friend class AbstractGenCompState;
  public:
    /*!
     * \brief
     *
     * Creates an abstract processing unit for the general computing paradigm
     * @param nm the SystemC name of the module
      *
     */

    scAbstractGenComp_PU(sc_core::sc_module_name nm);

    virtual ~scAbstractGenComp_PU(void); // Must be overridden
    virtual void Deliver_method();
    virtual void Heartbeat_method();
    virtual void Process_method();
    virtual void Relax_method();
    virtual void Initialize_method();
    virtual void Synchronize_method();
    virtual void Fail_method();
    /*!
     * \brief Sleep_method: the unit can auto-power-off if idle for a longer time.
     * It must be previously in 'Ready' state
     *
     */
    virtual void Sleep_method();
    virtual void Wakeup_method();
    void StateFlag_Set(GenCompStateMachineType_t S){    StateFlag = S;}
    GenCompStateMachineType_t StateFlag_Get(void){return StateFlag;}
     struct{
        sc_core::sc_event
                // Operation-related
            Begin_Computing,        // Time to begin computing
            End_Computing,          // Time to end computing
            Begin_Transmission,     // Start to send the result
            End_Transmission,       // Feedback from transmission unit
            Initialize,             // Put the unit to its ground state
            Synchronize,            // External synhronize signal
            Process,                // Make a new processing
            Deliver,                // Deliver result tp the 'output section'
            Relax,                  // Make a short coffe break
                // HW-related
            HeartBeat,              // Internal timekeeping
            Fail,                   // Computing failed, start over
            Sleep,                  // Send the HW to sleep
            Awaken,                 // Signals that is ready to use
            Wakeup;                  // The HW is needed again, awake it
        }
        EVENT_GenComp;
  protected:
    /**
     * @brief Puts the PU to its default state (just the HW)
    */
    virtual void Initialize();
    AbstractGenCompState* MachineState;     ///< Points to the service object of the state machine
    GenCompStateMachineType_t StateFlag;    ///< preserves last state
 };// of class scAbstractGenComp_PU


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
    /**
     * @brief Process
     */
    virtual void Process_method();

  protected:
    int32_t mNoOfArgs;    // The number of args before computation can start
 };// of class scTechGenComp_PU

/*!
 * \class scBioGenComp_PU
 * \brief  Implements a general biological-type computing
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

#endif // SCABSTRACTGENCOMP_H

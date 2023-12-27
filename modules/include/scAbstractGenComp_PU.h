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
static vector<AbstractGenCompState*> AbsPU_StateVector;


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
     * \brief Create an abstract processing unit for the general computing paradigm
     * @param nm the SystemC name of the module
     *
     */

    scAbstractGenComp_PU(sc_core::sc_module_name nm);

    virtual ~scAbstractGenComp_PU(void); // Must be overridden

     /**
      * @brief The physical delivery method
      *
      * The delivery method is called twice; at the beginning and at the end of the delivery phase
      * The first phase is executed in state 'Processing', the 2nd one in state 'Delivering'
      * The routine is actived by EVENT_GenComp.End_Computing
      */
    virtual void Deliver();
    virtual void Relax_method();
    virtual void Initialize_method();
    /**
     * @brief InputReceived_method
     *
     * An external partner signalled that an input was sent
     */
    virtual void InputReceived_method();
    /**
     * @brief Receving an input a momentary action, just administer its processing.
     * It is possible only in 'Ready' and 'Processing' states
     */
    virtual void ReceiveInput();
    /**
     * @brief Synchronize the PU to some external condition
     * - In biological computing, it results in issuing 'End Computing' (immediate spiking; passing to 'Delivering')
     * - In technical computing, it results in issuing 'Begin Computing' (starting computing; passing to 'Processing')
    */
    virtual void Synchronize_method();
    /**
     * @brief In 'Processing' state the unit can issue a signal 'fail'
     */
    virtual void Fail_method();
    /**
     * @brief Fail
     * - In biological computing, it means a failed charging-up; passes to 'Ready state'
     * - In technical computing, it results a failed operation
     */
    virtual void Fail();

#ifdef USE_PU_HWSLEEPING
    /*!
     * \brief Sleep_method: the unit can auto-power-off if idle for a longer time.
     * It must be previously in 'Ready' state
     *
     */
    virtual void Sleep_method();
    /**
     * @brief The unit can be powered on
     * - On demand when in state  'Processing'
     * - Explicitely when in state 'Sleeping'
     */
    virtual void Wakeup_method();
    virtual void Wakeup();
    /**
     * @brief Drives Sleep/Awake activity
     * Advances counters in the PU
     */
    virtual void Clock_method();
#endif //USE_PU_HWSLEEPING
    void StateFlag_Set(GenCompStateMachineType_t S){    mStateFlag = S; }
    GenCompStateMachineType_t StateFlag_Get(void){return mStateFlag;}
     struct{
        sc_core::sc_event
                // Operation-related
            Begin_Computing,        // To begin computing
            End_Computing,          // To end computing
            Begin_Transmission,     // Start to send the result
            End_Transmission,       // Feedback from transmission unit

            Begin_Sleeping,
            End_Sleeping,
            Initialize,             // Put the unit to its ground state
            InputReceived,          // New input received
            Synchronize,            // External synhronize signal
            Relax,                  // Make a short coffe break
            HeartBeat,             // Refresh PU's state
                // HW-related
            Fail,                   // Computing failed, start over
            Sleep,                  // Send the HW to sleep
            Awaken,                 // Signals that is ready to use
            Relaxed,               // Signals that the unit reached its resting potential
            Wakeup;                  // The HW is needed again, awake it
        }
        EVENT_GenComp;
     /**
     * @brief scLocalTime_Get
     * @return The simulated time since the beginning of the recent operation
     */
    sc_core::sc_time scLocalTime_Get(){return sc_core::sc_time_stamp()-mLocalTimeBase;}
    /**
     * @brief scLocalTime_Set
     * @param T The beginning of the simulated time of the recent operation
     */
    void scLocalTime_Set(sc_core::sc_time T = sc_core::sc_time_stamp()){    mLocalTimeBase = T;}
    sc_core::sc_time scTimeBase_Get(void){return mLocalTimeBase;}
     size_t NoOfInputsReceived_Get(){ return Inputs.size();}
  protected:
    AbstractGenCompState* MachineState;     ///< Points to the service object of the state machine
    /**
     * @brief Puts the PU to its default state (just the HW).
    */
    virtual void Initialize();
    GenCompStateMachineType_t mStateFlag;    ///< preserves last state
     int32_t mNoOfInputsNeeded;
    sc_core::sc_time mLocalTimeBase;    // The beginning of the local computing
    vector<int32_t> Inputs; // Stores reference to input source
 };// of class scAbstractGenComp_PU

/** @}*/

#endif // SCABSTRACTGENCOMP_H

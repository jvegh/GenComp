/** @file scGenComp_PU_Abstract.h
 *  @ingroup GENCOMP_MODULE_PROCESS
 *
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
#include "GenCompStates_Abstract.h"

#include <systemc>
#include <bitset>
using namespace sc_core; using namespace sc_dt;
using namespace std;
static vector<GenCompStates_Abstract*> AbsPU_StateVector;

/**
 *
 * @struct EVENT_GenComp_type
 *  @brief The events used by scGenComp_PU_Abstract_xxx classes
 *  @var EVENT_GenComp_type::DeliveringBegin
 *  Begin delivering the result (internally)
 *  @var EVENT_GenComp_type::DeliveringEnd
 *  Ended delivering the result (internally)
 *  @var EVENT_GenComp_type::ProcessingBegin
 *  Begin data processing
 *  @var EVENT_GenComp_type::ProcessingEnd
 *  End data processing
 *  @var EVENT_GenComp_type::RelaxingBegin
 *  Begin restoring the 'Ready' state
 *  @var EVENT_GenComp_type::RelaxingEnd
 *  End restoring the 'Ready' state
 *  @var EVENT_GenComp_type::TransmittingBegin
 *  Begin transmitting the result (externally); unattended
 *  @var EVENT_GenComp_type::TransmittingEnd
 *  Feedback from transmission unit; unattended
 *
 *  @var EVENT_GenComp_type::SleepingBegin
 *  Begin of HW sleepig
 *  Only in 'Ready' state, the PU can spare power by going to 'Sleeping' state
 *  @var EVENT_GenComp_type::SleepingEnd,
 *  End of HW spleeping
 *  @var EVENT_GenComp_type::Synchronize
 *  External event forces PU to synchronize
 *  @var EVENT_GenComp_type::Fail
 *  Computing failed, start over
 *  @var EVENT_GenComp_type::Initialize,             // Put the unit to its ground state
 *  @var EVENT_GenComp_type::InputReceived,          // New input received
 */

struct EVENT_GenComp_type {
    sc_core::sc_event
        // Operation-related
        DeliveringBegin,        // Begin delivering the result (internally)
        DeliveringEnd,          // End delivering
        Fail,                    // Computing failed, start over
        Heartbeat,               // Refresh PU's state
        Initialize,              // Put the unit to its ground state
        InputReceived,           // New input received
        ProcessingBegin,        // Begin data processing
        ProcessingEnd,          // End data processing
        //            Relax,                   // Begin resetting the unit
        RelaxingBegin,          // Begin restoring the 'Ready' state
        RelaxingEnd,            // End restoring the 'Ready' state
        SleepingBegin,
        SleepingEnd,
        Sleep,
        Synchronize,             // External synhronize signal

        TransmittingBegin,      // Begin transmitting the result (externally); unattended
        TransmittingEnd,        // Feedback from transmission unit; unattended
        PotentialRestored,      //
        // HW-related
        Awaken,                  // Signals that is ready to use
        Wakeup;                  // The HW is needed again, awake it
};

/*

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
 * - Transmit
 * - Synchronize
 *

*/
#ifndef SCBIOGENCOMP_H
#ifndef SCTECHGENCOMP_H // Just to exclude for Doxygen


/*!
 * \class scGenComp_PU_Abstract
 * \brief  A simple abstract class to deal  with states of a general computing unit.
 * The unit implements a one-shot elementary computation. The statios  of computing
 * are marked be event pairs
 * - Between events ProcessingBegin and ProcessingEnd the PU is in state 'Processing'
 * - Between events DeliveringBegin and DeliveringEnd the PU is in state 'Delivering'
 * - Between events RelaxingBegin and RelaxingEnd the PU is in state "Relaxing"
 * - Events TransmittingBegin and TransmittingEnd belong to the fellow transmitter unit
 *
 * The PU has correspondingly the states 'Processing', 'Delivering' and 'Relaxing'
 * as the minimum necessary basic states. Technically, the state 'Ready' is also needed,
 * where the PU starts from and arrives to. The module has a single-shot normal operating mode
 * that the operation is automatic between the corresponding xxxBegin and xxxEnd events,
 * and at the end of the xxxEnd events the corresponding xxxBegin event is issued.
 *
 * To initiate a one-shot operation while in 'Ready' statio, different methods are used by the derived classes.
 * - PU_Bio needs a single input spike to begin
 * - PU_Tech needs an external clock signal
 * - PU_Asynch needs the arrival of  ALL needed operands
 *
 * In all subclasses, there exists EVENT_GenComp_type events
  *
 * @see AbstractGenCompState
 *
 * The rest of methods are of technical nature.
 * - Heartbeat: technical signal to update PU's state (e.g. integrate a signal)
 * - Fail: sometheing went wrong, retry
 * - Sleep: if the unit is unused, sends it logically to sleep
 * - Wakeup: awake it if it was sleeping
 */
#endif //SCTECHGENCOMP_H
#endif //SCBIOGENCOMP_H

extern string GenCompStatesString[];
/*! \var typedef GenCompPUOperatingBits_t
 * \brief the names of the bits in the bitset describing scGenComp_PU_Abstract
 */
typedef enum
{
    gcob_ObserveModule,   ///< The scGenComp_PU_Abstract is observed (by the simulator)
    gcob_ObserveComputingBegin,  ///< Watch 'Begin Computing'
    gcob_ObserveComputingEnd,
    gcob_ObserveHeartbeat,      ///< Observe 'Heartbeat's of the PU
    gcob_ObserveInput,      ///< Observe 'Input's of the PU
    gcob_Max        // just maintains the number of bits used
} GenCompPUOperatingBits_t;


class scGenComp_PU_Abstract: public sc_core::sc_module
{
    friend class GenCompStates_Abstract;
  public:
    /*!
     * \brief Create an abstract processing unit for the general computing paradigm
     * @param nm the SystemC name of the module
     *
     */

    scGenComp_PU_Abstract(sc_core::sc_module_name nm);

    virtual ~scGenComp_PU_Abstract(void); // Must be overridden

    /**    If this macro is not defined, no code is generated;
  the variables, however, must be defined (although they will be
  optimized out as unused ones).
  Alternatively, the macros may be protected with "\#ifdef MAKE_TIME_BENCHMARKING".
  The macros have source-module scope. All variables must be passed by reference.

     * @brief The HW initialization
    */
    virtual void DoInitialize();

    /**
      * @brief The physical delivery method
      *
      * The delivery method is called twice; at the beginning and at the end of the 'delivery'Delivering' phase
      * The first phase is executed in state 'Processing', the 2nd one in state 'Delivering'
      * The routine is actived by EVENT_GenComp.End_Processing
      */
    virtual void Deliver();
     /**
     * @brief Delivering has started
     *
     * Internal result delivering has started
     */
    virtual void DeliveringBegin_method();


    /**
     * @brief Delivering has finished
     *
     * Internal result delivering has finished
     */
    virtual void DeliveringEnd_method();

    virtual void Fail_method();
    /**
     * @brief Fail
     * - In biological computing, it means a failed charging-up; passes to 'Ready state'
     * - In technical computing, it results a failed operation
     */
    virtual void Fail();

    /**
     * @brief Heartbeat_method
     *
     * A periodic signal as a timebase to refresh state; to avoid using clock signals
     *
     *   - For technical computing, it is used to display time delays
     *   - For biological computing, it is used to refresh PU's state
     *   - The unit receives a signal EVENT_GenComp.HeartBeat
     * and branches according to PU's state
     *
     * A heartbeat can be usesful in states
     * Ready : the potential is leaking
     * Processing: the potencial is leaking and charging up
     * Delivering: the membrane voltage builds up
     * Relaxing: the membrane potential decays
     *
     * Calls
     *      virtual void Heartbeat_Ready();
     *      virtual void Heartbeat_Processing();
     *      virtual void Heartbeat_Delivering();
     *      virtual void Heartbeat_Relaxing();
      */
    void Heartbeat_method();


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
    virtual void DoInputReceive();

    /**
     * @brief Processing has finished
     *
     */
    virtual void ProcessingEnd_method();

    /**
     * @brief Processing has started
     *
     */
    virtual void ProcessingBegin_method();
    void ProcessingBegin();
    void ProcessingEnd();

    /** @brief The PU is ready to operate
     *
     *  Is sensitive to events Initialize
     */
    void Ready_method(void);

    /**
     * @brief Relax_method
     *
     * - Sensitive to
     *
     * No input received; basically just spends time
     * (plus resets unit to "Ready" state
     *
     */
    /**
      * @brief Resetting the unix to its operating state
      *
      * The resetting method is called twice; at the beginning and at the end of the 'Relaxing' phase
      * The first phase is executed in state 'Delivering', the 2nd one in state 'Relaxing'
      * The routine is actived by EVENT_GenComp.End_Delivering
     */

    virtual void Relax_method(){ assert(0);};
    virtual void Relax();
    /**
     * @brief Relaxing has started
     *
     * After delivered the result internally to the 'output section', resetting begins
     */
    virtual void RelaxingBegin_method();
    /**
     * @brief Relaxing has finished
     *
     * After delivered the result internally to the 'output section', resetting begins
     */
    virtual void RelaxingEnd_method();


     /**
     * @brief Synchronize the PU to some external condition
     * - In biological computing, it results in issuing 'End Computing' (immediate spiking; passing to 'Delivering')
     * - In technical computing, it results in issuing 'Begin Computing' (starting computing; passing to 'Processing')
    */

    virtual void Synchronize_method();
    /**
     * @brief In 'Processing' state the unit can issue a signal 'fail'
     */
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
#endif //USE_PU_HWSLEEPING
    void StateFlag_Set(GenCompStateMachineType_t S){    mStateFlag = S; }
    GenCompStateMachineType_t StateFlag_Get(void){return mStateFlag;}
    EVENT_GenComp_type EVENT_GenComp;
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
     /*!
     * \brief Set an operating bit for this scGenComp_PU_Abstract
     * \param B is the bit to set
     * \param V is the requested value of the bit
     */
    void OperatingBit_Set(GenCompPUOperatingBits_t B, bool V)
     {
        assert(B < gcob_Max);
        mGenCompPUOperatingBits[B] = V;
    }

     /*!
     * \brief Set an operating bit for this scGenComp_PU_Abstract
     * \param B is the bit to set
     * \return is the requested value of the bit
     */
     bool OperatingBit_Get(GenCompPUOperatingBits_t B)
     {
         assert(B < gcob_Max);
         return mGenCompPUOperatingBits[B];
     }
     void GetData(int32_t &A){ A = 1234;}
   protected:
    GenCompStates_Abstract* MachineState;     ///< Points to the service object of the state machine
    /**
     * @brief Heartbeat_Ready
     *
     * Called by Heartbeat_method();
     *
     * They should be overloaded (by default make nothing)
     */
    virtual void Heartbeat_Ready(){};
    virtual void Heartbeat_Processing(){};
    virtual void Heartbeat_Delivering(){};
    virtual void Heartbeat_Relaxing(){};
    GenCompStateMachineType_t mStateFlag;    ///< preserves last state
    int32_t mNoOfInputsNeeded;
    sc_core::sc_time mLocalTimeBase;    // The beginning of the local computing
    vector<int32_t> Inputs; // Stores reference to input source
private:
    bitset<gcob_Max>
        mGenCompPUOperatingBits;   ///< The bits of the GenComp_PU state
 };// of class scGenComp_PU_Abstract

/** @}*/

#endif // SCABSTRACTGENCOMP_H

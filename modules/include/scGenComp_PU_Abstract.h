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
class scGenComp_Simulator;

#define HEARTBEAT_TIME_DEFAULT sc_core::sc_time(1,SC_NS)


//
/* Actually, the following main duties

Plus 2 adattípus a generáláskor: az egyik a szál, kommunikációra, a másik a megjelenítésre
*/

/**
 *  @struct EVENT_GenComp_type
 *  @brief The events used by scGenComp_PU_Abstract classes
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
 *  @var EVENT_GenComp_type::Failed
 *  Computing failed, start over
 *  @var EVENT_GenComp_type::Initialize,             // Put the unit to its ground state
 *  @var EVENT_GenComp_type::InputReceived,          // New input received
 */

struct EVENT_GenComp_type {
    sc_core::sc_event
        // Operation-related
        DeliveringBegin,        // Begin delivering the result (internally)
        DeliveringEnd,          // End delivering
        Failed,                 // Computing failed, start over
        Heartbeat,              // Refresh PU's state
        Initialize,             // Put the unit to its ground state
        InputReceived,          // New input received
        ProcessingBegin,        // Begin data processing
        ProcessingEnd,          // End data processing
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
 * Following a normal return, the HW specific operation of the \gls{PU} is executed,
 * the time delay of the operation is simulated by issuing a wait() call to SystemC kernel.
 * Optionally, also the next state is invoked by issuing a EVENT_GenComp.XXX.notify() notification.
 * The description is valid for methods
 * - Deliver
 * - Process
 * - Relax
 * - Transmit
 * - Synchronize
 */
#ifndef SCBIOGENCOMP_H
#ifndef SCTECHGENCOMP_H // Just to exclude for Doxygen


/*!
 * \class scGenComp_PU_Abstract
 * \brief  A simple abstract class to deal  with operations of a general computing unit.
 * The unit implements a one-shot elementary computation. The statios  of computing
 * are marked be event pairs
 * - Between events ProcessingBegin and ProcessingEnd the \gls{PU} is in state 'Processing'
 * - Between events DeliveringBegin and DeliveringEnd the \gls{PU} is in state 'Delivering'
 * - Between events RelaxingBegin and RelaxingEnd the \gls{PU} is in state "Relaxing"
 * - Events TransmittingBegin and TransmittingEnd belong to the fellow transmitter unit
 *
 * The \gls{PU} has correspondingly the states 'Processing', 'Delivering' and 'Relaxing'
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
 * In all subclasses, there exist EVENT_GenComp_type events
 *
 * @see GenCompStates_Abstract
 *
 * The rest of methods are of technical nature.
 * - Heartbeat: technical signal to update PU's state (e.g. integrate a signal)
 * - Failed: something went wrong, retry
 * - Sleep: if the unit is unused, sends it logically to sleep
 * - Wakeup: awake it if it was sleeping
 */
#endif //SCTECHGENCOMP_H
#endif //SCBIOGENCOMP_H

extern string GenCompStatesString[];
/*! \var typedef GenCompPUObservingBits_t
 * \brief the names of the bits in the bitset describing scGenComp_PU_Abstract
 */
typedef enum
{
    gcob_ObserveGroup,             ///< The scGenComp_PU_Abstract is observed (by the simulator)
    gcob_ObserveModule,             ///< The scGenComp_PU_Abstract is observed (by the simulator)
    gcob_ObserveProcessingBegin,     ///< Observe 'Begin Computing'
    gcob_ObserveProcessingEnd,       ///< Observe 'End Computing'
    gcob_ObserveDeliveringBegin,     ///< Observe 'Begin Delivering'
    gcob_ObserveDeliveringEnd,       ///< Observe 'End Delivering'
    gcob_ObserveHeartbeat,          ///< Observe 'Heartbeat's of the PU
    gcob_ObserveInput,              ///< Observe 'Input's of the PU
    gcob_ObserveInitialize,         ///< Observe 'Initialize's of the PU
    gcob_Max        // just maintains the number of bits used
} GenCompPUObservingBits_t;
extern string GenCompObserveStrings[];


class scGenComp_PU_Abstract: public sc_core::sc_module
{
    friend class GenCompStates_Abstract;
  public:
    /*!
     * \brief Create an abstract processing unit for the general computing paradigm
     * @param[in] nm the SystemC name of the module
     */
    scGenComp_PU_Abstract(sc_core::sc_module_name nm);

    virtual ~scGenComp_PU_Abstract(void); // Must be overridden

    /**
     * @brief CancelEvents
     *
     * When 'Reset' requested, cancels all pending events
     */
    void CancelEvents(void);

    /* *
      * @brief The physical delivery method
      *
      * The delivery method is called twice; at the beginning and at the end of the 'delivery'Delivering' phase
      * The first phase is executed in state 'Processing', the 2nd one in state 'Delivering'
      * The routine is actived by EVENT_GenComp.End_Processing
      *
    virtual void Deliver();*/
     /**
     * @brief Delivering has started
     *
     * Internal result delivering has started.
     * Usually activated by         EVENT_GenComp.DeliveringBegin,        // Begin delivering the result (internally)

     */
    virtual void DeliveringBegin_method();


    /**
     * @brief Delivering has finished
     *
     * Internal result delivering has finished.
     * Usually activated by         EVENT_GenComp.DeliveringEnd,        // Begin delivering the result (internally)
     */
    virtual void DeliveringEnd_method();

    /**
     * @brief Module failed
     * Usually activated by         EVENT_GenComp.Failed,        // module failed to compute
     * - In biological computing, it means a failed charging-up; passes to 'Ready state'
     * - In technical computing, it results a failed operation
     */
    virtual void Failed_Do();
    void Failed_method();


    /**
     * @brief Heartbeat_method
     *
     * A periodic signal as a timebase to refresh state; to avoid using clock signals
     * Usually activated by      EVENT_GenComp.Heartbeat,        // Represeh PUs state
     *
     *   - For technical computing, it is used to display time delays
     *   - For biological computing, it is used to refresh PU's state
     *   - The unit receives a signal EVENT_GenComp.HeartBeat
     * and branches according to PU's state
     *
     * A heartbeat can be useful in states
     * Ready : the potential is leaking
     * Processing: the potencial is leaking and charging up
     * Delivering: the membrane voltage builds up
     * Relaxing: the membrane potential decays
     *
     * Calls
     *      virtual void Heartbeat_Ready_Do();
     *      virtual void Heartbeat_Processing_Do();
     *      virtual void Heartbeat_Delivering_Do();
     *      virtual void Heartbeat_Relaxing_Do();
      */
    void Heartbeat_method();

    /**
     * @brief Initialize_method
     *
     * The method called upon EVENT_GenComp.Initialize
     * Usually activated by      EVENT_GenComp.Initialize,       // Put the unit to its ground state
     */
    void Initialize_method();
    /**
     * @brief The HW initialization
    */
    virtual void Initialize_Do();

    /**
     * @brief InputReceived_method
     *
     * An external input received to the terminal.
     * Usually activated by      EVENT_GenComp.InputReceived,       // Put the unit to its ground state
     */
     void InputReceived_method();
    /**
     * @brief Prepare external input for processing
     * Usually activated by      EVENT_GenComp.InputReceived,          // New input received
     */
    virtual void InputReceived_Do(void);

    /**
     * @brief Processing has started
     *
     * Usually activated by      EVENT_GenComp.ProcessingBegin,        // Begin data processing
     */
    void ProcessingBegin_method();
    virtual void ProcessingBegin_Do();

    /**
     * @brief Processing has finished
     * Usually activated by         EVENT_GenComp.ProcessingEnd,          // End data processing
     *
     */
    void ProcessingEnd_method();
    virtual void ProcessingEnd_Do();

    /**
     * @brief Resetting the unit to its operating state
     *
     * No input received; basically just spends time
     * (plus resets unit to "Ready" state)
     *
     * The routine is actived by EVENT_GenComp.End_Delivering
     */
    void Reset(void);

     /**
     * @brief Relaxing has started
     *
     * Usually activated by RelaxingBegin,          // Begin restoring the 'Ready' state
     * After delivered the result internally to the 'output section', resetting begins
     */
    void RelaxingBegin_method();
    virtual void RelaxingBegin_Do(){};

    /**
     * @brief Relaxing has finished
     *
     * Usually activated by    EVENT_GenComp.RelaxingEnd,            // End restoring the 'Ready' state
     * After delivered the result internally to the 'output section', resetting begins
     */
    void RelaxingEnd_method();
    virtual void RelaxingEnd_Do();

 /*
    struct _type {
        sc_core::sc_event
            * Usually activated by      EVENT_GenComp.Initialize,       // Put the unit to its ground state

            * Usually activated by      SleepingBegin,
            * Usually activated by      SleepingEnd,

            * Usually activated by     EVENT_GenComp.TransmittingBegin,      // Begin transmitting the result (externally); unattended
            * Usually activated by     EVENT_GenComp.TransmittingEnd,        // Feedback from transmission unit; unattended
            PotentialRestored,      //
            // HW-related
            Awaken,                  // Signals that is ready to use
            Wakeup;                  // The HW is needed again, awake it
    };

  */


    /**
     * @brief Synchronize the PU to some external condition
     *
     * Usually activated by     EVENT_GenComp.Synchronize,             // External synhronize signal

     * - In biological computing, it results in issuing 'End Computing' (immediate spiking; passing to 'Delivering')
     * - In technical computing, it results in issuing 'Begin Computing' (starting computing; passing to 'Processing')
     */
    virtual void Synchronize_method();


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
    void scLocalTime_Set(sc_core::sc_time T){    mLocalTimeBase = T;}
    sc_core::sc_time scTimeBase_Get(void){return mLocalTimeBase;}
    size_t NoOfInputsReceived_Get(){ return Inputs.size();}
     /*!
     * \brief Set an observing bit for this scGenComp_PU_Abstract
     * \param B is the bit to set  in mObservedBit
     * \param V is the requested value of the bit
     */
    void ObservingBit_Set(GenCompPUObservingBits_t B, bool V)
     {
        assert(B < gcob_Max);  // Check if wrong bit
        mObservedBits[B] = V;
    }

     /*!
     * \brief Set an observing bit for this scGenComp_PU_Abstract
     * \param B is the bit to get in mObservedBits
     * \return is the requested value of the bit
     */
    bool ObservingBit_Get(GenCompPUObservingBits_t B)
    {
         assert(B < gcob_Max);  // Check if wrong bit
         return mObservedBits[B];
    }

    /**
     * @brief Heartbeat_Get
     * @return The actual heartbeat time
     */
    sc_core::sc_time Heartbeat_Get(){return mHeartbeat;}
    /**
     * @brief Heartbeat_Set
     * @param T the requested heartbeat time
     */
    void Heartbeat_Set(sc_core::sc_time T){mHeartbeat = T;}
    /**
     * Save the address of the observer simulator in the PU
     */
    void RegisterSimulator(scGenComp_Simulator*);
    /**
    * @brief ObserverNotify
    *
    * Notify the observer about some important change
    * @param[in] ObservedBit The event that occured
    */
    void ObserverNotify(GenCompPUObservingBits_t ObservedBit);

   protected:
    /**
     * @brief Handle Heartbeat in 'Ready' mode
     *
     * Called by Heartbeat_method();
     *
     * They should be overloaded (by default make nothing)
     */
    virtual void Heartbeat_Ready_Do(){};
    /**
     * @brief Handle Heartbeat in 'Processing' mode
     *
     * Called by Heartbeat_method();
     *
     * They should be overloaded (by default make nothing)
     */
    virtual void Heartbeat_Processing_Do(){};
    /**
     * @brief Handle Heartbeat in 'Delivering' mode
     *
     * Called by Heartbeat_method();
     *
     * They should be overloaded (by default make nothing)
     */
    virtual void Heartbeat_Delivering_Do(){};
    /**
     * @brief Handle Heartbeat in 'Relaxing' mode
     *
     * Called by Heartbeat_method();
     *
     * They should be overloaded (by default make nothing)
     */
    virtual void Heartbeat_Relaxing_Do(){};

    GenCompStates_Abstract* MachineState;     ///< Points to the service object of the state machine
    GenCompStateMachineType_t mStateFlag;    ///< preserves last state

    int32_t mNoOfInputsNeeded;          ///< Remember how many inputs needed
    vector<int32_t> Inputs; // Stores reference to input source
    /**
     * @brief mLocalTimeBase: the PUs have a local time: the time spent since starting processing
     */
    sc_core::sc_time mLocalTimeBase;    ///< The beginning of the local computing
    sc_core::sc_time mHeartbeat;        ///< The heartbeat length of the unit
    sc_core::sc_time mLastProcessingTime; ///< Remember last processing time duration  (the result)
    sc_core::sc_time mLastIdleTime;     ///< Time duration from Last mLastRelaxingEndTime to ProcessingBegin
    sc_core::sc_time mLastRelaxingEndTime;   ///< Remember the beginning of the 'Idle' period
    sc_core::sc_time mLastComputingTime; ///< Remember last computing time duration  (the result)
private:
    /**
     * @brief mObservedBits: Store here which events the unit wants to be observed
     *
     * @see GenCompPUObservingBits_t
     */
    bitset<gcob_Max> mObservedBits;   ///< The bits of the GenComp_PU state
    scGenComp_Simulator* MySimulator;     ///< The simulator that observes us
};// of class scGenComp_PU_Abstract

/** @}*/

#endif // SCABSTRACTGENCOMP_H

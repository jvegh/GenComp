/** @file scGenComp_PU_Abstract.h
 *  @ingroup GENCOMP_MODULE_PROCESS
 *
 *  @brief Function prototypes for the computing module.
 *  Just event handling, real activity
 *  @todo Implement Fail, Synchronize
 *  @todo Implement Sleep, Awake
 *  @todo Implement PotentialRestored
 *  @todo Implement TransmittingEnd
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

#include <systemc>
#include <bitset>
#include "../../3rdParty/Minuit/include/GaussRandomGen.h"
//#include "GaussRandomGen.h"
using namespace sc_core; using namespace sc_dt;
using namespace std;
class scGenComp_Simulator;

#define HEARTBEAT_TIME_DEFAULT sc_core::sc_time(1,SC_NS)


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
 *
  *  @var EVENT_GenComp_type::Synchronize
 *  External event forces PU to synchronize
 *  @var EVENT_GenComp_type::Failed
 *  Computing failed, start over
 *  @var EVENT_GenComp_type::Initialize
 *  Put the unit to its ground state
 *  @var EVENT_GenComp_type::InputReceived
 *  New input received
 *
 *  Actually, the above main duties govern processing of the abstract computing units.
 * The operating principle of event processing:
 *
 * "XXX_thread" methods are "sensitive" to specific events. After receiving an event,
 * the corresponding virtual routine of XXX_Do method is called.
 * If the old and the new states are not compatible, that routine asserts. (wrong event)
 * Following a normal return, the HW specific operation of the \gls{PU} is executed.
 * The next state is invoked by issuing a EVENT_GenComp.XXX.notify() notification.
 * The length of operations is simulated by issuing a wait() call to SystemC kernel.
 * (technically, 'next_trigger()' is used, to mitigate system's non-payload activity.)
 *
 * There are continuos actions; they begin their activity in a XXXBegin_method
 * and terminate it in a XXXEnd_method. The event XXXBegin is issued by either another statio's XXXEnd_method
 * or by the simulator scGenComp_Simulator. The XXXEnd events are generated during processing of the statio.
 * The corresponding events are issued in form of a EVENT_GenComp.Event.notify(SC_ZERO_TIME).
 * (the argument SC_ZERO_TIME means: immediately after activities at this time terminated).
 * The non-virtual methods XXXBegin_method and  XXXEnd_method should not be overridden
 * (because of the event handling method of SystemC); they use the virtual methods XXXBegin_Do and XXXEnd_Do.
 * Those latter methods can call the corresponding method of their superclass or replace them.
 *
 * The description is valid for state-implementing methods
 * - Deliver
 * - Process
 * - Relax
 * - Transmit
 *
 * There are also single action similar methods where there are no XXXBegin_method
 * and XXXEnd_method; instead they have non-virtual XXX_method and similarly they call
 * the virtual XXX_Do() method.
 *
 * The description is valid for single-shot action-implementing methods
 * - Synchronize
 *
*/
/* Implement later
 *  @var EVENT_GenComp_type::TransmittingBegin
        *  Begin transmitting the result (externally); unattended
    *  @var EVENT_GenComp_type::TransmittingEnd
        *  Feedback from transmission unit; unattended
 *  @var EVENT_GenComp_type::SleepingBegin
 *  Begin of HW sleepig
 *  Only in 'Ready' state, the PU can spare power by going to 'Sleeping' state
 *  @var EVENT_GenComp_type::SleepingEnd,
 *  End of HW spleeping
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
        Ready,                  // The unit is ready again
        RelaxingBegin,          // Begin restoring the 'Ready' state
        RelaxingEnd,            // End restoring the 'Ready' state
        Synchronize,             // External synhronize signal

//        TransmittingBegin,      // Begin transmitting the result (externally); unattended
        // It is the same as 'RelaxingBegin'
//        TransmittingEnd,        // Feedback from transmission unit; unattended
        // Do we need it?
        //PotentialRestored,      //
        // Do we need it
        // HW-related
        //Awaken,                  // Signals that is ready to use
        // Implement later
//        SleepingBegin,          // Begin
//        SleepingEnd,
        // Sleep,
        // Implement later
        Wakeup;                  // The HW is needed again, awake it
};

#ifndef SCBIOGENCOMP_H
#ifndef SCTECHGENCOMP_H // Just to exclude for Doxygen, to not repat it


/*!
 * \class scGenComp_PU_Abstract
 * \brief  A simple abstract class to deal  with operations of a general computing unit.
 * The unit implements a single-shot elementary computation. The statios  of computing
 * are marked by event pairs
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
 * To initiate a single-shot operation while in 'Ready' statio, different methods are used by the derived classes.
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

/*! \var typedef  GenCompStateMachineType_t
 * The operation of an elementary computing unit of general computing is modelled as a multiple-state machine,
 * with internal state variables.
 * The states are used in scGenComp_PU_Abstract.
 @verbatim
  Sleeping   - waiting for activation;
               Goes to Ready
  Ready      - everything is prepared, variables available, activated from Seeping state;
               Normally goes to Processing; After some inactivity goes to Sleeping;
               The abstract units are generated to this state.
  Processing - receives inputs (computes), runs timed functions;
               Normally goes to Delivering its result; it may also fail and go to Relaxing
  Delivering - produces output for some time;
               Normally goes to Relaxing.  An extranal Synchronizing may force immediate Delivering
  Relaxing   - is on vacation, no input received; everything reset
               Goes to Readyafter that goes to Working

  The General Computing Unit's abstract state machine

                   Sleeping<-------> Ready   <---Initially
                                   ^  ^    \   ^
                                  /  HBeat  \    <-InputReceived
                                 /         v v v
                         Relaxing <-Failed- Processing
                                ^           /
                                 \         /
                                  \       v
                                  Delivering<-----Synchronizing
 @endverbatim
 *
 * The states of the general computing unit's state machine
 * - Sleeping: just a technical state, unused units can be switched off
 *   - Can be activated explicitely (after 3 processor clock signals delay)
 *   - Can be activaed implicitely ((after 3 processor clock signals delay)
 * - Ready: The unit is ready, waiting for 'Begin Computing';
 *   - after some inactivity, can pass to Sleeping
 *   - after receiving 'Begin Computing' passes to Processing
 * - InputReceived: the unit received new input (a momentary state)
 *   - in biological mode, starts to add new synaptic input. If first,  issues 'Begin Computing';
 *   - in technical mode, account arrival of an argument; If all,  issues 'Begin Computing';
 * - Processing: The unit is computing (takes time \[T_P\])
 *   - in synchronized technical mode: received synchron signal
 *   - in asynchron mode: received all needed arguments
 *   - in biological mode: received its first argument
 *   Passes to Delivering (after issuing 'End Computing') if successful;
 *   Passes to Relaxing if Failed
 * - Relaxing: Resets state and passes to 'Computing'
 * --These states below are momantary states: need action and passes to one of the above states
 * - Delivering: The unit is delivering its result to its output section
 *   - After some time, it Sends 'Begin Transmitting' @see scGenComp_PU_Abstract
 *     (Activates transmission unit to send computed result to its chained unit(s),
 *      then goes to Relaxing
 * - Failing :
 * - Synchronizing: deliver result, anyhow ;  (a momentary state)
 *   - in biologycal mode, deliver immediate spike
 *   - in technical mode, deliver immediate result
 *   Passes to Relaxing (after issuing 'End Computing)
 */
#endif //SCTECHGENCOMP_H
#endif //SCBIOGENCOMP_H


typedef enum {gcsm_Sleeping, gcsm_Ready, gcsm_Processing, gcsm_Delivering, gcsm_Relaxing}
GenCompStateMachineType_t;


extern string GenCompStatesString[];
/*! \var typedef GenCompPUObservingBits_t
 * \brief the names of the bits in the bitset describing
 * which signals in scGenComp_PU_Abstract are monitored by the scGenComp_Simulator
 */
typedef enum
{
    gcob_ObserveGroup,              ///< The module's group is observed (by the simulator)
    gcob_ObserveModule,             ///< The module is observed (by the simulator)
    gcob_ObserveDeliveringBegin,    ///< Observe 'Begin Delivering'
    gcob_ObserveDeliveringEnd,      ///< Observe 'End Delivering'
    gcob_ObserveHeartbeat,          ///< Observe 'Heartbeat's of the PU
    gcob_ObserveInput,              ///< Observe 'Input's of the PU
    gcob_ObserveInitialize,         ///< Observe 'Initialize' of the PU
    gcob_ObserveProcessingBegin,    ///< Observe 'Begin Computing'
    gcob_ObserveProcessingEnd,      ///< Observe 'End Computing'
    gcob_ObserveReady,              ///< Observe 'Ready' of the PU
    gcob_ObserveRelaxingBegin,      ///< Observe 'Begin Relaxing'
    gcob_ObserveRelaxingEnd,        ///< Observe 'End Relaxing'
    gcob_ValueChanged,              ///< Observe 'Value changed'
    gcob_Max                    // just maintains the number of bits used
} GenCompPUObservingBits_t;
extern string GenCompObserveStrings[];


class scGenComp_PU_Abstract: public sc_core::sc_module
{
   public:
    /*!
     * \brief Create an abstract processing unit for the general computing paradigm
     * @param[in] nm the SystemC name of the module
     * @param[in] HeartBeat the state refreshing time
     */
    scGenComp_PU_Abstract(sc_core::sc_module_name nm
                             , sc_core::sc_time HeartBeat = HEARTBEAT_TIME_DEFAULT);

    virtual ~scGenComp_PU_Abstract(void); // Must be overridden

    /**
     * @brief CancelEvents
     *
     * When 'Reset' requested, cancels all pending events
     */
    void CancelEvents(void);

    /**
     * @brief Internal result delivering has started.
     *
     * Usually activated by         EVENT_GenComp.DeliveringBegin
     */
    void DeliveringBegin_method();
    virtual void DeliveringBegin_Do();

    /**
     * @brief Internal result delivering has finished.
     *
     * Usually activated by         EVENT_GenComp.DeliveringEnd
     */
    void DeliveringEnd_method();
    virtual void DeliveringEnd_Do();

    /**
     * @brief Module failed.
     *
     * Usually activated by         EVENT_GenComp.Failed
     * - In biological computing, it means a failed charging-up; passes to 'Ready state'
     * - In technical computing, it results in a failed operation
     */
    void Failed_method();
    virtual void Failed_Do();

    /**
     * @brief A periodic signal as a timebase to refresh state; to avoid using clock signals.
     *
     * Usually activated by EVENT_GenComp.Heartbeat. A heartbeat can be useful in states
     * - Ready : the potential is leaking
     * - Processing: the potencial is leaking and charging up
     * - Delivering: the membrane voltage builds up and leaking
     * - Relaxing: the membrane potential decays
     *
     * Branches according to PU's state and calls (see the corresponding state machine diagram)
     *    -  virtual void Heartbeat_Ready_Do();
     *    -  virtual void Heartbeat_Processing_Do();
     *    -  virtual void Heartbeat_Delivering_Do();
     *    -  virtual void Heartbeat_Relaxing_Do();
     *
     * Usage
     *   - For technical computing, it is used to display time delays
     *   - For biological computing, it is used to solve PDEs
     */
    void Heartbeat_method();

    /**
     * @brief HeartbeatTime_Get
     * @return The actual heartbeat time
     */
    inline sc_core::sc_time HeartbeatTime_Get(){return mHeartbeat;}

    /**
     * @brief HeartbeatTime_Set
     * @param T the requested heartbeat time
     */
    inline void HeartbeatTime_Set(sc_core::sc_time T){mHeartbeat = T;}

    /**
     * @brief The heartbeat time is divided into mHeartbeatDivisions intervalls for calculation
     * @return the number of heartbeat divisions
     */
    inline int32_t HeartbeatDivisions_Get(void){return mHeartbeatDivisions;}
    inline void HeartbeatDivisions_Set(int32_t N){mHeartbeatDivisions = N;}

    /**
     * @brief Initialize_method
     *
     * Usually activated by      EVENT_GenComp.Initialize
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
     * Usually activated by      EVENT_GenComp.InputReceived
     */
    void InputReceived_method();

    /**
     * @brief Prepare external input for processing
     * Usually activated by      EVENT_GenComp.InputReceived
     */
    virtual void InputReceived_Do(void);

    /**
     * @brief Processing has started
     *
     * Usually activated by      EVENT_GenComp.ProcessingBegin
     */
    void ProcessingBegin_method();
    virtual void ProcessingBegin_Do();

    /**
     * @brief Processing has finished
     * Usually activated by      EVENT_GenComp.ProcessingEnd
     *
     */
    void ProcessingEnd_method();
    virtual void ProcessingEnd_Do(){};

    /**
     * @brief the PU is ready again
     *
     * Usually activated by         EVENT_GenComp.Ready
     */
    void  Ready_method();
    virtual void Ready_Do(){};

    /* *
     * @brief Resetting the unit to its operating state
     *
     * The routine is actived by EVENT_GenComp.End_Delivering
     */
 //   void Reset(void);

     /**
     * @brief Relaxing has started
     *
     * Usually activated by RelaxingBegin
     * After delivered the result internally to the 'output section', resetting begins
     */
    void RelaxingBegin_method();
    virtual void RelaxingBegin_Do(){};

    /**
     * @brief Relaxing has finished
     *
     * Usually activated by    EVENT_GenComp.RelaxingEnd
     */
    void RelaxingEnd_method();
    virtual void RelaxingEnd_Do(){};

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
     * Usually activated by     EVENT_GenComp.Synchronize

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
    size_t NoOfInputsReceived_Get(){ return Inputs.size();}
    sc_core::sc_time scTimeTransmission_Get(){ return mLastTransmissionTime;}

    /**
    * @brief ObserverNotify
    *
    * Notify the observer about some important change
    * @param[in] ObservedBit The event that occured
    */
    void ObserverNotify(GenCompPUObservingBits_t ObservedBit);

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

    /**
     * Save the address of the observer simulator in the PU
     */
    void RegisterSimulator(scGenComp_Simulator*);

    /**
     * @brief scLocalTime_Get
     * @return The simulated time since the beginning of the recent operation
     */
    sc_core::sc_time scLocalTime_Get() const {return sc_core::sc_time_stamp()-mLocalTimeBase;}
    /**
     * @brief scLocalTimeBase_Set
     * @param T The beginning of the simulated time of the recent operation
     */
    void scLocalTimeBase_Set(sc_core::sc_time T) { mLocalTimeBase = T;}
    /**
     * @brief scLocalTimeBase_Get
     * @return The beginning of the simulated time of the recent operation
     */
    sc_core::sc_time scTimeBase_Get(void)const {return mLocalTimeBase;}

   protected:
    /**
     * @brief Handle Heartbeat in 'Ready' mode
     *
     * It should be overloaded (by default makes nothing).
     * Called by Heartbeat_method();
      */
    virtual void Heartbeat_Ready_Do(){};
    /**
     * @brief Handle Heartbeat in 'Processing' mode
     *
     * It should be overloaded (by default makes nothing).
     * Called by Heartbeat_method();
     */
    virtual void Heartbeat_Processing_Do(){};
    /**
     * @brief Handle Heartbeat in 'Delivering' mode
     *
     * It should be overloaded (by default makes nothing).
     * Called by Heartbeat_method();
     */
    virtual void Heartbeat_Delivering_Do(){};
    /**
     * @brief Handle Heartbeat in 'Relaxing' mode
     *
     * It should be overloaded (by default makes nothing)
     * Called by Heartbeat_method();
     */
    virtual void Heartbeat_Relaxing_Do(){};

    inline sc_core::sc_time
    LastOperatingTime_Get() const {return mLastOperatingTime;}
    inline sc_core::sc_time
    LastResultTime_Get() const {return mLastResultTime;}
    inline sc_core::sc_time
    LastIdleTime_Get() const{return mLastIdleTime;}
    inline int32_t
    OperationCounter_Get() const {return mOperationCounter;}

    GenCompStateMachineType_t mStateFlag;    ///< Preserves last state

    int32_t mNoOfInputsNeeded;          ///< Remember how many inputs needed
    int32_t mHeartbeatDivisions;           ///< How many subdivisions the 'heartbeat' time divides to
    vector<int32_t> Inputs; // Stores reference to input source
    /**
     * @brief mLocalTimeBase: the PUs have a local time: the time spent since starting this processing
     */
    sc_core::sc_time
         mLocalTimeBase        /// The beginning of the local computing
        ,mHeartbeat            /// The heartbeat length of the unit
        ,mLastOperatingTime    /// Remember last total operating time duration
        ,mLastResultTime       /// Remember last processing time duration  (the result)
        ,mLastRelaxingEndTime  /// Remember the beginning of the 'Idle' period
        ,mLastIdleTime         /// Time duration from Last mLastRelaxingEndTime to ProcessingBegin
        ,mLastTransmissionTime /// The time of last transmission began (aka spiking)
        ;
private:
    /**
     * @brief mObservedBits: Store here which events the unit wants to be observed
     *
     * @see GenCompPUObservingBits_t
     */
    bitset<gcob_Max> mObservedBits;   ///< The bits of the GenComp_PU state
    scGenComp_Simulator* MySimulator;     ///< The simulator that observes us
    int32_t mOperationCounter;            ///< Count the operations
};// of class scGenComp_PU_Abstract

/** @}*/

#endif // SCABSTRACTGENCOMP_H

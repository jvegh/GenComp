/** @file GenCompStates.h
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief Working states of the archetypes of processing units
 */
 /*  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef GenCompStates_h
#define GenCompStates_h

// Idea from https://stackoverflow.com/questions/14676709/c-code-for-state-machine/19896947
// must not be taken as in SystemC no new electronic module can be created.
// So, AbstractGenComp_PU handles the events and calls the corresponding
//using namespace sc_core; using namespace std;

class scAbstractGenComp_PU;

// This define in only temporarily here,  should go to HWConfig.h
#define USE_PU_HWSLEEPING


/*! \var typedef  GenCompStateMachineType_t
 * The operation of an elementary computing unit of general computing is modelled as a multiple-state machine,
 * with internal state variables.
 * The states are used in AbstractGenComp_PU.
 @verbatim
  Sleeping   - waiting for activation;
               Goes to Ready
  Ready      - everything is prepared, variables available, activated from Seeping state;
               Normally goes to Processing; After some inactivity goes to Sleeping;
               The abstract units are generated to this state.
  Processing - receives inputs (computes), runs timed functions;
               Normallygoes to Delivering its result; it may also fail and go to Relaxing
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
 * - Processing: The unit is computing (takes time $T_P$)
 *   - in synchronized technical mode: received synchron signal
 *   - in asynchron mode: received all needed arguments
 *   - in biological mode: received its first argument
 *   Passes to Delivering (after issuing 'End Computing') if successful;
 *   Passes to Relaxing if Failed
 * - Delivering : The unit is delivering its result to its output section
 *   - After some time, it Sends 'Begin Transmitting' @see AbstractGenComp_TU
 *     (Activates transmission unit to send computed result to its chained unit(s),
 *      then goes to Relaxing
 * - Relaxing: Resets state and passes to 'Computing'
 * - Synchronizing: deliver result, anyhow ;  (a momentary state)
 *   - in biologycal mode, deliver immediate spike
 *   - in technical mode, deliver immediate result
 *   Passes to Relaxing (after issuing 'End Computing)
 */
typedef enum {gcsm_Sleeping, gcsm_Ready, gcsm_Processing, gcsm_Delivering, gcsm_Relaxing, gcsm_Syncronizing, gcsm_Failed}
              GenCompStateMachineType_t;


/*!
 * \brief
 * This class implements a state machine for abstract computing unit scAbstractGenComp_PU
 *
 * The general computing is event-driven, i.e. events are received by the abstract general processing unit
 * and are processed by a scTechGenComp_PU or scBioGenComp_PU general processing unit.
 * This state machine controls transitions between states. When a processing unit receives a state-related
 * event, it calls the corresponding method of this class.
 * The PU is generated in Ready (ready to process, gcsm_Ready) state.
 *
 * @see scAbstractGenComp_PU
 * @see scAbstractGenComp_PU#EVENT_GenComp
 * @see GenCompStateMachineType_t
 * @see scAbstractGenComp_PU#mStateFlag
 */

class AbstractGenCompState
{
    public:
        /**
         * @brief Puts the PU state to 'Ready' (called by the scAbstractGenComp_PU's constructor)
         * and sets up its event handling
         */
        AbstractGenCompState(void);
        virtual ~AbstractGenCompState(void);
        /**
         * @brief Deliver: Signal 'End computing'; result to the 'output section'
         *
         * @param PU The HW to set
         *
         * Called twice:
         * - 1st time, the system must be in state gcsm_Processing, passes to gcsm_Delivering
         * - 2nd time in state gcsm_Delivering; passes to gcsm_Relaxing
         */
        virtual void Deliver(scAbstractGenComp_PU* PU);

        /**
         * @brief Process: Signal 'begin computing" received; arguments in the 'input section'; start computing
         *
         * @param PU The HW to set
         */
        virtual void Process(scAbstractGenComp_PU* PU);

        /**
         * @brief Relax: After finishing processing, resets the HW. Uses @see Reinitialize
         *
         * @param PU The HW to set
         */
        virtual void Relax(scAbstractGenComp_PU* PU);

        /**
         * @brief Initialize: Sets the state machineto its well-defined initial state
         *
         * @param PU The HW to set
         * A simple subroutine, sets state to gcsm_Ready, trigger to
         */
        virtual void Initialize(scAbstractGenComp_PU* PU);

        /**
         * @brief InputReceived: The machine received new input, administer it
         *
         * @param PU The HW to set
         * A simple subroutine, sets state to 'ready', trigger to
         */
        virtual void InputReceived(scAbstractGenComp_PU* PU);

        /**
         * @brief Synchronize: Independently from its actual state, forces the HW to @see Deliver
         *
         * @param PU The HW to set
         */
        virtual void Synchronize(scAbstractGenComp_PU* PU);

        /**
         * @brief Fail: Can happen only in Processing state; passes to Relaxing state
         *
         * @param PU The HW that failed
         */
        virtual void Fail(scAbstractGenComp_PU* PU);

        /**
         * @brief State_Set
         *
         * @param PU The HW to set
         * @param State The selected state type flag
         */
        void State_Set(scAbstractGenComp_PU* PU, GenCompStateMachineType_t& State);

#ifdef USE_PU_HWSLEEPING
         /**
         * @brief Sleep: Send the HW to sleep if idle for a longer time;  economize power
         *
         * @param PU The HW to set
         */
        virtual void Sleep(scAbstractGenComp_PU *PU);

        /**
         * @brief WakeUp: Wake up machine if was sent to sleep;  economize power
         *
         * @param PU The HW to set
         */
        virtual void Wakeup(scAbstractGenComp_PU *PU);
 #endif // USE_PU_HWSLEEPING
    protected:
        void UpdatePU(scAbstractGenComp_PU& PU);
 };
;

#endif //GenCompStates_h

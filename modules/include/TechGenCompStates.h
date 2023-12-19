/** @file TechGenCompStates.h
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief Working states of the archetypes of technical processing units
 */
 /*  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef TechGenCompStates_h
#define TechGenCompStates_h

// Idea from https://stackoverflow.com/questions/14676709/c-code-for-state-machine/19896947
// must not be taken as in SystemC no new electronic module can be created.
// So, scTechGenComp_PU handles the events and calls the corresponding
//using namespace sc_core; using namespace std;

class scTechGenComp_PU;

// This define in only temporarily here,  should go to HWConfig.h
#define USE_PU_HWSLEEPING


/* ! \var typedef  GenCompStateMachineType_t
 * The operation of an elementary computing unit of general computing is modelled as a multiple-state machine,
 * with internal state variables.
 * The states are used in scTechGenComp_PU.
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
 *   - in biological mode, deliver immediate spike
 *   - in technical mode, deliver immediate result
 *   Passes to Relaxing (after issuing 'End Computing)
 */
//typedef enum {gcsm_Sleeping, gcsm_Ready, gcsm_Processing, gcsm_Delivering, gcsm_Relaxing, gcsm_Syncronizing, gcsm_Failed}
  //            GenCompStateMachineType_t;


/*!
 * \brief
 * This class implements states for an technical computing unit; base for techical computing
 *
 * The general computing is event-driven, i.e. events are received by the technical state machine
 * and are processed by a technical or biological biological processing unit.
 * The PU is generated in Ready (ready to process) state.
 *
 * @see TechGenCompState#EVENT_GenComp
 * @see TechGenCompStateMachineType_t
 */

class TechGenCompState
{
    public:
        /**
         * @brief Puts the PU state to 'Ready' (called by the BioGenComp_PU's constructor)
         * and sets up its event handling
         */
        TechGenCompState(void);
        virtual ~TechGenCompState(void);
        /**
         * @brief Deliver: Signal 'End computing'; result to the 'output section'
         */
        virtual void Deliver(scTechGenComp_PU *PU);

        /**
         * @brief Process: Signal 'begin computing" received; arguments in the 'input section'; start computing
         */
        virtual void Process(scTechGenComp_PU *PU);

        /**
         * @brief Relax: After finishing processing, resets the HW. Uses @see Reinitialize
         */
        virtual void Relax(scTechGenComp_PU *PU);

        /**
         * @brief Initialize: Sets the state machineto its well-defined initial state
         *
         * @param PU The HW to set
         * A simple subroutine, sets state to 'ready', trigger to
         */
        virtual void Initialize(scTechGenComp_PU* PU);

        /**
         * @brief InputReceived: The machine received new input, administer it
         *
         * @param PU The HW to set
         * A simple subroutine, sets state to 'ready', trigger to
         */
        virtual void InputReceived(scTechGenComp_PU* PU);

        /**
         * @brief Synchronize: Independently from its actual state, forces the HW to @see Deliver
         */
        virtual void Synchronize(scTechGenComp_PU *PU);

        /**
         * @brief Fail: Can happen only in Processing state; passes to Relaxing state
         * @param PU The HW that failed
         */
        virtual void Fail(scTechGenComp_PU* PU);
        void State_Set(scTechGenComp_PU* PU, GenCompStateMachineType_t& State);

#ifdef USE_PU_HWSLEEPING
         /**
         * @brief Sleep: Send the HW to sleep if idle for a longer time;  economize power
         * @param PU The HW to set
         */
        virtual void Sleep(scTechGenComp_PU *PU);

        /**
         * @brief WakeUp: Wake up machine if was sent to sleep;  economize power
         */
        virtual void Wakeup(scTechGenComp_PU *PU);
        /**
         * @brief State_Set Set the state of PU to st
         * @param PU The HW to set
         */
#endif // USE_PU_HWSLEEPING
    protected:
        void UpdatePU(scTechGenComp_PU& PU);
    private:
 };

/*
 * The subclasses define the behavior *in the actual state*
 *
 */

 /**
 * @brief The ReadyTechGenCompState class
 *
 * The PU is ready to operate, is waiting for a 'Begin computing' signal
 */
class ReadyTechGenCompState : public TechGenCompState {
    public:
        ReadyTechGenCompState(void);
        virtual ~ReadyTechGenCompState();
        void Deliver();
        void Process();
        void Relax();
        void Reinitialize();
        void Synchronize();
};

/**
 * @brief The SleepingTechGenCompState class
 *
 * The PU is going to economize power
 *
 */

class SleepingTechGenCompState : public TechGenCompState {
    public:
        SleepingTechGenCompState(void);
        virtual ~SleepingTechGenCompState();
        void Process();
};

/**
 * @brief The ProcessingTechGenCompState class
 *
 * The PU starts to process the data
 */

class ProcessingTechGenCompState : public TechGenCompState
{
    public:
        ProcessingTechGenCompState(void);
        virtual ~ProcessingTechGenCompState();
        void Process();
};

/**
 * @brief The DeliveringTechGenCompState class
 *
 * The PU deliver its result to the 'output section'
 */
class DeliveringTechGenCompState : public TechGenCompState
{
    public:
        DeliveringTechGenCompState(void);
        virtual ~DeliveringTechGenCompState();
};

/**
 * @brief The RelaxingTechGenCompState class
 *
 * The PU recovers its operating state
 */
class RelaxingTechGenCompState : public TechGenCompState
{
    public:
        RelaxingTechGenCompState(void);
        virtual ~RelaxingTechGenCompState();
};

/**
 * @brief The AwakingTechGenCompState class
 *
 * The sleeping (for economizing power) unit is activated
 */
class AwakingTechGenCompState : public TechGenCompState
{
    public:
        AwakingTechGenCompState(void);
        virtual ~AwakingTechGenCompState();
};

/**
 * @brief The ResettingTechGenCompState class
 *
 * The PU is set to its default state
 */
class ResettingTechGenCompState : public TechGenCompState
{
    public:
        ResettingTechGenCompState(void);
        virtual ~ResettingTechGenCompState();
};

/**
 * @brief The FailedTechGenCompState class
 *
 * Some error happened or synchronization received
 */

class FailedTechGenCompState : public TechGenCompState
{
    public:
        FailedTechGenCompState(void);
        virtual ~FailedTechGenCompState();
        void Process();
};

#endif //TechGenCompStates_h

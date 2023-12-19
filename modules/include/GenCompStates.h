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

                   Slepping<-------> Ready   <---Initially
                                   ^      \
                                  /        \
                                 /          v
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
 * - Processing: The unit is computing (takes time $T_P$)
 *   - in synchronized technical mode: received synchron signal
 *   - in asynchron mode: received all needed arguments
 *   - in biological mode: received its first argument
 *   Passes to Delivering (after issueing 'End Computing') if successful;
 *   Passes to Relaxing if Failed
 * - Delivering : The unit is delivering its result to its output section
 *   - After some time, it Sends 'Begin Transmitting' @see AbstractGenComp_TU
 *     (Activates transmission unit to send computed result to its chained unit(s),
 *      then goes to Relaxing
 * - Relaxing: Resets state and passes to 'Computing'
 */
typedef enum {gcsm_Sleeping, gcsm_Ready, gcsm_Processing, gcsm_Delivering, gcsm_Relaxing, gcsm_Syncronizing, gcsm_Failed}
              GenCompStateMachineType_t;


/*!
 * \brief
 * This class implements an abstract computing unit; base for technical and biological computing
 *
 * The general computing is event-driven, i.e. events are received by the abstract state machine
 * and are processed by a technical or biological abstract processing unit.
 * The PU is generated in Ready (ready to process) state.
 *
 * @see AbstractGenCompState#EVENT_GenComp
 * @see GenCompStateMachineType_t
 */

class AbstractGenCompState
{
    public:
        /**
         * @brief Puts the PU state to 'Ready' (called by the AbstractGenComp_PU's constructor)
         * and sets up its event handling
         */
        AbstractGenCompState(void);
        virtual ~AbstractGenCompState(void);
        /**
         * @brief Deliver: Signal 'End computing'; result to the 'output section'
         */
        virtual void Deliver();

        /**
         * @brief Process: Signal 'begin computing" received; arguments in the 'input section'; start computing
         */
        virtual void Process(void);

        /**
         * @brief Relax: After finishing processing, resets the HW. Uses @see Reinitialize
         */
        virtual void Relax();

        /**
         * @brief Initialize: Sets the state machineto its well-defined initial state
         *
         * @param PU The HW to set
         * A simple subroutine, sets state to 'ready', trigger to
         */
        virtual void Initialize(scAbstractGenComp_PU* PU);

        /**
         * @brief Synchronize: Independently from its actual state, forces the HW to @see Deliver
         */
        virtual void Synchronize();

        /**
         * @brief Fail: Independently from its actual state, forces the HW to @see Deliver
         */
        virtual void Fail();

         /**
         * @brief Sleep: Send the HW to sleep if idle for a longer time;  economize power
         * @param PU The HW to set
         */
        virtual void Sleep(scAbstractGenComp_PU *PU);

        /**
         * @brief WakeUp: Wake up machine if was sent to sleep;  economize power
         */
        virtual void Wakeup(scAbstractGenComp_PU *PU);
        /**
         * @brief State_Set Set the state of PU to st
         * @param PU The HW to set
         * @param[inout] State The state to set to
         */
        void State_Set(scAbstractGenComp_PU* PU, GenCompStateMachineType_t& State);

          protected:
        void UpdatePU(scAbstractGenComp_PU& PU);
    private:
 };

/*
 * The subclasses define the behavior *in the actual state*
 *
 */

 /**
 * @brief The ReadyGenCompState class
 *
 * The PU is ready to operate, is waiting for a 'Begin computing' signal
 */
class ReadyGenCompState : public AbstractGenCompState {
    public:
        ReadyGenCompState(void);
        virtual ~ReadyGenCompState();
        void Deliver();
        void Process();
        void Relax();
        void Reinitialize();
        void Synchronize();
};

/**
 * @brief The SleepingGenCompState class
 *
 * The PU is going to economize power
 *
 */

class SleepingGenCompState : public AbstractGenCompState {
    public:
        SleepingGenCompState(void);
        virtual ~SleepingGenCompState();
        void Process();
};

/**
 * @brief The ProcessingGenCompState class
 *
 * The PU starts to process the data
 */

class ProcessingGenCompState : public AbstractGenCompState
{
    public:
        ProcessingGenCompState(void);
        virtual ~ProcessingGenCompState();
        void Process();
};

/**
 * @brief The DeliveringGenCompState class
 *
 * The PU deliver its result to the 'output section'
 */
class DeliveringGenCompState : public AbstractGenCompState
{
    public:
        DeliveringGenCompState(void);
        virtual ~DeliveringGenCompState();
};

/**
 * @brief The RelaxingGenCompState class
 *
 * The PU recovers its operating state
 */
class RelaxingGenCompState : public AbstractGenCompState
{
    public:
        RelaxingGenCompState(void);
        virtual ~RelaxingGenCompState();
};

/**
 * @brief The AwakingGenCompState class
 *
 * The sleeping (for economizing power) unit is activated
 */
class AwakingGenCompState : public AbstractGenCompState
{
    public:
        AwakingGenCompState(void);
        virtual ~AwakingGenCompState();
};

/**
 * @brief The ResettingGenCompState class
 *
 * The PU is set to its default state
 */
class ResettingGenCompState : public AbstractGenCompState
{
    public:
        ResettingGenCompState(void);
        virtual ~ResettingGenCompState();
};

/**
 * @brief The FailedGenCompState class
 *
 * Some error happened or synchronization received
 */

class FailedGenCompState : public AbstractGenCompState
{
    public:
        FailedGenCompState(void);
        virtual ~FailedGenCompState();
        void Process();
};

#endif //GenCompStates_h

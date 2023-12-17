/** @file scGenCompStates.h
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief Working states of a neurer.
 */
 /*  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef GenCompStates_h
#define GenCompStates_h

#include <systemc>
// Idea from https://stackoverflow.com/questions/14676709/c-code-for-state-machine/19896947
//using namespace sc_core; using namespace std;

/*!
 * \brief GenCompStates class
 * The operation of an elementary computing unit of general computing is modelled as a multiple-state machine,
 * with internal state variables.
 * The states are used in AbstractGenComp_PU;
 *
 * The states are
 * @verbatim
 * Dormant    - waiting for activation
 * Ready      - everything is prepared, variables available, activated from dormant state; After some inactivity goes Dormant
 * Processing - receives inputs (computes), runs timed functions;; or Delivering its result
 * Delivering - produces output for some time; after that relaxes
 * Relaxing   - is on vacation, no input received; after that goes to Working
 * @endverbatim
 */


/*! \var typedef  GenCompStateMachineType_t
 * The states of the general computing unit's state machine
 * - Dormant: just a technical state, unused units can be switched off
 *   - Can be activated explicitely (after 3 processor clock signals delay)
 *   - Can be activaed implicitely ((after 3 processor clock signals delay)
 * - Ready: The unit is ready, waiting for 'Begin Computing';
 *   after some inactivity, can pass to Dormant
 *   after receiving 'Begin Computing' passes to Computing
 * - Processing: The unit is computing (takes time T_P
 * -     in synchronized technical mode: received synchron signal
 * -     in asynchron mode: received all needed arguments
 * -     in biological mode: received its first argument
 * -     Passes to Delivering (after issueing 'End Computing') if successful, to Ready if not
 * - Delivering : The unit is delivering its result to its output section
 * -     Activates transmission unit to send computed result to its chained unit(s)
 * - Relaxing: Resets state and passes to 'Computing'
 */
typedef enum {gcsm_Dormant, gcsm_Ready, gcsm_Processing, gcsm_Delivering, gcsm_Relaxing, gcsm_Syncronizing, gcsm_Failed} GenCompStateMachineType_t;






class AbstractGenComp_PU;


/*
 * This class implements an abstract computing unit; base for technical and biological computing
 * @verbatim
 * State of the General Computing Unit's state machine
 *
 *                   Dormant<-------> Ready   <---Initially
 *                                  ^      \
 *                                 /        \
 *                                /          v
 *                        Relaxing <-Failed- Processing
 *                               ^           /
 *                                \         /
 *                                 \       v
 *
 *                                 Delivering<----->Synchronizing
 * @endverbatim
 *
 * The general computing is event-driven, i.e. events are received by the abstract state machine
 * and are processed by technical or biological abstract computing unit
 * The system is generated to be in Ready (ready to compute) state.
 */


class AbstractGenCompState {
    public:
        virtual ~AbstractGenCompState(void);
        /**
         * @brief Deliver Signal 'End computing'; result to the 'output section'
         * @param machine The HW that delivers its result
         */
        virtual void Deliver(AbstractGenComp_PU& machine);

        /**
         * @brief Process Signal 'begin computing" received; arguments in the 'input section'; start computing
         * @param machine The HW that starts to process
         */
        virtual void Process(AbstractGenComp_PU& machine);

        /**
         * @brief Relax After finishing processing, resets the HW. Uses @see Reinitialize
         * @param machine
         */
        virtual void Relax(AbstractGenComp_PU& machine);

        /**
         * @brief Reinitialize Sets the HW to its well-defined initial state
         * @param machine
         */
        virtual void Reinitialize(AbstractGenComp_PU& machine);

        /**
         * @brief Synchronize Independently from its actual state, forces the HW to @see Deliver
         * @param machine
         */
        virtual void Synchronize(AbstractGenComp_PU& machine);

        /**
         * @brief Fail Independently from its actual state, forces the HW to @see Deliver
         * @param machine
         */
        virtual void Fail(AbstractGenComp_PU& machine);

        /**
         * @brief HeartBeat The unit updates its internal state
         * @param machine
         */
        virtual void HeartBeat(AbstractGenComp_PU& machine);
        /**
         * @brief Flag_Get Return the code for its internal state
         * @return
         */
        GenCompStateMachineType_t Flag_Get(void){return flag;}

        /**
         * @brief Sleep send the HW to seep if idle for a longer time;  economize power
         * @param machine The HW to be sent to sleep ; just technical
         */
        virtual void Sleep(AbstractGenComp_PU& machine);

        /**
         * @brief WakeUp wake up machine if was sent to sleep;  economize power
         * @param machine The HW to wake up; just technical
         */
        virtual void WakeUp(AbstractGenComp_PU& machine);

        struct{
            sc_core::sc_event
                GenComp_Begin,          // Time to begin computing
                GenComp_End,            // Time to end computing
                GenComp_Fail,           // Computing failed, start over
                GenComp_Awake,          // The HW is needed again, awake it
                GenComp_Relax           // Make a shord coffe brreak
                ;
        }EVENT_GenComp; //< These events are handled by the GenComp state machine
        void State_Set(AbstractGenComp_PU& PU, AbstractGenCompState* st);

    protected:
        void UpdatePU(AbstractGenComp_PU& PU);
        GenCompStateMachineType_t flag;
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
        void Deliver(AbstractGenComp_PU& machine);
        void Process(AbstractGenComp_PU& machine);
        void Relax(AbstractGenComp_PU& machine);
        void Reinitialize(AbstractGenComp_PU& machine);
        void Synchronize(AbstractGenComp_PU& machine);
        void HeartBeat(AbstractGenComp_PU& machine);
};

/**
 * @brief The DormantGenCompState class
 *
 * The PU is going to economize power
 *
 */

class DormantGenCompState : public AbstractGenCompState {
    public:
        DormantGenCompState(void);
        virtual ~DormantGenCompState();
        void Process(AbstractGenComp_PU& machine);
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
        void Process(AbstractGenComp_PU& machine);
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
        void HeartBeat(AbstractGenComp_PU& machine);
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
        void Process(AbstractGenComp_PU& machine);
};

#endif //GenCompStates_h

/** @file GenCompStates_Bio.h
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief Working states of the archetypes of biological processing units
 */
 /*  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef GenCompStates_Bio_h
#define GenCompStates_Bio_h
#include "GenCompStates_Abstract.h"

// Idea from https://stackoverflow.com/questions/14676709/c-code-for-state-machine/19896947
// must not be taken as in SystemC no new electronic module can be created.
// So, scGenComp_PU_Bio handles the events and calls the corresponding virtual transfer routine
//using namespace sc_core; using namespace std;

class scGenComp_PU_Bio;

// This define in only temporarily here,  should go to HWConfig.h
#define USE_PU_HWSLEEPING


/* ! \var typedef  GenCompStateMachineType_t
 * The operation of an elementary computing unit of general computing is modelled as a multiple-state machine,
 * with internal state variables.
 * The states are used in scGenComp_PU_Bio.
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
 * - InputReceive: the unit received new input (a momentary state)
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
//typedef enum {gcsm_Sleeping, gcsm_Ready, gcsm_Processing, gcsm_Delivering, gcsm_Relaxing, gcsm_Synchronizing, gcsm_Failed}
  //            GenCompStateMachineType_t;


/*!
 * \brief
 * This class implements states for an biological computing unit; base for biological computing
 *
 * The general computing is event-driven, i.e. events are received by the biological state machine
 * and are processed by a technical or biological biological processing unit.
 * The PU is generated in Ready (ready to process) state.
 *
 * @see BioGenCompState#EVENT_GenComp
 * @see BioGenCompStateMachineType_t
 */

class GenCompStates_Bio : public GenCompStates_Abstract
{
    public:
        /**
         * @brief Puts the PU state to 'Ready' (called by the scGenComp_PU_Bio's constructor)
         * and sets up its event handling
         */
        GenCompStates_Bio(void);
        virtual ~GenCompStates_Bio(void);
        /**
         * @brief Signal 'End computing' received; issues 'Begin Delivering'
         * do delivering result to the 'output section';
         *
         */
        virtual void Deliver(scGenComp_PU_Abstract *PU);

        /**
         * @brief Process: Signal 'begin computing" received; arguments in the 'input section'; start computing
         */
        virtual void Process(scGenComp_PU_Abstract *PU);

        /**
         * @brief Relax: After finishing processing, resets the HW. Uses @see Reinitialize
         */
        virtual void Relax(scGenComp_PU_Abstract *PU);

        /**
         * @brief Initialize_Do: Sets the state machineto its well-defined initial state
         *
         * @param PU The HW to set
         * A simple subroutine, sets state to 'ready', trigger to

        virtual void Initialize_Do(scGenComp_PU_Abstract* PU);*/

        /**
         * @brief The machine received new input, administer it
         *
         * @param PU The HW to set
         * A simple subroutine, sets state to 'ready', trigger to
         */
        virtual void DoInputReceive(scGenComp_PU_Abstract* PU);

        /**
         * @brief Synchronize: Independently from its actual state, forces the HW to @see Deliver
         */
        virtual void Synchronize(scGenComp_PU_Abstract* PU);

        /**
         * @brief Fail: Can happen only in Processing state; passes to Relaxing state
         * @param PU The HW that failed
         */
        virtual void Fail(scGenComp_PU_Abstract* PU);
        void State_Set(scGenComp_PU_Abstract* PU, GenCompStateMachineType_t& State);

#ifdef USE_PU_HWSLEEPING
         /**
         * @brief Sleep: Send the HW to sleep if idle for a longer time;  economize power
         * @param PU The HW to set
         */
        virtual void Sleep(scGenComp_PU_Abstract* PU);

        /**
         * @brief WakeUp: Wake up machine if was sent to sleep;  economize power
         */
        virtual void Wakeup(scGenComp_PU_Abstract* PU);
        /**
         * @brief State_Set Set the state of PU to st
         * @param PU The HW to set
         */
#endif // USE_PU_HWSLEEPING
    protected:
        void UpdatePU(scGenComp_PU_Bio& PU);
    private:
 };
;

#endif //GenCompStates_Bio__h

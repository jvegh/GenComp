/** @file scGenComp_PU_Bio_HodgkinHuxley.h
 *  @ingroup GENCOMP_MODULE_BIOLOGY

 *  @brief Implements Hodgkin-Huxley model of neuronal operation
 *  @todo Implement the correct DEs
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#ifndef SCBIOGENCOMPHODGKINHUXLEY_H
#define SCBIOGENCOMPIZHIKEVICH_H
/** @addtogroup GENCOMP_MODULE_PROCESS
 *  @{
 */

#include "scGenComp_PU_Bio.h"

/*
 * \class scGenComp_PU_Bio_HodgkinHuxley
 * \brief  A simple abstract class to implement the operation of a general bio computing unit
 *
 * Operating principle @see scGenComp_PU_Bio
 *
 * Some virtual methods overwrite the general "Bio-Computing" behavior, according to HodgkinHuxley's model
 *
 * The rest of methods are of technical nature.
 * - HeartBeat: integrate HodgkinHuxley's PDE
  */

/*!
 * \class scGenComp_PU_Bio_HodgkinHuxley
 * \brief  Implements the HodgkinHuxley-type biological computing PU
     * @param[in] nm the SystemC name of the module
     * @param[in] Heartbeat the state refresh time
     *
     * Creates  an HodgkinHuxley-type biological computing unit:
     *
     * @cite HodgkinHuxley:1952
     *
     * [1] A. L. Hodgkin and A. F. Huxley, A quantitative description of membrane current
     * and its application to conduction and excitation in nerve
     * J. Physiol. (1952) 117:500-544

     *
     *  Hodgkin-Huxley model:
     *  \f[ \frac{dv}{dt} = 0.04*v^2 + 5*v + 140 - u + I \f]
     *  \f[ \frac{du}{dt} = a( b*v - u) \f]
     *   - \f$ v \f$ the membrane potential (in mV)
     *   - \f$ u \f$ the membrane recovery potential (in mV)
     *   - \f$ t \f$ time (in msec, the local time)
     *   - \f$ a \f$ the time scale of the recovery variable \f$ u \f$. (0.02) Smaller values result in slower recovery.
     *   - \f$ b \f$ the sensitivity of the recovery variable \f$ u \f$ to the subthreshold fluctuations of the membrane potential \f$ v \f$
     *   - \f$ c \f$ after-spike reset value of the membrane potential v. (c = 065 mV)
     *   - \f$ d \f$ after-spike reset of the recovery variable of\f$ u \f$ (d=2)
     *
     * This is really HH
     *  V_m        double - Membrane potential in mV
     *  E_L        double - Resting membrane potential in mV.
     *  g_L        double - Leak conductance in nS.
     *  C_m        double - Capacity of the membrane in pF.
     *  tau_ex     double - Rise time of the excitatory synaptic alpha function in ms.
     *  tau_in     double - Rise time of the inhibitory synaptic alpha function in ms.
     *  E_Na       double - Sodium reversal potential in mV.
     *  g_Na       double - Sodium peak conductance in nS.
     *  E_K        double - Potassium reversal potential in mV.
     *  g_K        double - Potassium peak conductance in nS.
     *  Act_m      double - Activation variable m
     *  Act_h      double - Activation variable h
     *  Inact_n    double - Inactivation variable n
     *  I_e        double - Constant external input current in pA.
     *  End of HH
     *
     *
     *   Statios: In different stages the parameters I and V_th are interpreted differently.
     *  - In 'Processing' statio, I is the sum of the synaptic input current plus the introduced direct current:
     *   the PU counts, and sums up the charge received in these two ways, until its threshold voltage V_th reached.
     *   Then the PU passes to 'Delivering' statio: the result is ready, we must deliver it to the hillock.
     *   The ion channels get closed. no more synaptic ionic input and the threshold is about the minimum+25 mV
     *  - In 'Delivering' mode, the I input current is what Na+ and K+ ions from the extracellular space contribute and is much higher than
     *   in 'processing' mode; however no synaptic and direct input. The state is terminated when potential +130 mV reached.
     *   At that point, the spike starts and relaxing begins; passes to 'Relaxing' statio
     *  - In 'Relaxing' statio, some negative ionic current from the extracellular space is present and the
     *   membrane potential decreases below the resting potential. Now the neuron passes to statio 'Ready'.
     *  - in 'Ready' statio, the the ioninc channels are open, but the voltage decays towards the resting potential.
     *   Any input current takes the system to 'Processing, see above.
     *
     *   in the original model, statios 'Processing' and 'Relaxing' are contracted and described by the same function,
     *   'Relaxing' statio is dropped (time skip) and replaced by a drascic and sudden change in v_M
     *
     *   The modified model separates 'Processing' and 'Delivery', introduces a 'Relaxing' statio that has no current input
     *   and relaxes in the same way as the original in 'Ready' statio. The station passes to 'Ready' when the original
     *   membrane cutback point reached.
     *
     * Code uses a Nest fragment
     */

class scGenComp_PU_Bio_HodgkinHuxley : public scGenComp_PU_Bio
{
  public:
    scGenComp_PU_Bio_HodgkinHuxley(sc_core::sc_module_name nm   // Module name
                   ,bool AsPublished // if with omitting statios 'delivering' and sRelaxing'
                   ,sc_core::sc_time Heartbeat);  // Heartbeat time
    virtual ~scGenComp_PU_Bio_HodgkinHuxley(void); // Must be overridden

    /**
     * @brief A new spike received; legal only in 'Ready' and 'Processing' states
     *
     * Called by scGenComp_PU_Abstract::InputReceived_method.
     * A spike arrived, store spike parameters. Receving an input is a momentary action, just administer its processing.
     * If it was the first spike, issue 'Begin_Computing'
     *
     * Reimplemented given that in biology the first input also starts processing
     * Receiving input is recently just symbolic, no details elaborated
     */
    virtual void InputReceived_Do();

    /* *
     * @brief Called when the state 'Relaxing' begins
     *
      */
//    virtual void RelaxingBegin_Do();

    /**
     * @brief Called when the state 'Relaxing' ends
     *
     * The unit passes to phase 'Ready'
     */
    virtual void RelaxingEnd_Do(){}

    /**
     * @brief Heartbeat_method
     *
     * A periodic  signal as a timebase for solving differential equations.
     *
     * - The unit receives a signal EVENT_GenComp.HeartBeat and calculates the membran potential.
     *
     *   Most of the job is done in methods Heartbeat_Processing_Do() (This was elaborated first),
     * Heartbeat_Delivering_Do() and  Heartbeat_Relaxing_Do() and Heartbeat_Ready_Do().
     * These Heartbeat_XXX_Do() methods work on the same principle, only their function and terminating condition are different
     * (furthermore, the station to which they pass). They issue the corresponding event to pas to the next statio.

     * - In 'Processing' mode, re-calculates membrane's charge-up potential
     * - In 'Ready' mode, re-calculates membrane's decay potential
     * - In 'Delivering' mode, re-calculates membrane's decay potential
     */


  protected:
     /*
     * Handle heartbeats in 'Ready' mode
     */
    /**
     * @brief Handle heartbeats in 'Processing' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential reaches its threshold value
     * -  Starts to receive heartbeats after EVENT_GenComp.ProcessingBegin arrives
     * -  Stops generating heartbeats and issues EVENT_GenComp.ProcessingEnd
     * -  Issues EVENT_GenComp.Fail whey the membrane potential decaya near to its threshold
     */
    virtual void Heartbeat_Processing_Do();

    /**
     * @brief Handle heartbeats in 'Delivering' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential at hillock reaches its peak value
     * -  Starts to receive heartbeats after EVENT_GenComp.DeliveringBegin arrives
     * -  Stops generating heartbeats and issues EVENT_GenComp.DeliveringEnd
     */
    void Heartbeat_Delivering_Do();

    /**
     * @brief Handle heartbeats in 'Ready' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential reaches its recovery value
     * -  Starts to receive heartbeats after EVENT_GenComp.ReadyBegin arrives
     * -  Stops generating heartbeats and issues EVENT_GenComp.ReadyEnd
     */
    void Heartbeat_Ready_Do();

    /**
     * @brief Handle heartbeats in 'Relaxing' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential starts to increase
     * -  Starts to receive heartbeats after EVENT_GenComp.RelaxingBegin arrives
     * -  Stops generating heartbeats after EVENT_GenComp.DeliveringEnd  arrives
     *     then issues EVENT_GenComp.Initializing
     */
     void Heartbeat_Relaxing_Do();

    /**
     * @brief SolvePDE : solve the differential equations for the actual time increment
     *
     *  HodgkinHuxley model:
     *  \f[ \frac{dv}{dt} = 0.04*v^2 + 5*v + 140 - u + I \f]
     *  \f[ \frac{du}{dt} = a( b*v - u) \f]
     *   - \f$ v \f$ the membrane potential (in mV)
     *   - \f$ u \f$ the membrane recovery potential (in mV)
     *   - \f$ t \f$ time (in msec, the local time)
     *   - \f$ a \f$ the time scale of the recovery variable \f$ u \f$. (0.02) Smaller values result in slower recovery.
     *   - \f$ b \f$ the sensitivity of the recovery variable \f$ u \f$ to the subthreshold fluctuations of the membrane potential \f$ v \f$
     *   - \f$ c \f$ after-spike reset value of the membrane potential v. (c = 065 mV)
     *   - \f$ d \f$ after-spike reset of the recovery variable of\f$ u \f$ (d=2)
     *
     *  The state of the biological computing is re-calculated (as the simulation time passes)
     *  (solve the differential equation at this time; reset takes place in RelaxingBegin_Do )
     *  @see scGenComp_PU_Bio#RelaxingBegin_Do
     */
     /**
     * @brief Threshold_Exceeded
     * @return true if spike generation threshold exceeded and 'Delivering'  follows
     */
    bool ThresholdExceeded(void);

    /**
     * @brief Peak_Exceeded
     * @return true if spike generation threshold exceeded and 'Relaxing' follows
     */
    bool PeakExceeded(void);

    /**
     * @brief Resting potential approached
     * @return true if potential approached mV_M to < m_C + m_D
     */
    bool RestingApproached(void);

    /**
     * @brief Resting potential reached
     */
    bool RestingReached(void);

    /**
     *
     * @brief InputCurrent_Set
     * @param[in] I The current, in pA
     */
    void InputCurrent_Set(int I) {I_e = I;}
    int InputCurrent_Get(void) {return I_e;}
    /**
     * @brief UseAsPublished_Set
     * @param[in] B if to use the method as published
     *
     * As published, the method uses no delivery and refractory periods
     */
//    void UseAsPublished_Set(bool B) { mAsPublished = B;}
//    bool UseAsPublished_Get(void){return mAsPublished;}
protected:
    //        : t_ref_( 2.0 )   // ms
    double mTimeStep    //As documented in Nest
        , V_M      // mV, membrane potential
        , g_Na     // nS, Sodium peak conductance
        , g_K      // nS, Potassium peak conductance
        , g_L      // nS, Leak conductance
        , C_M      // pF, Capacity of the membrane
        , E_Na     // mV, Sodium reversal potential
        , E_K      // mV, Potassium reversal potential
        , E_L      // mV, Resting membrane potential
        , tau_synE // ms
        , tau_synI // ms
        , I_e      // pA Constant external input current

        ,m
        ,n
        ;
/*

                tau_ex     double - Rise time of the excitatory synaptic alpha function in ms.
                tau_in     double - Rise time of the inhibitory synaptic alpha function in ms.

                .
                Act_m      double - Activation variable m
              Act_h      double - Activation variable h
              Inact_n    double - Inactivation variable n
         bool mAsPublished;
*/
  };// of class scGenComp_PU_Bio_HodgkinHuxley
/** @}*/

#endif // SCBIOGENCOMPIZHIKEVICH_H

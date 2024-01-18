/** @file scGenComp_PU_Bio_Izhikevich.cpp
 *  @ingroup GENCOMP_MODULE_BIOLOGY
 *  @brief   Implements Izhikevich's model
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#include "scGenComp_PU_Bio_Izhikevich.h"
// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing
//static GenCompStates_Bio *TheGenCompStates_Bio;


// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing
    scGenComp_PU_Bio_Izhikevich::
scGenComp_PU_Bio_Izhikevich(sc_core::sc_module_name nm
                ,sc_core::sc_time Heartbeat ):  // Heartbeat time
    scGenComp_PU_Bio(nm, Heartbeat)
/*
 *  a_( 0.02 )                                      // a
  , b_( 0.2 )                                       // b
  , c_( -65.0 )                                     // c without unit
  , d_( 8.0 )                                       // d
  , I_e_( 0.0 )                                     // pA
  , V_th_( 30.0 )                                   // mV
  , V_min_( -std::numeric_limits< double >::max() ) // mV

  : v_( -65.0 ) // membrane potential
  , u_( 0.0 )   // membrane recovery variable
  , I_( 0.0 )   // input current
*/
    ,mV_M (-65) // mV
    ,mU_R (0.)   //mV
    ,mV_Th (30.) //mV
    ,mI_In(100.) //mV
    ,mV_Min (-70)
    ,m_A(0.02)
    ,m_B(0.2)
    ,m_C(-65.)
    ,m_D(8.)
    ,mAsPublished(false)
{
    typedef scGenComp_PU_Bio_Izhikevich SC_CURRENT_USER_MODULE;
    mTimeStep = HeartbeatTime_Get().to_seconds();
    // *** Do not reimplement any of the xxx_method functions
    // *** until you know what you are doing. Do what you want in methods xxx_Do
}
    scGenComp_PU_Bio_Izhikevich::
~scGenComp_PU_Bio_Izhikevich(void)
{
}



    /**
     * Handle heartbeats in 'Processing' mode
     */
void scGenComp_PU_Bio_Izhikevich::
    Heartbeat_Processing_Do()
{
    double LocalTime = scLocalTime_Get().to_seconds()*1000*1000; // Have the time in us
    double RefinedTime = HeartbeatTime_Get().to_seconds()*1000*1000; // Have the time step refinement in us
    double N = HeartbeatDivisions_Get();
    double mTimeStep = RefinedTime/N/1000.;
    for(int32_t i = 0; (i< N) and !Processing_Finished(); i++)
    {
        SolvePDE();
        DEBUG_SC_EVENT_LOCAL("V(memb)=" << mV_M << ", U(rec)=" << mU_R << ", N=" << i);
    }
    if (Processing_Finished())
    {   // We are about finishing processing
        EVENT_GenComp.ProcessingEnd.notify(SC_ZERO_TIME);
                    DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.ProcessingEnd");
     }
    else
    {   // We are still processing; re-issue the heartbeat
           // if the limit is not yet reached
        EVENT_GenComp.Heartbeat.notify(mHeartbeat);
                    DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with mHeartbeat");
    }
}



    /**
     *
     * Handle heartbeats in 'Relaxing' mode
     */
void scGenComp_PU_Bio_Izhikevich::
    Heartbeat_Relaxing_Do()
{
    if (scLocalTime_Get() < sc_time(8*HEARTBEAT_TIME_DEFAULT_BIO))
    {   // We are still processing; re-issue the heartbeat
        // if the limit is not yet reached
        EVENT_GenComp.Heartbeat.notify(HEARTBEAT_TIME_DEFAULT_BIO);
                DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with BIO_HEARTBEAT_TIME");
    }
    else
    {   // We are about finishing processing
        EVENT_GenComp.RelaxingEnd.notify(SC_ZERO_TIME);
                DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.RelaxingEnd");
    }
}

void scGenComp_PU_Bio_Izhikevich::
    RelaxingBegin_Do()
{
    mV_M = m_C; // Restore membrane potential
    mU_R = m_D; // Restore recovery potential
}
    /*  S_.v_ = P_.c_;
        S_.u_ = S_.u_ + P_.d_;*/

/*
 * Initialize the GenComp unit.
 */
void scGenComp_PU_Bio_Izhikevich::
    Initialize_Do(void)
{
    scGenComp_PU_Bio::Initialize_Do();
                DEBUG_SC_EVENT_LOCAL("Initialized for Izhikevich mode");
 }


/*
 * This routine makes actual input processing, although most of the job is done in Process() and Heartbeat()
 * It can be called in state 'Processing' (if not first input)
 * or in state 'Ready' if first input
 *
 * If it was the first spike, issue 'ComputingBegin' and re-issue

 */
void scGenComp_PU_Bio_Izhikevich::
   InputReceived_Do(void)
{
    if(!((gcsm_Ready == mStateFlag) || (gcsm_Processing == mStateFlag))) return;
    // inputs are processed only in 'Ready' and 'Processing' states
    if(gcsm_Ready == mStateFlag)
    {   // we are still in 'Ready' state
        mStateFlag = gcsm_Processing;   // Be sure we do not repeat
        EVENT_GenComp.ProcessingBegin.notify(SC_ZERO_TIME); // Put events in order
                DEBUG_SC_EVENT_LOCAL("SEND EVENT_GenComp.ProcessingBegin");
        EVENT_GenComp.InputReceived.notify(1,SC_PS); // Re-issue InputReceived
                DEBUG_SC_EVENT_LOCAL("re-SEND EVENT_GenComp.InputReceived with 1 ps delay");
    }
    else
    {
        DEBUG_SC_EVENT_LOCAL("RCVD EVENT_GenComp.InputReceived in mode '" << GenCompStatesString[mStateFlag] << "'");
        scGenComp_PU_Bio::InputReceived_Do();
    }
 }



bool scGenComp_PU_Bio_Izhikevich::
    Processing_Finished(void)
{
    return mV_M>=mV_Th;
    // threshold crossing
 /*   if ( S_.v_ >= P_.V_th_ )*/
}

void scGenComp_PU_Bio_Izhikevich::
    SolvePDE()
{    
        double OldV = mV_M;
        double OldU = mU_R;

        mV_M += mTimeStep*  ( 0.04 * OldV * OldV + 5.0 *OldV + 140.0 - OldU //??+ mI_e +
                                    + mI_In );
        mU_R += mTimeStep * m_A * (m_B * OldV - OldU);
        // lower bound of membrane potential
        if(mV_M<mV_Min)
            mV_M = mV_Min; // Restore membrane potential
}
//S_.v_ += h * ( 0.04 * v_old * v_old + 5.0 * v_old + 140.0 - u_old + S_.I_ + P_.I_e_ )
//       + B_.spikes_.get_value( lag );
//        S_.u_ += h * P_.a_ * ( P_.b_ * v_old - u_old );
//    S_.v_ = ( S_.v_ < P_.V_min_ ? P_.V_min_ : S_.v_ );

/* Izhikevich parameters
 *
 *  a_( 0.02 )                                      // a
  , b_( 0.2 )                                       // b
  , c_( -65.0 )                                     // c without unit
  , d_( 8.0 )                                       // d
  , I_e_( 0.0 )                                     // pA
  , V_th_( 30.0 )                                   // mV
  , V_min_( -std::numeric_limits< double >::max() ) // mV

  : v_( -65.0 ) // membrane potential
  , u_( 0.0 )   // membrane recovery variable
  , I_( 0.0 )   // input current

*/

/* Nest method for calculating
 *     if ( P_.consistent_integration_ )
    {
      v_old = S_.v_;
      u_old = S_.u_;
      S_.v_ += h * ( 0.04 * v_old * v_old + 5.0 * v_old + 140.0 - u_old + S_.I_
                     + P_.I_e_ )
        + B_.spikes_.get_value( lag );
      S_.u_ += h * P_.a_ * ( P_.b_ * v_old - u_old );
    }

    {
        double OldPotential = mMembranePotential;
        double OldRecovery = mRecoveryPotential;

        mMembranePotential += mTimeStep*  ( 0.04 * OldPotential * OldPotential + 5.0 *OldPotential + 140.0 - OldRecovery + OldCurrent +
                     + mParam_I )
      S_.v_ += h * ( 0.04 * v_old * v_old + 5.0 * v_old + 140.0 - u_old + S_.I_
                     + P_.I_e_ )
        + B_.spikes_.get_value( lag );
      S_.u_ += h * P_.a_ * ( P_.b_ * v_old - u_old );
        mRecoveryPotential += mTimeStep * mParam_A * (mParamB * OldPotential - OldRecovery);
    }

    // lower bound of membrane potential
    S_.v_ = ( S_.v_ < P_.V_min_ ? P_.V_min_ : S_.v_ );

*/

/*
 *     // threshold crossing
    if ( S_.v_ >= P_.V_th_ )
    {
      S_.v_ = P_.c_;
      S_.u_ = S_.u_ + P_.d_;

      // compute spike time
      set_spiketime( Time::step( origin.get_steps() + lag + 1 ) );

      SpikeEvent se;
      kernel().event_delivery_manager.send( *this, se, lag );
    }

    // set new input current
    S_.I_ = B_.currents_.get_value( lag );

    // voltage logging
    B_.logger_.record_data( origin.get_steps() + lag );
  }
*/




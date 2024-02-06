/** @file scGenComp_PU_Bio_HodgkinHuxley.cpp
 *  @ingroup GENCOMP_MODULE_BIOLOGY
 *  @brief   Implements Hodgkin-Huxley's model
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#include "scGenComp_PU_Bio_HodgkinHuxley.h"
// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#define DEBUG_DISABLED
#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing


// Implement Hodgkin-Huxley model
    scGenComp_PU_Bio_HodgkinHuxley::
scGenComp_PU_Bio_HodgkinHuxley(sc_core::sc_module_name nm
                                ,bool AsPublished
                ,sc_core::sc_time Heartbeat ):  // Heartbeat time
    scGenComp_PU_Bio(nm, Heartbeat)
    , g_Na( 12000.0 ) // nS, Sodium peak conductance
    , g_K( 3600.0 )   // nS, Potassium peak conductance
    , g_L( 30.0 )     // nS, Leak conductance
    , C_M (100.0 )    // pF, Capacity of the membrane
    , E_Na( 50.0 )    // mV, Sodium reversal potential
    , E_K( -77.0 )    // mV, Potassium reversal potential
    , E_L( -54.402 )  // mV,
    , V_M(-65.)       // Resting membrane potential
    , tau_synE( 0.2 ) // ms
    , tau_synI( 2.0 ) // ms
    , I_e( 0.0 )      // pA Constant external input current
    , m(1.)
    , n(1.)
{
    typedef scGenComp_PU_Bio_HodgkinHuxley SC_CURRENT_USER_MODULE;
//    double RefinedTime = HeartbeatTime_Get().to_seconds()*1000; // Have the time step refinement in ms
  //  mTimeStep = RefinedTime/HeartbeatDivisions_Get();
    // *** Do not reimplement any of the xxx_method functions
    // *** until you know what you are doing. Do what you want in methods xxx_Do

     double V_M = -65.; // mV
    double v = 1.; // mm/ms
    double C_k = 1;
    double I_i = 1;
    double dt = 0.2;
    bool cont = true;
    double V_ext = -25;
    double e = .5;
    double I_Ca = 10.;
    double I_K = 5.;
    double I_l = -2;
    double C_M = 1000;
    double t = 0;
    double I_M = 0;
    double velo;
    double t_limit = 5;
    while(t<6.5)
    {//
        if(t<t_limit)
        {
        velo = e*(V_ext-V_M);
        I_M += velo*(I_Ca + I_K);
        V_M += dt/C_M*velo*(I_M + I_l);
        cerr << "(" << t <<  "," << 0.0028*I_M << ")" << endl;
        }
         else
             {
 //           if((t>2.7) && (t<3.3)) I_M -= 150;
            I_M -= I_M*2*dt;
            cerr << "(" << t << "," <<  0.0028*I_M << ")" << endl;
//                 cerr << "(" << t << "," << 6.6*exp(-2*t) << ")" << endl;;
         }
        t += dt;
    }
    // Concentration
}
    scGenComp_PU_Bio_HodgkinHuxley::
~scGenComp_PU_Bio_HodgkinHuxley(void)
{
}


/*
     * Handle heartbeats in 'Delivering' mode
     */
void scGenComp_PU_Bio_HodgkinHuxley::
    Heartbeat_Delivering_Do()
{
    //double LocalTime = scLocalTime_Get().to_seconds()*1000; // Have the time in ms
//    double MyCurrent;
 /*   if(mAsPublished)
    {   // Use with the input current
        MyCurrent = mI_In;
    }
    else
    {   // Use charge from the extracellular space. temporary
        MyCurrent = 5*mI_In;
    }*/
    for(int32_t i = 0; (i < mHeartbeatDivisions) and !PeakExceeded(); i++)
    {
        // ** This part solves the PDE
        double h = 1;

    const double I_Na = g_Na * m * m * m * h * ( V_M - E_Na );
    const double I_K = g_K * n * n * n * n * ( V_M - E_K );
    const double I_L = g_L * ( V_M - E_L );
    V_M = ( -( I_Na + I_K + I_L ) //+I_stim_ + I_e + I_ex + + I_in
       ) / C_M;



/*        double OldV = mV_M;
        double OldU = mU_R;

        mV_M += mTimeStep * ( 0.04 * OldV * OldV + 5.0 *OldV + 140.0 - OldU + MyCurrent ); //??+ spike contrib
        mU_R += mTimeStep * m_A * (m_B * OldV - OldU);
        // check lower bound of membrane potential
        if(mV_M<mV_Min)
            mV_M = mV_Min; // Restore membrane potential
        // ** end of pDE solving
*/
        DEBUG_SC_EVENT_LOCAL("@" << LocalTime+ i*mTimeStep <<  "; V(memb)=" << mV_M << ", U(rec)=" << mU_R << ", N=" << i);
    }
    if (PeakExceeded())
    {   // We are about finishing processing
        EVENT_GenComp.DeliveringEnd.notify(SC_ZERO_TIME);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.DeliveringEnd");
    }
    else
    {   // We are still processing; re-issue the heartbeat
        // if the limit is not yet reached
        EVENT_GenComp.Heartbeat.notify(mHeartbeat);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with " << sc_time_String_Get(mHeartbeat, SC_TIME_UNIT_DEFAULT)<< " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT]);
    }
}



    /*
     * Handle heartbeats in 'Processing' mode
     */
void scGenComp_PU_Bio_HodgkinHuxley::
    Heartbeat_Processing_Do()
{
    //double LocalTime = scLocalTime_Get().to_seconds()*1000; // Have the time in ms
 //   for(int32_t i = 0; (i < mHeartbeatDivisions) and !ThresholdExceeded(); i++)
    {
        // ** This part solves the PDE
 /*           double OldV = mV_M;
            double OldU = mU_R;

            mV_M += mTimeStep * ( 0.04 * OldV * OldV + 5.0 *OldV + 140.0 - OldU + mI_In ); //??+ spike contrib
            mU_R += mTimeStep * m_A * (m_B * OldV - OldU);
            // check lower bound of membrane potential
            if(mV_M<mV_Min)
                mV_M = mV_Min; // Restore membrane potential
        // ** end of pDE solving
 */
        DEBUG_SC_EVENT_LOCAL("@" << LocalTime+ i*mTimeStep <<  "; V(memb)=" << mV_M << ", U(rec)=" << mU_R << ", N=" << i);
    }
    if (ThresholdExceeded())
    {   // We are about finishing processing
        EVENT_GenComp.ProcessingEnd.notify(SC_ZERO_TIME);
                    DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.ProcessingEnd");
    }
    else
    {   // We are still processing; re-issue the heartbeat
           // if the limit is not yet reached
        EVENT_GenComp.Heartbeat.notify(mHeartbeat);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with +" << sc_time_String_Get(mHeartbeat, SC_TIME_UNIT_DEFAULT) << " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT]
                             << ", in " << HeartbeatDivisions_Get() << " divisions");
    }
}


    /**
     * Handle heartbeats in 'Relaxing' mode
     */

void scGenComp_PU_Bio_HodgkinHuxley::
    Heartbeat_Relaxing_Do()
{
    //if(mAsPublished)
    {   // Just restore potentials
/*        mV_M = m_C; // Restore membrane potential
        mU_R += m_D; // Restore recovery potential
           // We are about finishing processing
            EVENT_GenComp.RelaxingEnd.notify(SC_ZERO_TIME);
            DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.RelaxingEnd");
*/
        return;
    }
/*
    double  MyCurrent = 10*mI_In;
    // This is the updated processing method
    double LocalTime = scLocalTime_Get().to_seconds()*1000; // Have the time in ms
    for(int32_t i = 0; (i < mHeartbeatDivisions) and !RestingApproached(); i++)
    {
         // ** This part solves the PDE
         double OldV = mV_M;
         double OldU = mU_R;

         mV_M += mTimeStep * ( //0.04 * OldV * OldV + 5.0 *OldV
                     130
                              + 140.0 - OldU - MyCurrent); // No input current
         mU_R += mTimeStep * m_A * (m_B * OldV - OldU);
         // check lower bound of membrane potential
         if(mV_M<mV_Min)
             mV_M = mV_Min; // Restore membrane potential
         // ** end of pDE solving
         DEBUG_SC_EVENT_LOCAL("@" << LocalTime+ i*mTimeStep <<  "; V(memb)=" << mV_M << ", U(rec)=" << mU_R << ", N=" << i);
    }
*/
     if (RestingApproached())
     {   // We are about finishing processing
         EVENT_GenComp.RelaxingEnd.notify(SC_ZERO_TIME);
         DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.RelaxingEnd");
     }
     else
     {   // We are still processing; re-issue the heartbeat
         // if the limit is not yet reached
         EVENT_GenComp.Heartbeat.notify(mHeartbeat);
         DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with  " << sc_time_String_Get(mHeartbeat, SC_TIME_UNIT_DEFAULT)<< " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT]
                                                                                                                                    << ", in " << HeartbeatDivisions_Get() << " divisions");
     }
}

/**
     * Handle heartbeats in 'Ready' mode
     */

void scGenComp_PU_Bio_HodgkinHuxley::
    Heartbeat_Ready_Do()
{
 /*   if(mAsPublished)
    {   // Nothing to do
        mV_M = m_C; // Restore membrane potential
        mU_R += m_D; // Restore recovery potential
        {   // We are about finishing processing
            EVENT_GenComp.RelaxingEnd.notify(SC_ZERO_TIME);
            DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.RelaxingEnd");
        }
    }
    // This is the updated processing method
    double LocalTime = scLocalTime_Get().to_seconds()*1000; // Have the time in ms
    for(int32_t i = 0; (i < mHeartbeatDivisions) and !RestingApproached(); i++)
    {
        // ** This part solves the PDE
        double OldV = mV_M;
        double OldU = mU_R;

        mV_M += mTimeStep * ( 0.04 * OldV * OldV + 5.0 *OldV + 140.0 - OldU - mI_In ); // No input current
        mU_R += mTimeStep * m_A * (m_B * OldV - OldU);
        // check lower bound of membrane potential
        if(mV_M<mV_Min)
            mV_M = mV_Min; // Restore membrane potential
        // ** end of pDE solving
        DEBUG_SC_EVENT_LOCAL("@" << LocalTime+ i*mTimeStep <<  "; V(memb)=" << mV_M << ", U(rec)=" << mU_R << ", N=" << i);
    }
    if (RestingApproached())
    {   // We are about finishing processing
        mV_M = m_C; // Restore membrane potential, as published
        mU_R += m_D; // Restore recovery potential
        DEBUG_SC_EVENT_LOCAL("End of processing, in ready again");
    }
    else
    {   // We are still processing; re-issue the heartbeat
        // if the limit is not yet reached
        EVENT_GenComp.Heartbeat.notify(mHeartbeat);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with  " << sc_time_String_Get(mHeartbeat, SC_TIME_UNIT_DEFAULT)<< " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT]
                                                                                                                                   << ", in " << HeartbeatDivisions_Get() << " divisions");
    }
*/
}

/*
 * This routine makes actual input processing, although most of the job is done in Process() and Heartbeat()
 * It can be called in state 'Processing' (if not first input)
 * or in state 'Ready' if first input
 *
 * If it was the first spike, issue 'ComputingBegin' and re-issue
 */
void scGenComp_PU_Bio_HodgkinHuxley::
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
//        DEBUG_SC_EVENT_LOCAL("RCVD EVENT_GenComp.InputReceived in mode '" << GenCompStatesString[mStateFlag] << "'");
        //scGenComp_PU_Bio::InputReceived_Do();
    }
 }



bool scGenComp_PU_Bio_HodgkinHuxley::
    ThresholdExceeded(void)
{
    //return mV_M>=mV_Min+25;   // threshold crossing
    return false;
}

bool scGenComp_PU_Bio_HodgkinHuxley::
    PeakExceeded(void)
{
 //   return mV_M>=130;   // Peak voltage crossing
    return false;
}

bool scGenComp_PU_Bio_HodgkinHuxley::
    RestingApproached(void)
{
      //return mV_M < m_C+m_D;   // Restoration potential approached
    return false;
}
bool scGenComp_PU_Bio_HodgkinHuxley::
    RestingReached(void)
{
  //  return mV_M < m_C+m_D/4;   // Restoration potential approached
    return false;
}

/*
 *     // threshold crossing
    if ( S_.v_ >= P_.V_th_ )
    {

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




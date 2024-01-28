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
#define DEBUG_DISABLED
#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing

// Implement Izhikevich model
    scGenComp_PU_Bio_Izhikevich::
scGenComp_PU_Bio_Izhikevich(sc_core::sc_module_name nm
                                ,bool AsPublished
                ,sc_core::sc_time Heartbeat ):  // Heartbeat time
    scGenComp_PU_Bio(nm, Heartbeat)
    ,mV_M (-65)  //mV, membrane potential -65*random[0,1]
    ,mU_R (0.)   //mV membrane recovery variable, m_B * mV_M
    ,mV_Th (30.) //mV; fixed 30
    ,mI_In(40.) //mV*/R [5*random[0,1]-2*random[0,1]
    ,mV_Min (-70.)  // Minimum membrane voltage
    ,m_A(0.02)      // [0.02,0.1] * random[0,1]
    ,m_B(0.2)       // [0.2,0.27] * random[0,1]
    ,m_C(-65.)      // [-65,-50] + 15*random[0,1]^2
    ,m_D(8.)        // [0.05,8] - 6*random[0,1]^2
    ,mAsPublished(AsPublished)    // If to use without 'Relaxing' and 'Delvering'
{
    typedef scGenComp_PU_Bio_Izhikevich SC_CURRENT_USER_MODULE;
    double RefinedTime = HeartbeatTime_Get().to_seconds()*1000; // Have the time step refinement in ms
    mTimeStep = RefinedTime/HeartbeatDivisions_Get();
    // *** Do not reimplement any of the xxx_method functions
    // *** until you know what you are doing. Do what you want in methods xxx_Do
}
    scGenComp_PU_Bio_Izhikevich::
~scGenComp_PU_Bio_Izhikevich(void)
{
}


/*
     * Handle heartbeats in 'Delivering' mode
     */
void scGenComp_PU_Bio_Izhikevich::
    Heartbeat_Delivering_Do()
{
    double LocalTime = scLocalTime_Get().to_seconds()*1000; // Have the time in ms
    double MyCurrent;
    if(mAsPublished)
    {   // Use with the input current
        MyCurrent = mI_In;
    }
    else
    {   // Use charge from the extracellular space. temporary
        MyCurrent = 5*mI_In;
    }
    for(int32_t i = 0; (i < mHeartbeatDivisions) and !PeakExceeded(); i++)
    {
        // ** This part solves the PDE
        double OldV = mV_M;
        double OldU = mU_R;

        mV_M += mTimeStep * ( 0.04 * OldV * OldV + 5.0 *OldV + 140.0 - OldU + MyCurrent ); //??+ spike contrib
        mU_R += mTimeStep * m_A * (m_B * OldV - OldU);
        // check lower bound of membrane potential
        if(mV_M<mV_Min)
            mV_M = mV_Min; // Restore membrane potential
        // ** end of pDE solving
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
void scGenComp_PU_Bio_Izhikevich::
    Heartbeat_Processing_Do()
{
    double LocalTime = scLocalTime_Get().to_seconds()*1000; // Have the time in ms
    for(int32_t i = 0; (i < mHeartbeatDivisions) and !ThresholdExceeded(); i++)
    {
        // ** This part solves the PDE
            double OldV = mV_M;
            double OldU = mU_R;

            mV_M += mTimeStep * ( 0.04 * OldV * OldV + 5.0 *OldV + 140.0 - OldU + mI_In ); //??+ spike contrib
            mU_R += mTimeStep * m_A * (m_B * OldV - OldU);
            // check lower bound of membrane potential
            if(mV_M<mV_Min)
                mV_M = mV_Min; // Restore membrane potential
        // ** end of pDE solving
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

void scGenComp_PU_Bio_Izhikevich::
    Heartbeat_Relaxing_Do()
{
    if(mAsPublished)
    {   // Just restore potentials
        mV_M = m_C; // Restore membrane potential
        mU_R += m_D; // Restore recovery potential
           // We are about finishing processing
            EVENT_GenComp.RelaxingEnd.notify(SC_ZERO_TIME);
            DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.RelaxingEnd");
            return;
    }

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

void scGenComp_PU_Bio_Izhikevich::
    Heartbeat_Ready_Do()
{
    if(mAsPublished)
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
//        DEBUG_SC_EVENT_LOCAL("RCVD EVENT_GenComp.InputReceived in mode '" << GenCompStatesString[mStateFlag] << "'");
        scGenComp_PU_Bio::InputReceived_Do();
    }
 }



bool scGenComp_PU_Bio_Izhikevich::
    ThresholdExceeded(void)
{
    return mV_M>=mV_Min+25;   // threshold crossing
}

bool scGenComp_PU_Bio_Izhikevich::
    PeakExceeded(void)
{
    return mV_M>=130;   // Peak voltage crossing
}

bool scGenComp_PU_Bio_Izhikevich::
    RestingApproached(void)
{
      return mV_M< m_C+m_D;   // Restoration potential approached
}
bool scGenComp_PU_Bio_Izhikevich::
    RestingReached(void)
{
    return mV_M< m_C+m_D/4;   // Restoration potential approached
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




/** @file scGenComp_PU_Bio.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#include "scGenComp_PU_Bio.h"
// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing
static GenCompStates_Bio *TheGenCompStates_Bio;

// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing
    scGenComp_PU_Bio::
scGenComp_PU_Bio(sc_core::sc_module_name nm
                ,sc_core::sc_time HeartBeat ):  // Heartbeat time
    scGenComp_PU_Abstract(nm)
{
    mHeartbeat = HEARTBEAT_TIME_DEFAULT_BIO;
    typedef scGenComp_PU_Bio SC_CURRENT_USER_MODULE;
    if(!TheGenCompStates_Bio)
        TheGenCompStates_Bio = new GenCompStates_Bio(); // We need one singleton copy of state machine
    MachineState =  TheGenCompStates_Bio;     // However, the state flag is stored per PU object
    // *** Do not reimplement any of the xxx_method functions
    // *** until you know what you are doing. Do what you want in methods xxx_Do
}

    scGenComp_PU_Bio::
~scGenComp_PU_Bio(void)
{
}


/*
void scGenComp_PU_Bio::
    Deliver()
{   // The state machine ensures that we are in phases either 'Processing' or 'Delivering'
    DEBUG_SC_EVENT_LOCAL("   ---");
    if(gcsm_Processing == StateFlag_Get())
    {   // We are at the beginning of the 'Delivering' phase
        StateFlag_Set(gcsm_Delivering);
    }
    else
    {   // We are at the end of the 'Delivering' phase
        EVENT_GenComp.DeliveringEnd.notify(SC_ZERO_TIME);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.End_Delivering");
        MachineState->Relax(this);
    }
}
*/

// Heartbeat distribution happens in scGenComp_PU_Abstract
    /**
     * Handle heartbeats in 'Delivering' mode
     */
void scGenComp_PU_Bio::
    Heartbeat_Delivering_Do()
{
    if (scLocalTime_Get() < sc_time(8*HEARTBEAT_TIME_DEFAULT_BIO))
    {   // We are still processing; re-issue the heartbeat
        // if the limit is not yet reached
        EVENT_GenComp.Heartbeat.notify(HEARTBEAT_TIME_DEFAULT_BIO);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with BIO_HEARTBEAT_TIME");
    }
    else
    {   // We are about finishing processing
        EVENT_GenComp.ProcessingEnd.notify(SC_ZERO_TIME);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.DeliveringEnd");
    }
}

    /**
     * Handle heartbeats in 'Processing' mode
     */
void scGenComp_PU_Bio::
    Heartbeat_Processing_Do()
{
    SolvePDE();
    if (Processing_Finished())
    {   // We are about finishing processing
        EVENT_GenComp.ProcessingEnd.notify(SC_ZERO_TIME);
                    DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.ProcessingEnd");
        //            EVENT_GenComp.Heartbeat.cancel();
        //                    DEBUG_SC_EVENT_LOCAL("CNCL    EVENT_GenComp.Heartbeat");
    }
    else
    {   // We are still processing; re-issue the heartbeat
           // if the limit is not yet reached
        EVENT_GenComp.Heartbeat.notify(mHeartbeat);
                    DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with mHeartbeat");
    }
}

    /**
     * Handle heartbeats in 'Ready' mode
     */
void scGenComp_PU_Bio::
    Heartbeat_Ready_Do()
{
    if (scLocalTime_Get() < sc_time(2*HEARTBEAT_TIME_DEFAULT_BIO))
    {   // We are still processing; re-issue the heartbeat
        // if the limit is not yet reached
        EVENT_GenComp.Heartbeat.notify(HEARTBEAT_TIME_DEFAULT_BIO);
                //DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.HeartBeat with BIO_HEARTBEAT_TIME");
    }
    else
    {   // We are about finishing processing
        EVENT_GenComp.ProcessingEnd.notify(SC_ZERO_TIME);
    }
}

    /**
     *
     * Handle heartbeats in 'Relaxing' mode
     */
void scGenComp_PU_Bio::
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

/*
 * Initialize the GenComp unit.
 */
void scGenComp_PU_Bio::
    Initialize_Do(void)
{
    scGenComp_PU_Abstract::Initialize_Do();
                DEBUG_SC_EVENT_LOCAL("Initialized for BIO mode");
 }


/*
 * This routine makes actual input processing, although most of the job is done in Process() and Heartbeat()
 * It can be called in state 'Processing' (if not first input)
 * or in state 'Ready' if first input
 *
 * If it was the first spike, issue 'ComputingBegin' and re-issue

 */
void scGenComp_PU_Bio::
   InputReceived_Do(void)
{
    if(!((gcsm_Ready == mStateFlag) || (gcsm_Processing == mStateFlag))) return;
    // inputs are processed only in 'Ready' and 'Processing' states
                DEBUG_SC_EVENT_LOCAL("RCVD EVENT_GenComp.InputReceived in mode '" << GenCompStatesString[mStateFlag] << "'");
    if(gcsm_Ready == mStateFlag)
    {   // we are still in 'Ready' state
        mStateFlag = gcsm_Processing;   // Be sure we do not repeat
        EVENT_GenComp.ProcessingBegin.notify(SC_ZERO_TIME); // Put events in order
                DEBUG_SC_EVENT_LOCAL("SEND EVENT_GenComp.ProcessingBegin");
        EVENT_GenComp.InputReceived.notify(1,SC_PS); // Re-issue InputReceived
                DEBUG_SC_EVENT_LOCAL("re-SEND EVENT_GenComp.InputReceived");
    }
    else
    {
        scGenComp_PU_Abstract::InputReceived_Do();
    }
 }

/*
 * This virtual method makes ProcessingBegin activity
 * */
void scGenComp_PU_Bio::
    ProcessingBegin_Do()
{
    scGenComp_PU_Abstract::ProcessingBegin_Do();  // Make default processing
    // Technically, just make sure that Heartbeat comes last:
    // 1st: ProcessingBegin
    // 2nd: Input
    // 3rd: Heartbeat
    EVENT_GenComp.Heartbeat.notify(2,SC_PS);
                DEBUG_SC_EVENT_LOCAL("SENT 'EVENT_GenComp.HeartBeat' with zero");
}


void scGenComp_PU_Bio::
    ProcessingEnd_Do()
{
    DEBUG_SC_EVENT_LOCAL("Processing finished");
    MachineState->Deliver(this);    // Pass to "Delivering
}

bool scGenComp_PU_Bio::
    Processing_Finished(void)
{
    return scLocalTime_Get() >= sc_core::sc_time(500,SC_US);

}

void scGenComp_PU_Bio::
    SolvePDE()
{
    int i=1;
}


/* Izkievitch parameters
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
/*
void scGenComp_PU_Bio::
    Relax()
{   // The state machine ensures that we are in phases either 'Delivering' or 'Relaxing'
     if(gcsm_Processing == StateFlag_Get())
    {   // We are at the beginning of the 'Delivering' phase
        StateFlag_Set(gcsm_Relaxing);
    }
    else
    {   // We are at the end of the 'Delivering' phase
        EVENT_GenComp.DeliveringEnd.notify(SC_ZERO_TIME);
        DEBUG_SC_EVENT_LOCAL("SENT    EVENT_GenComp.End_Delivering");
        MachineState->Initialize(this);
        StateFlag_Set(gcsm_Ready);
    }
}
*/



#include <gtest/gtest.h>
#include "scGenComp_PU_Abstract.h"

#define SUPPRESS_LOGGING    // Suppress log messages
//#define DEBUGGING       // Uncomment to debug this unit
#include "DebugMacros.h"
#undef DEBUGGING
#undef SUPPRESS_LOGGING

//#include <sstream>

/** @class	GenCompTest
 * @brief	Tests the operation of the objects for generalized computing
 * 
 */
extern bool UNIT_TESTING;		// Switched off by default

#include "scGenComp_PU_Abstract.h"
#include "scGenComp_PU_Bio.h"
#include "scGenComp_PU_Tech.h"

/*scGenComp_PU_Abstract PU("myAbs");
scGenComp_PU_Tech TPU("myTech",2);*/
scGenComp_PU_Bio BPU("myBio",HEARTBEAT_TIME_DEFAULT_BIO);

// A new test class  of these is created for each test
class GenCompTest : public testing::Test
{
public:
    virtual void SetUp()
    {
        DEBUG_PRINT("GenCompTest started");
        UNIT_TESTING = false;
     }

    virtual void TearDown()
    {
        DEBUG_PRINT("GenCompTest terminated");
    }
};

/**
 * Tests the abstract processing unit
 */
#if 0
TEST_F(GenCompTest, AbstractPU)
{
    scGenComp_PU_Abstract* MyPU = &PU;
    EXPECT_EQ( gcsm_Ready,MyPU->StateFlag_Get());  // The unit is initialized to 'Ready' state

    // Attempt to pass to 'Processing'
    sc_core::sc_time sct_base = sc_time_stamp();
    MyPU->EVENT_GenComp.InputReceived.notify(10,SC_US);
    wait(MyPU->EVENT_GenComp.InputReceived);
    // Now we should be at 10 us simulated time
    EXPECT_EQ(sct_base + sc_core::sc_time(10,SC_US),sc_core::sc_time_stamp());
    MyPU->EVENT_GenComp.InputReceived.notify(10,SC_US);
//    MyPU->EVENT_GenComp.Heartbeat.notify(SC_ZERO_TIME);
//    EXPECT_DEATH(wait(MyPU->EVENT_GenComp.Heartbeat),"");    //Attempt to send the unit to sleep again
 //   EXPECT_EQ(gcsm_Ready, PU.StateFlag_Get());  // Remains ready, given that AbstractPU.Process failed
}
/*  Not yet implemented
 *   PU.EVENT_GenComp.Sleep.notify(SC_ZERO_TIME);
    wait(PU.EVENT_GenComp.Sleep);
    //Send the unit to sleep in 'Ready' state
    EXPECT_EQ(gcsm_Sleeping,MyPU->StateFlag_Get()); // Now it is sleeping
    EXPECT_DEATH(MyPU->Sleep_method(),"");    //Attempt to send the unit to sleep again
*/
#endif

/**
 * Tests the generic biological processing unit
 */
TEST_F(GenCompTest, BioPU)
{
    scGenComp_PU_Bio* MyBPU = &BPU;
    EXPECT_EQ( gcsm_Ready, MyBPU->StateFlag_Get());  // The unit is initialized to 'Ready' state
    // Now the 1st spike arrives
    BPU.EVENT_GenComp.InputReceived.notify(SC_ZERO_TIME);     // The BPU starts to receive spikes
    wait(BPU.EVENT_GenComp.ProcessingBegin);
    EXPECT_EQ( gcsm_Processing, MyBPU->StateFlag_Get());  // The unit goes to 'Processing' state
    wait(BPU.EVENT_GenComp.Heartbeat);
    BPU.EVENT_GenComp.InputReceived.notify(SC_ZERO_TIME);     // The BPU starts to receive spikes
    wait(BPU.EVENT_GenComp.ProcessingEnd);
    EXPECT_EQ( gcsm_Delivering, MyBPU->StateFlag_Get());  // The unit goes to 'Delivering' state
    wait(BPU.EVENT_GenComp.DeliveringEnd);
    EXPECT_EQ( gcsm_Relaxing, MyBPU->StateFlag_Get());  // The unit goes to 'Delivering' state

}

/**
 * Tests the generic technical processing unit
*/
#if 0
TEST_F(GenCompTest, TechPU)
{
    scGenComp_PU_Tech* MyTPU = &TPU;
     EXPECT_EQ( gcsm_Ready, MyTPU->StateFlag_Get());  // The unit is initialized to 'Ready' state
//   TPU.Process_method();                         // The TPU starts to 'process'
//     EXPECT_EQ( gcsm_Processing, MyTPU->StateFlag_Get());  // The unit is goes to 'Processing' state
}
#endif

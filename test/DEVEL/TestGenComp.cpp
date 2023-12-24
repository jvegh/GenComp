#include <gtest/gtest.h>
#include "scAbstractGenComp_PU.h"

#define SUPPRESS_LOGGING    // Suppress log messages
//#define DEBUGGING       // Uncomment to debug this unit
#include "DebugMacros.h"
#undef DEBUGGING
#undef SUPPRESS_LOGGING

#include <sstream>

/** @class	GenCompTest
 * @brief	Tests the operation of the objects for generalized computing
 * 
 */
extern bool UNIT_TESTING;		// Switched off by default

#include "scAbstractGenComp_PU.h"
#include "scBioGenComp_PU.h"
#include "scTechGenComp_PU.h"

//scAbstractGenComp_PU PU("myAbstrct");
//scTechGenComp_PU TPU("myTech",2);
scBioGenComp_PU BPU("myBio");

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

#if 0
/**
 * Tests the abstract processing unit
 */
TEST_F(GenCompTest, AbstractPU)
{
    scAbstractGenComp_PU* MyPU = &PU;
    EXPECT_EQ( gcsm_Ready,MyPU->StateFlag_Get());  // The unit is initialized to 'Ready' state

    // Attempt to pass to 'Processing'
    MyPU->EVENT_GenComp.InputReceived.notify(10,SC_US);
    wait(MyPU->EVENT_GenComp.InputReceived);
 /*   PU.EVENT_GenComp.Sleep.notify(SC_ZERO_TIME);
    wait(PU.EVENT_GenComp.Sleep);
    //Send the unit to sleep in 'Ready' state
    EXPECT_EQ(gcsm_Sleeping,MyPU->StateFlag_Get()); // Now it is sleeping
    EXPECT_DEATH(MyPU->Sleep_method(),"");    //Attempt to send the unit to sleep again
*/
 //   EXPECT_EQ(gcsm_Ready, PU.StateFlag_Get());  // Remains ready, given that AbstractPU.Process failed
}
#endif // 0
/**
 * Tests the generic biological processing unit
 */
TEST_F(GenCompTest, BioPU)
{
    scBioGenComp_PU* MyBPU = &BPU;
    EXPECT_EQ( gcsm_Ready, MyBPU->StateFlag_Get());  // The unit is initialized to 'Ready' state
    // Now the 1st spike arrives
    BPU.EVENT_GenComp.InputReceived.notify(SC_ZERO_TIME);     // The BPU starts to receive spikes
    wait(BPU.EVENT_GenComp.Begin_Computing);
    EXPECT_EQ( gcsm_Processing, MyBPU->StateFlag_Get());  // The unit goes to 'Processing' state
    wait(BPU.EVENT_GenComp.HeartBeat);
    wait(BPU.EVENT_GenComp.HeartBeat);
    BPU.EVENT_GenComp.InputReceived.notify(SC_ZERO_TIME);     // The BPU starts to receive spikes
    wait(BPU.EVENT_GenComp.HeartBeat);
    wait(BPU.EVENT_GenComp.HeartBeat);
    wait(BPU.EVENT_GenComp.End_Computing);
    EXPECT_EQ( gcsm_Delivering, MyBPU->StateFlag_Get());  // The unit goes to 'Delivering' state

}

/**
 * Tests the generic technical processing unit

TEST_F(GenCompTest, TechPU)
{
    scTechGenComp_PU* MyTPU = &TPU;
     EXPECT_EQ( gcsm_Ready, MyTPU->StateFlag_Get());  // The unit is initialized to 'Ready' state
//   TPU.Process_method();                         // The TPU starts to 'process'
//     EXPECT_EQ( gcsm_Processing, MyTPU->StateFlag_Get());  // The unit is goes to 'Processing' state
}
*/

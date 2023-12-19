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

scAbstractGenComp_PU PU("my");
scTechGenComp_PU TPU("my1",2);
scBioGenComp_PU BPU("my2");

// A new test class  of these is created for each test
class GenCompTest : public testing::Test
{
public:
    virtual void SetUp()
    {
        DEBUG_PRINT("GenCompTest started");
     }

    virtual void TearDown()
    {
        DEBUG_PRINT("GenCompTest terminated");
    }
};
 
/**
 * Tests the abstract processing unit
 */
TEST_F(GenCompTest, AbstractPU)
{
    scAbstractGenComp_PU* MyPU = &PU;
    EXPECT_EQ( gcsm_Ready,MyPU->StateFlag_Get());  // The unit is initialized to 'Ready' state
    PU.EVENT_GenComp.Sleep.notify(SC_ZERO_TIME);
    wait(PU.EVENT_GenComp.Sleep);
    //Send the unit to sleep in 'Ready' state
    EXPECT_EQ(gcsm_Sleeping,MyPU->StateFlag_Get()); // Now it is sleeping
    EXPECT_DEATH(MyPU->Sleep_method(),"");    //Send the unit to sleep again
 //   EXPECT_EQ(gcsm_Ready, PU.StateFlag_Get());  // Remains ready, given that AbstractPU.Process failed
}

TEST_F(GenCompTest, BioPU)
{
    scBioGenComp_PU* MyBPU = &BPU;
     EXPECT_EQ( gcsm_Ready, MyBPU->StateFlag_Get());  // The unit is initialized to 'Ready' state
 //  BPU.Process_method();                        // The BPU starts to 'process'
     EXPECT_EQ( gcsm_Processing, MyBPU->StateFlag_Get());  // The unit goes to 'Processing' state

}

TEST_F(GenCompTest, TechPU)
{
    scTechGenComp_PU* MyTPU = &TPU;
     EXPECT_EQ( gcsm_Ready, MyTPU->StateFlag_Get());  // The unit is initialized to 'Ready' state
//   TPU.Process_method();                         // The TPU starts to 'process'
     EXPECT_EQ( gcsm_Processing, MyTPU->StateFlag_Get());  // The unit is goes to 'Processing' state
}

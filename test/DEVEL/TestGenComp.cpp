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
scTechGenComp_PU TPU("my",2);
scBioGenComp_PU BPU("my");

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
    EXPECT_EQ( gcsm_Ready,PU.StateFlag_Get());  // The unit is initialized to 'Ready' state
    EXPECT_DEATH(PU.Process_method(),"");
    EXPECT_EQ(gcsm_Ready, PU.StateFlag_Get());  // Remains ready, given that AbstractPU.Process failed
}

TEST_F(GenCompTest, BioPU)
{
    scBioGenComp_PU BPU("MyB");
    EXPECT_EQ( gcsm_Ready, BPU.StateFlag_Get());  // The unit is initialized to 'Ready' state
    BPU.Process();                        // The BPU starts to 'process'
    EXPECT_EQ( gcsm_Processing, BPU.StateFlag_Get());  // The unit goes to 'Processing' state

}

TEST_F(GenCompTest, TechPU)
{
   scTechGenComp_PU TPU("MyT",2);
     EXPECT_EQ( gcsm_Ready, TPU.StateFlag_Get());  // The unit is initialized to 'Ready' state
    TPU.Process();                         // The TPU starts to 'process'
     EXPECT_EQ( gcsm_Processing, TPU.StateFlag_Get());  // The unit is goes to 'Processing' state
}

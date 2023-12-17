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
    AbstractGenComp_PU PU;
    EXPECT_EQ( gcsm_Ready, PU.State_Get()->Flag_Get());  // The unit is initialized to 'Ready' state
        EXPECT_DEATH(PU.State_Get()->Process(PU),"");
    EXPECT_EQ(gcsm_Ready, PU.State_Get()->Flag_Get());  // Remains ready, given that AbstractPU.Process failed
}

TEST_F(GenCompTest, BioPU)
{
    BioGenComp_PU BPU;
    EXPECT_EQ( gcsm_Ready, BPU.State_Get()->Flag_Get());  // The unit is initialized to 'Ready' state
    BPU.State_Get()->Process(BPU);                        // The BPU starts to 'process'
    EXPECT_EQ( gcsm_Processing, BPU.State_Get()->Flag_Get());  // The unit goes to 'Processing' state
}

TEST_F(GenCompTest, TechPU)
{
   TechGenComp_PU TPU(2);
     EXPECT_EQ( gcsm_Ready, TPU.State_Get()->Flag_Get());  // The unit is initialized to 'Ready' state
    TPU.State_Get()->Process(TPU);                         // The TPU starts to 'process'
     EXPECT_EQ( gcsm_Processing, TPU.State_Get()->Flag_Get());  // The unit is goes to 'Processing' state
}

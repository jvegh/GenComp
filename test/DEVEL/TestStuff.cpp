#include <gtest/gtest.h>
#include "Stuff.h"

#define SUPPRESS_LOGGING    // Suppress log messages
//#define DEBUGGING       // Uncomment to debug this unit
#include "DebugMacros.h"
#undef DEBUGGING
#undef SUPPRESS_LOGGING

#include "Utils.h"
#include <sstream>

/** @class	StuffTest
 * @brief	Tests the operation of the support stuff
 * 
 */
//#define DEBUG_DISABLED

extern bool OBJECT_FILE_PRINTED;
// A new test class  of these is created for each test
class StuffTest : public testing::Test
{
public:
    virtual void SetUp()
    {
        DEBUG_PRINT("StuffTest started");
     }

    virtual void TearDown()
    {
        DEBUG_PRINT("StuffTest terminated");
    }
};
 
/**
 * Tests some routines of the stuff
 */
TEST_F(StuffTest, Tools)
{
    // An illegal mask (no 'hot' bit results in ID '-1)
    EXPECT_EQ((unsigned int) -1 , MaskToID(0) );
    // An 'all ones', 32-bit value results in the highest ID possible
    EXPECT_EQ((unsigned int) 31 , MaskToID((unsigned int)-1) );

    // ID 0 results in mask 0x0001
    EXPECT_EQ(1 , IDtoMask(0) );
    // ID 5 results in mask 2**5 = 0x0020
    EXPECT_EQ(32 , IDtoMask(5) );
    // ID 35 is illegal ID, results in mask 0
    EXPECT_EQ(0 , IDtoMask(MAX_GRIDPOINTS) );
    string MySimpleFileName = "asum.Eyo";
    string MyRelativeFileName = "~MyPath/../files/asum.Eyo";
    string ExpectedName = GetAppName() + "_txt" + "_asum";
    size_t found = ExpectedName.find(" "); // Replace " " with "-" in file name
    ExpectedName.replace(found,1,"-");
    EXPECT_EQ(ExpectedName, GetFileNameRoot(MyRelativeFileName));
    EXPECT_EQ(ExpectedName, GetFileNameRoot(MySimpleFileName));
}

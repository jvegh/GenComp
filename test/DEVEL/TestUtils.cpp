#include <gtest/gtest.h>

#define SUPPRESS_LOGGING    // Suppress log messages
//#define DEBUGGING       // Uncomment to debug this unit
#include "Macros.h"
#undef DEBUGGING
#undef SUPPRESS_LOGGING

#include "Utils.h"

//extern bool UNIT_TESTING;		// Switched off by default

// A new test class  of these is created for each test
class UtilsTest : public testing::Test
{
public:

   virtual void SetUp()
   {
 //       DEBUG_PRINT("UtilsTest started");
   }

   virtual void TearDown()
   {
//        DEBUG_PRINT("UtilsTest terminated");
   }
};
 


/**
 * Tests some routines of the stuff
 */
TEST_F(UtilsTest, Tools)
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

    EXPECT_EQ('2', AssembleQTIDSubcode(0x2));

    EXPECT_EQ("2",IDtoString(2,4));
    EXPECT_EQ("03",IDtoString(3));
    EXPECT_EQ("004",IDtoString(4,12));
}

TEST_F(UtilsTest, CountBits)
{

    EXPECT_EQ(0, MaskOfLength(0));
    EXPECT_EQ(0x3, MaskOfLength(2));
    EXPECT_EQ(0x7fffffff, MaskOfLength(31));
    EXPECT_EQ(0xffffffff, MaskOfLength(32));
    // Check if mask bits are counter correctly
    EXPECT_EQ(2, OnesInMask_Get(0x6,5));
    EXPECT_EQ(0, OnesInMask_Get(0x0,5)); // Nothing masked out
    EXPECT_EQ(0, OnesInMask_Get(0x1F,0)); // Nothing to mask
    EXPECT_EQ(5, OnesInMask_Get(0x1F,5)); // Everyhing masked out
    EXPECT_EQ(5, OnesInMask_Get(0xFF,5));  // The mask is too long, but it is neglected
    EXPECT_EQ(32, OnesInMask_Get(0xFFFFFFFF,32));  // Everything cleared
    EXPECT_EQ(0, OnesInMask_Get(0x0,32));  // Nothing cleared
    EXPECT_EQ(1, OnesInMask_Get(0x1000,32));  // One bit cleared
    EXPECT_EQ(31, OnesInMask_Get(0xFFEFFFFF,32));  // Everything but one cleared
}

TEST_F(UtilsTest, BitPosition)
{
    EXPECT_EQ(0, PositionOfFirstOne_Get(0x3,5));
    EXPECT_EQ(4, PositionOfFirstOne_Get(0x10,5));
    EXPECT_EQ(-1, PositionOfFirstOne_Get(0x3,0));
    EXPECT_EQ(-1, PositionOfFirstOne_Get(0x3,MAX_GRIDPOINTS));
    EXPECT_EQ(-1, PositionOfFirstOne_Get(0x0,5));
    EXPECT_EQ(0, PositionOfFirstOne_Get(0xff,5));
    EXPECT_EQ(1, PositionOfFirstOne_Get(0xfe,5));

    EXPECT_EQ(2, PositionOfFirstZero_Get(0x3,5));
    EXPECT_EQ(0, PositionOfFirstZero_Get(0x10,5));
    EXPECT_EQ(0, PositionOfFirstZero_Get(0x1e,5));
    EXPECT_EQ(-1, PositionOfFirstZero_Get(0x1f,5));
    EXPECT_EQ(-1, PositionOfFirstZero_Get(0x3,0));
    EXPECT_EQ(-1, PositionOfFirstZero_Get(0x3,50));
}

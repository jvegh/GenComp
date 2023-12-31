/*! @file Utils.h
 *  @ingroup GENCOMP_MODULE_STUFF
 *  @brief Utility function prototypes for the cooperating electronic modules.
 *
 *  This contains the prototypes for some useful stuff for the simulator, utilities.
 */
 /*  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef EMPA_UTILS_H
#define EMPA_UTILS_H
#include <iomanip>      // std::setfill, std::setw
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <string>
#include "HWConfig.h"


using namespace std;

    int
MaskToID(SC_GRIDPOINT_MASK_TYPE Mask); ///< Converts core mask to its sequence number
/**
 *  @brief Converts a core ID to the corresponding mask.
 *  It is assumed that ID is in the range 0 to the number of bits in integer
 *  If Mask > sizeof(int), Mask = 0
 */
SC_GRIDPOINT_MASK_TYPE IDtoMask(int ID);

  SC_GRIDPOINT_MASK_TYPE
MaskOfLength(int Length =GRIDPOINT_MASK_WIDTH); ///< Produces a mask of length Length

// Count ones in 'Mask', limited to 'Length' bits
// Return -1 if out of range or not found
    int
OnesInMask_Get(SC_GRIDPOINT_MASK_TYPE ClearMask=-1, int Length =GRIDPOINT_MASK_WIDTH);

// Get position of the first 1 (from right) in word Mask
    int
PositionOfFirstOne_Get(SC_GRIDPOINT_MASK_TYPE Mask=-1, int Length =GRIDPOINT_MASK_WIDTH);

    int
PositionOfFirstZero_Get(SC_GRIDPOINT_MASK_TYPE Mask=-1, int Length =GRIDPOINT_MASK_WIDTH);

// Return a character corresponding to the 1s in the children mask
    char
AssembleQTIDSubcode(SC_GRIDPOINT_MASK_TYPE ChildMask);

    string
IDtoString(int ID, int BitWidth=8);

// Return positive modulo even for negative x
int moduloN(int x,int N);

    int
YFromPosition_Get(int x, int y);


    string
sc_time_to_nsec_Get(sc_core::sc_time T = sc_core::sc_time_stamp(), const int d = 2, const int w=6);

     string
sc_time_to_usec_Get(sc_core::sc_time T = sc_core::sc_time_stamp(), const int d = 0, const int w=6);

     string  /// Return the string describing the time
StringOfTime_Get(void);

int sb_fprintf(FILE *fp, const char *fmt, ...) ;

#endif // EMPA_UTILS_H

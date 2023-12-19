/** @file scTechGenComp_PU.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scTechGenComp_PU.h"
// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
//#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing

static TechGenCompState TheTechGenCompState = TechGenCompState();
// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing


    scTechGenComp_PU::
scTechGenComp_PU(sc_core::sc_module_name nm, int32_t No):
    scAbstractGenComp_PU(nm),
    mNoOfArgs(No)
{
}

scTechGenComp_PU::
    ~scTechGenComp_PU(void)
{
}

void scTechGenComp_PU::
    Process_method(void)
{
}

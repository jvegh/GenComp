/** @file scBioGenComp_PU.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "scBioGenComp_PU.h"
// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
//#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file

#include "DebugMacros.h"

extern bool UNIT_TESTING;	// Whether in course of unit testing

static BioGenCompState TheBioGenCompState = BioGenCompState();
// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing


    scBioGenComp_PU::
scBioGenComp_PU(sc_core::sc_module_name nm):
    scAbstractGenComp_PU(nm)
{
}


    scBioGenComp_PU::
~scBioGenComp_PU(void)
{
}

    void scBioGenComp_PU::
Process_method(void)
{
 //   int32_t i = State_Get()->Flag_Get();
}


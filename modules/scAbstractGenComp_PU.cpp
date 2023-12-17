/** @file scAbstractGenComp_PU.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/
// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
//#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

#include "scAbstractGenComp_PU.h"


extern bool UNIT_TESTING;	// Whether in course of unit testing

// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing

    AbstractGenComp_PU::
AbstractGenComp_PU(void)
{
    state = new ReadyGenCompState();
}

    AbstractGenComp_PU::
~AbstractGenComp_PU(void)
{
}

    BioGenComp_PU::
BioGenComp_PU(void):
    AbstractGenComp_PU()
{
}


    BioGenComp_PU::
~BioGenComp_PU(void)
{
}

    void BioGenComp_PU::
Process(void)
{
    int32_t i = State_Get()->Flag_Get();
}

    TechGenComp_PU::
    TechGenComp_PU(int32_t No):
    AbstractGenComp_PU(),
    mNoOfArgs(No)
{
}

TechGenComp_PU::
    ~TechGenComp_PU(void)
{
}

void TechGenComp_PU::
    Process(void)
{
    int32_t i = State_Get()->Flag_Get();
}

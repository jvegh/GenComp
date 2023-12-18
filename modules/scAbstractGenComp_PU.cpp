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

static AbstractGenCompState TheAbstractGenCompState = AbstractGenCompState();
// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing

    scAbstractGenComp_PU::
scAbstractGenComp_PU(sc_core::sc_module_name nm): sc_core::sc_module( nm)
    ,MachineState (&TheAbstractGenCompState)
    ,StateFlag(gcsm_Ready)
{
 /*   SC_METHOD(Initialize_method);
    sensitive << EVENT_GenComp.Initialize;*/
    // dont_initialize(); Execute the processn
}
scAbstractGenComp_PU::
~scAbstractGenComp_PU(void)
{
}

// Puts the PU in its default state
void scAbstractGenComp_PU::
    Initialize_method(void)
{
    MachineState->Initialize();   // Change status to 'Initial'
    Initialize(); // Initialize the unit, mainly HW

    // Put PU in its default state
}

void scAbstractGenComp_PU::
    Initialize(void)
{
}

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
Process(void)
{
 //   int32_t i = State_Get()->Flag_Get();
}

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
    Process(void)
{
 //   int32_t i = State_Get()->Flag_Get();
}

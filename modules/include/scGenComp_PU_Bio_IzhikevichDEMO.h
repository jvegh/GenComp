/** @file scGenComp_PU_Bio_IzhikevichDEMO.h
 *  @ingroup GENCOMP_MODULE_DEMO

 *  @brief A demo unit to demonstrate the operation of an Izhikevich-type biological processing unit
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef SCBIOIZHIKEVICHGENCOMPDEMO_H
#define SCBIOIZHIKEVICHGENCOMPDEMO_H
/** @addtogroup GENCOMP_MODULE_DEMO
 *  @{
 */

//#include "GenCompStates_Bio.h"
#include "scGenComp_PU_Bio_Izhikevich.h"

#define BIO_DEMO_HEARTBEAT_TIME sc_core::sc_time(100,SC_US)
#define BIO_DEMO_INITIAL_DELAY sc_core::sc_time(10,SC_MS)

/*
 * \class scGenComp_PU_Bio_IzhikevichDemo
 * \brief  A simple demo class to implement the operation of a general bio computing unit
 *
 * Operating principle of event processing:
 * The unit cycles through the statios of processing
 *
 */

/*!
 * \class scGenComp_PU_Bio_IzhikevichDEMO
 * \brief  Implements a demo of general biological-type computing PU
 *
 */
class scGenComp_PU_Bio_IzhikevichDEMO : public scGenComp_PU_Bio_Izhikevich
{
  public:
    /*!
     * \brief Creates a demo Izhikevich-type biological general computing unit
     * @param[in] nm the SystemC name of the module
     * @param[in] Heartbeat the integration time lap
      *
     * Creates a demo abstract biological computing unit.
     * A template for developing and testing your own units
     */
      scGenComp_PU_Bio_IzhikevichDEMO(sc_core::sc_module_name nm   // Module name
                           , sc_core::sc_time Heartbeat = BIO_DEMO_HEARTBEAT_TIME);  // Heartbeat time
     ~scGenComp_PU_Bio_IzhikevichDEMO(void){}

    /**
     * @brief Handle heartbeats in 'Processing' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential at hillock reaches its threshold value
     * -  Starts to receive heartbeats after EVENT_GenComp.DeliveringBegin arrives
     * -  Stops generating heartbeats after EVENT_GenComp.DeliveringEnd arrives
     *     then issues EVENT_GenComp.RelaxingBegin
     * -  Heartbeat frequency is set to BIO_HEARTBEAT_TIME (10 us)
     */
 //     virtual void Heartbeat_Processing_Do();

    /**
      * Prepare events for the demo unit; run before the other 'method's
      *
      * See the operation in scGenComp_PU_BioDEMO.cpp, InitializeForDemo_method()
      */
    void InitializeForDemo_method();

    /** @brief MembraneThresholdExceeded
     * @return true if memmrane threshold exceeded
     *  if ihis case, returns true at 31 us local time
     */
#if 0
    bool MembraneThresholdExceeded_Processing(void);

    /**
     * @brief Heartbeat_Recalculate_Membrane_Potential
     *
     *  The state of the biological computing is re-calculated (as the simulation time passes)
     *  (solve the differential equation at this time)
     *
     *
     */
    void HeartbeatRecalculateMembranePotential();

    /**
     * @brief Processing_Finished
     * @return true if processing finished and 'Delivering' follows
     *
     *
     */
    virtual bool Processing_Finished(void);
#endif
protected:
    sc_core::sc_event Demo_Event;
};
/** @}*/

#endif // SCBIOIZHIKEVICHGENCOMPDEMO_H

/** @file scGenComp_PU_BioDEMO.h
 *  @ingroup GENCOMP_MODULE_DEMO

 *  @brief A demo unit to demonstrate the operation of biological processing unit
 *  It is just event handling, timed actions, no real activity
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef SCBIOGENCOMPDEMO_H
#define SCBIOGENCOMPDEMO_H
/** @addtogroup GENCOMP_MODULE_DEMO
 *  @{
 */

#include "GenCompStates_Bio.h"
#include "scGenComp_PU_Bio.h"

#define BIO_HEARTBEAT_TIME sc_core::sc_time(10,SC_US)

/*
 * \class scGenComp_PU_BioDemo
 * \brief  A simple demo class to implement the operation of a general bio computing unit
 *
 * Operating principle of event processing:
 * The unit cycles through the statios of processing
 *
 */

/*!
 * \class scGenComp_PU_BioDEMO
 * \brief  Implements a demo of general biological-type computing PU
 *
 */
class scGenComp_PU_BioDEMO : public scGenComp_PU_Bio
{
  public:
    /*!
     * \brief Creates a demo biological general computing unit
     * @param nm the SystemC name of the module
      *
     * Creates a demo abstract biological computing unit
     */
      scGenComp_PU_BioDEMO(sc_core::sc_module_name nm);

      /**
     * @brief Handle heartbeats in 'Delivering' mode
     *
     * -  Re-issues EVENT_GenComp.Heartbeat until membrane potential at hillock reaches its threshold value
     * -  Starts to receive heartbeats after EVENT_GenComp.DeliveringBegin arrives
     * -  Stops generating heartbeats after EVENT_GenComp.DeliveringEnd arrives
     *     then issues EVENT_GenComp.RelaxingBegin
     * -  Heartbeat frequency is set to BIO_HEARTBEAT_TIME (10 us)
     */
      void Heartbeat_Processing();

    /**
      *  Prepare events for the demo unit; run before the other 'method's
      * The unit is 'Ready', expected to 'live' at 120 us
      * Until 125 us receives heartbeats, in 'Processing' mode
      * Until 130 us receives heartbeats, in 'Delivering' mode
      * Until 135 us receives heartbeats, in 'Relaxing' mode
      * Until 140 us receives heartbeats, in 'Ready' mode
    */
    void InitializeForDemo_method();

    /** @brief MembraneThresholdExceeded
     * @return true if memmrane threshold exceeded
     *  if ihis case, returns true at 31 us local time
     */
    bool MembraneThresholdExceeded_Processing(void);
    /**
     * @brief Heartbeat_Recalculate_Membrane_Potential
     *
     *  The state of the biological computing is re-calculated (as the simulation time passes)
     *  (solve the differential equation at this time)
     */
    void HeartbeatRecalculateMembranePotential();
protected:
    sc_core::sc_event Demo_Event;
};
/** @}*/

#endif // SCBIOGENCOMPDEMO_H

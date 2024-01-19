/** @file scGenComp_PU_AbstractDEMO.h
 *  @ingroup GENCOMP_MODULE_DEMO

 *  @brief A demo unit to demonstrate the operation of an abstract processing unit
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef SCABSTRACTGENCOMPDEMO_H
#define SCABSTRACTGENCOMPDEMO_H
/** @addtogroup GENCOMP_MODULE_DEMO
 *  @{
 */

#include "scGenComp_PU_Abstract.h"

#define BIO_DEMO_HEARTBEAT_TIME sc_core::sc_time(100,SC_US)
#define BIO_DEMO_INITIAL_DELAY sc_core::sc_time(10,SC_MS)

/*
 * \class scGenComp_AbstractDemo
 * \brief  A simple demo class to implement the operation of a general abstract computing unit
 *
 * Operating principle of event processing:
 * The unit cycles through the statios of processing
 */

/*!
 * \class scGenComp_PU_AbstractDEMO
 * \brief  Implements a demo of general abstract computing PU
 */
class scGenComp_PU_AbstractDEMO : public scGenComp_PU_Abstract
{
  public:
    /*!
     * \brief Creates a demo abstract general computing unit
     * @param[in] nm the SystemC name of the module
     * @param[in] Heartbeat the integration time lap
     *
     * Creates a demo abstract general computing unit.
     * A template for developing and testing your own units
     */
      scGenComp_PU_AbstractDEMO(sc_core::sc_module_name nm   // Module name
                           , sc_core::sc_time Heartbeat = BIO_DEMO_HEARTBEAT_TIME);  // Heartbeat time
     ~scGenComp_PU_AbstractDEMO(void){}

    /**
      * Prepare events for the demo unit; run before the other 'method's
      */
    void InitializeForDemo_method();

    virtual void DeliveringBegin_Do();
    virtual void ProcessingBegin_Do();
    virtual void Ready_Do();
    virtual void RelaxingBegin_Do();

protected:
    sc_core::sc_event Demo_Event;
};
/** @}*/

#endif // SCABSTRACTGENCOMPDEMO_H

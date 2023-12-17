/** @file scAbstractGenComp_PU.h
 *  @ingroup GENCOMP_MODULE_PROCESS

 *  @brief Function prototypes for the computing module
 *  It is just event handling, no modules
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef ABSTRACTGENCOMP_H
#define ABSTRACTGENCOMP_H
/** @addtogroup GENCOMP_MODULE_BASIC
 *  @{
 */
//?#include "AbstractEnumTypes.h"
#include "scGenCompStates.h"

using namespace std;


/*!
 * \class AbstractGenComp_PU
 * \brief  A simple abstract class to deal  with states of a general computing unit
 *
 *
 */
class AbstractGenComp_PU
{
    friend class AbstractGenCompState;
  public:
    /*!
     * \brief
     *
     * Creates
     */

    AbstractGenComp_PU(void);
    virtual ~AbstractGenComp_PU(void); // Must be overridden
    virtual void Deliver(){assert(0);}
    virtual void HeartBeat(){assert(0);}
    virtual void Process(){assert(0);}
    virtual void Relax(){assert(0);}
    virtual void Reinitialize(){assert(0);}
    virtual void Synchronize(){assert(0);}
    virtual void Fail(){assert(0);}
    virtual void Sleep(){assert(0);}
    virtual void WakeUp(){assert(0);}
    AbstractGenCompState* State_Get(void){return state;}
  protected:
    AbstractGenCompState* state;

 };// of class AbstractGenComp_PU

/*!
 * \class TechGenComp_PU
 * \brief  A simple math class
 *
 * The technical PUs need all arguments at the beginning
 */
class TechGenComp_PU : public AbstractGenComp_PU
{
  public:
    /*!
     * \brief Implements a general technical-type computing
     *
     *
     */

    TechGenComp_PU(int32_t No);
    virtual ~TechGenComp_PU(); // Must be overridden
    /**
     * @brief Process
     */
    virtual void Process();

  protected:
    int32_t mNoOfArgs;    // The number of args before computation can start
 };// of class TechGenComp_PU

/*!
 * \class BioGenComp_PU
 * \brief  Implements a general biological-type computing
 *
 */
class BioGenComp_PU : public AbstractGenComp_PU
{
  public:
    /*!
     * \brief Creates a biological abstract processing unit
     *
     * Creates an abstract biological computing unit
     */

    BioGenComp_PU(void);
    virtual ~BioGenComp_PU(void); // Must be overridden
/*    virtual void Deliver(){assert(0);}
    virtual void HeartBeat(){assert(0);}*/
    /**
     * @brief Process
     *
     * In biological computing,
     */
    virtual void Process();
/*    virtual void Relax(){assert(0);}
    virtual void Reinitialize(){assert(0);}
    virtual void Synchronize(){assert(0);}
    virtual void Fail(){assert(0);}
*/
  protected:
 };// of class BioGenComp_PU
/** @}*/

#endif // ABSTRACTGENCOMP_H

#ifndef CATTessPointPolyIter_H
#define CATTessPointPolyIter_H

// COPYRIGHT DASSAULT SYSTEMES 2004


#include "TessAPI.h"

//#include "CATMathDef.h"
//#include "CATMathVector.h"
//#include "CATCGMNewArray.h"

//#include "CATIAV5Level.h"

class CATFace;
class CATTessPoint;
/**
 * Class defining the iterator on the points resulting from a Tessellation operator.
 * <br> Such iterator is directly created by the @href CATBodyTessellator#GetFace
 * or @href CATCellTessellator#GetFace methods. Its deletion is managed at the
 * CATBodyTessellator and CATCellTessellator deletion. 
 * Use it according to the following scheme:
 * <ul>
 * <li> Retrieve the number of points 
 * <li> Go the next result
 * <li> Retrieve the result
 * <li> Optionally, reset to begin again.
 * </ul>
 * <p> This class also allows you to directly retrieve the results in an array that 
 * you have to allocate and deallocate after use.
 */
class ExportedByTessAPI CATTessPointPolyIter
{
public:
  /**
   * @nodoc
   * Default constructor.
   */
  CATTessPointPolyIter();

  /** @nodoc */
  CATCGMNewClassArrayDeclare;
  
  /**
   * Returns the Cartesian coordinates of the current point.
   * @return
   * The array of the Cartesian coordinates.
   */
  virtual double const * GetPointXyz() const;

  /**
   * Retrieves a pointer to the normal at the current point.
   * @param oNor
   * The pointer to the normal.
   */
  virtual CATBoolean GetPointNor(SPACATMathVector const *& oNor);

  /**
   * Returns the parameters of the current point (in case of a face or a surface).
   * @return
   * The array of parameters.
   */
  virtual double const * GetPointUv() const;

  /**
   * Returns the CATFace of the current point (in case of a face or a surface).
   * @return
   * The pointer to the CATFace
   */
  CATFace * GetFace();

  /**
   * Skips to the next point.
   */
  virtual void GoToNext();
 
  /**
   * Resets the iterator to the beginning.
   */
  virtual void Reset();

  /**
   * Tests whether the list is completed.
   * @return
   * The result of the test.
   * <br><b>Legal values</b>: <tt>1</tt> if the list is completed, <tt>0</tt> otherwise.
   */
  virtual CATLONG32 IsExhausted() const;

  /**
   * @nodoc
   * Internal Use.
   */
  virtual void Initialize(CATTessPoint * iPoint);

protected:
  /** @nodoc */
  CATTessPoint * _PointFirst;
  CATTessPoint * _PointCurrent;

};

#endif

#ifndef SPA3dPmTetraHpcSizeSpecification_hxx
#define SPA3dPmTetraHpcSizeSpecification_hxx

#include "SPA3dPmACIS.h"

/**
* @file SPA3dPmTetraHpcSizeSpecification.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup ACIS3DPM
*
* @{
*/

namespace Spa3dpm
{

struct TetraHpcSizeSpecification_Impl;

/**
* The <tt>TetraHpcSizeSpecification</tt> class represents size-specification data for a tetra-hpc (cvm) operator.
* <tt>TetraHpcSizeSpecification</tt> objects are light weight objects containing only a pointer to the implementation
* object. Copying a <tt>TetraHpcSizeSpecification</tt> object does not duplicate the underlying size-specification implementation, it only increases
* the number of light objects referencing the actual size-specification. The default constructor <tt>TetraHpcSizeSpecification()</tt> creates
* an object that points to no size-specification implementation, and thus it is considered an uninitialized object.
* In case of an error, the class methods throw exceptions.
*/
class DECL_SPA3dpm TetraHpcSizeSpecification
{
public:

	/**
	* Default constructor.
	* Creates an uninitialized object. It refers to no actual size specification. Use the assignment operator or the API function @href <tt></tt> to make the object refer to some existing data.
	*/
	TetraHpcSizeSpecification();

	/**
	* Copy constructor. The new object will reference the same size specification as the original object.
	*/
	TetraHpcSizeSpecification(const TetraHpcSizeSpecification &other);

	/**
	* Assignment operator. The "this" object will reference the same size specification as the "other" object.
	*/
	const TetraHpcSizeSpecification &operator=(const TetraHpcSizeSpecification &other);

	/**
	* Destructor. It only deletes the underlying data structure if this object was the last reference to it.
	*/
	~TetraHpcSizeSpecification();

	/**
	* Returns true if the object refers to actual size specification.
	*/
	bool Initialized() const;


	/**
	* The <tt>BoundaryLayerDistribution</tt> class is an abstract base class that can be used to provide a callback function
	* for a boundary layer distribution constraint.
	*/
	class DECL_SPA3dpm BoundaryLayerDistribution
	{
	public:
		/**
		* Override this function to provide a callback function for a boundary layer distribution constraint.
		*/
		virtual double GetSizeAtLayer(int layer) = 0;

		/**
		* Default constructor.
		*/
		BoundaryLayerDistribution();

		/**
		* Destructor.
		*/
		virtual ~BoundaryLayerDistribution();

		void *impl;
	};

	/**
	* Add a boundary layer null constraint on a face.
	*/
	void AddBoundaryLayerNullConstraintOnFace(int face_type, int face_idx, int face_orientation);

	/**
	* Add a boundary layer arithmetic constraint on a face.
	*/
	void AddBoundaryLayerArithmeticConstraintOnFace(double h0, double difference, double blending, int n_min, int n_max, int relative, int face_type, int face_idx, int face_orientation);

	/**
	* Add a boundary layer geometric constraint on a face.
	*/
	void AddBoundaryLayerGeometricConstraintOnFace(double h0, double progression, double blending, int n_min, int n_max, int relative, int face_type, int face_idx, int face_orientation);

	/**
	* Add a boundary layer uniform constraint on a face.
	*/
	void AddBoundaryLayerUniformConstraintOnFace(double h, int n, int relative, int face_type, int face_idx, int face_orientation);

	/**
	* Add a boundary layer distribution constraint on a face.
	*/
	void AddBoundaryLayerDistributionConstraintOnFace(int n_min, int n_max, BoundaryLayerDistribution &distribution, int relative, int face_type, int face_idx, int face_orientation);

	/**
	* Add a boundary layer null constraint to a face tag.
	*/
	void AddBoundaryLayerNullConstraintOnFaceTag(int tag);

	/**
	* Add a boundary layer arithmetic constraint to a face tag.
	*/
	void AddBoundaryLayerArithmeticConstraintOnFaceTag(double h0, double difference, double blending, int n_min, int n_max, int relative, int tag);

	/**
	* Add a boundary layer geometric constraint to a face tag.
	*/
	void AddBoundaryLayerGeometricConstraintOnFaceTag(double h0, double progression, double blending, int n_min, int n_max, int relative, int tag);

	/**
	* Add a boundary layer uniform constraint to a face tag.
	*/
	void AddBoundaryLayerUniformConstraintOnFaceTag(double h, int n, int relative, int tag);

	/**
	* Add a boundary layer distribution constraint to a face tag.
	*/
	void AddBoundaryLayerDistributionConstraintOnFaceTag(int n_min, int n_max, BoundaryLayerDistribution &distribution, int relative, int tag);

	/**
	* Add a boundary layer null constraint on a subdomain.
	*/
	void AddBoundaryLayerNullConstraintOnSubdomain(int seed_type, int seed_idx, int seed_orientation);

	/**
	* Add a boundary layer arithmetic constraint on a subdomain.
	*/
	void AddBoundaryLayerArithmeticConstraintOnSubdomain(double h0, double difference, double blending, int n_min, int n_max, int relative, int seed_type, int seed_idx, int seed_orientation);

	/**
	* Add a boundary layer geometric constraint on a subdomain.
	*/
	void AddBoundaryLayerGeometricConstraintOnSubdomain(double h0, double progression, double blending, int n_min, int n_max, int relative, int seed_type, int seed_idx, int seed_orientation);

	/**
	* Add a boundary layer uniform constraint on a subdomain.
	*/
	void AddBoundaryLayerUniformConstraintOnSubdomain(double h, int n, int relative, int seed_type, int seed_idx, int seed_orientation);

	/**
	* Add a boundary layer distribution constraint on a subdomain.
	*/
	void AddBoundaryLayerDistributionConstraintOnSubdomain(int n_min, int n_max, BoundaryLayerDistribution &distribution, int relative, int seed_type, int seed_idx, int seed_orientation);

	/**
	* Add a boundary layer null constraint to a face tag in a subdomain.
	*/
	void AddBoundaryLayerNullConstraintOnFaceTagInSubdomain(int tag, int seed_type, int seed_idx, int seed_orientation);

	/**
	* Add a boundary layer arithmetic constraint to a face tag in a subdomain.
	*/
	void AddBoundaryLayerArithmeticConstraintOnFaceTagInSubdomain(double h0, double difference, double blending, int n_min, int n_max, int relative, int tag, int seed_type, int seed_idx, int seed_orientation);

	/**
	* Add a boundary layer geometric constraint to a face tag in a subdomain.
	*/
	void AddBoundaryLayerGeometricConstraintOnFaceTagInSubdomain(double h0, double progression, double blending, int n_min, int n_max, int relative, int tag, int seed_type, int seed_idx, int seed_orientation);

	/**
	* Add a boundary layer uniform constraint to a face tag in a subdomain.
	*/
	void AddBoundaryLayerUniformConstraintOnFaceTagInSubdomain(double h, int n, int relative, int tag, int seed_type, int seed_idx, int seed_orientation);

	/**
	* Add a boundary layer distribution constraint to a face tag in a subdomain.
	*/
	void AddBoundaryLayerDistributionConstraintOnFaceTagInSubdomain(int n_min, int n_max, BoundaryLayerDistribution &distribution, int relative, int tag, int seed_type, int seed_idx, int seed_orientation);

private:
	TetraHpcSizeSpecification_Impl *impl;
	friend struct II;
};

}

/** @} */
#endif

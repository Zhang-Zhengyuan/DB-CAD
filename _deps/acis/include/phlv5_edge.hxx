// COPYRIGHT DASSAULT SYSTEMES 1989
// IncludeFile: phlv5_edge.hxx		Edge for Hidden Line Removal
#if !defined(PHLV5_EDGE_CLASS)
#define PHLV5_EDGE_CLASS
#include "dcl_phlv5.h"
#include "logical.h"
#include "curve.hxx"
#include "ent_phlv5.hxx"
#include "phlv5entmc.hxx"
#include "position.hxx"
#include "lists.hxx"
#include "box.hxx"
#include "transfrm.hxx"
#include "phlv5_occ.hxx"
/**
 * @file phlv5_edge.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup PHLV5API
 *  \brief Declared at <phlv5_edge.hxx>
 *  @{
 */
class CURVE;
class FACE;
class BODY;
class PHLV5_SEGMENT;
class ENTITY_LIST;
class phlv5_segment;
class AcisOptions;
/**
 * The version in which store the modeling EDGE
 */
#define PORTMANTEAU( maj, min ) (100 * maj + min)
const int PHLV5_EDGE_STORE_EDGE_MAJOR = 15;
const int PHLV5_EDGE_STORE_EDGE_MINOR = 0;
const int PHLV5_EDGE_STORE_EDGE_VERSION =
	PORTMANTEAU(PHLV5_EDGE_STORE_EDGE_MAJOR,PHLV5_EDGE_STORE_EDGE_MINOR);

const int PHLV5_OCC_STORE_MAJOR = 17;
const int PHLV5_OCC_STORE_MINOR = 0;
const int PHLV5_OCC_STORE_VERSION =
	PORTMANTEAU(PHLV5_OCC_STORE_MAJOR,PHLV5_OCC_STORE_MINOR);
/**
 * Derivation level
 */
#define PHLV5_EDGE_LEVEL ( ENTITY_PHLV5_LEVEL + 1 )

extern DECL_PHLV5 int PHLV5_EDGE_TYPE;

// Don't doc this class
/*
// tbrv
*/
/**
 * @nodoc
 */
class phlv5_edge : public ACIS_OBJECT
{
	public:
		EDGE* MyEdge;
		BODY* MyBody;
        FACE* MyFace;
		phlv5_segment* MySegment;
		float* myPts;
		int myNP;
		double mySParam;
		double myEParam;
		int MySense;
		int MyBodyIndex;
		int MyTransformIndex;

	public:
		phlv5_edge(){};
		~phlv5_edge(){ACIS_DELETE [] STD_CAST myPts;
						//if(MyTransf) 
						//	ACIS_DELETE MyTransf;
						};

		phlv5_edge(EDGE* Edge,
				 BODY* Body,
                 FACE* theFace,
				 phlv5_segment* Seg,
				 float * Point,
				 int n,
				 int Sense,
				 int body_occ = -1,
				 int transf_occ = -1)
		{
			MyEdge = Edge;
			MyBody = Body;
			MySegment = Seg;
            MyFace = theFace;
			MyBodyIndex = body_occ;
			MyTransformIndex = transf_occ;

			int nn = n*3;
			if (nn > 0)
			{
				myPts = ACIS_NEW float[nn];
		        for (int i = 0; i<nn; ++i)
			       myPts[i] = Point[i];
			}

			myNP = n;
			MySense = Sense;
		};

		void SetBounds(double SParam, double EParam) {mySParam = SParam; myEParam = EParam; };
};
/**
 * Defines an PHLV5 edge.
 * <br>
 * <b>Role:</b> The <tt>PHLV5_EDGE</tt> class acts as the primary output of the API <tt>api_phlv5_compute</tt>. Each PHLV5 edge is
 * essentially a wrapper to an underlying ACIS EDGE. It contains a pointer to the CURVE of the <tt>EDGE</tt>, a pointer to a list of
 * <tt>PHLV5_SEGMENTs</tt> (containing visibility information for that <tt>EDGE</tt>), and a pointer to a polyline representing
 * the <tt>EDGE</tt> in 3D space. The modeling data with visibility start and stop parameters can be used to display the
 * hidden line representation in an end user application or, for better performance, the polyline data can be used directly.
 * <br><br>
 * @see PHLV5_SEGMENT,BODY,CURVE
 */
class DECL_PHLV5 PHLV5_EDGE : public ENTITY_PHLV5
{
	public:

// Main constructor
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator,
 * because this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param Curve
 * A curve.
 * @param Body
 * Body.
 * @param theFace
 * Face.
 * @param Seg
 * Segment.
 * @param Point
 * Polyline points.
 * @param n
 * number of points.
 * @param MySense
 * Sense.
 * @param Edge
 * ACIS EDGE pointer
 * @param occ
 * PHLV5_OCC pointer.
 */
PHLV5_EDGE(CURVE* Curve=0,
			BODY* Body=0,
            FACE* theFace = 0,
			PHLV5_SEGMENT* Seg=0,
			float * Point=0,
			int n=0,
			int MySense = 0,
			EDGE* Edge=0,
			PHLV5_OCC * occ=0,
			logical persistable = TRUE);

// 
/**
 * Returns TRUE if the PHLV5_EDGE refers to a silhouette line.
 */
logical isSilhouette() const {return (MyCurve ? FALSE : TRUE);};


// Get the body of the edge
/**
 * Gets the <tt>BODY</tt> of the underlying modeling <tt>EDGE</tt>.
 */
BODY* GetBody() const {return (MyBody);};

// Get the modeling EDGE of the edge
/**
 * Gets the <tt>EDGE</tt> of the underlying modeling <tt>EDGE</tt>.
 */
EDGE* GetEdge() const {return (MyEdge);};

// Get the curve
/**
 * Gets the <tt>CURVE</tt> of the underlying modeling <tt>EDGE</tt>.
 */
CURVE* GetCurve( AcisOptions* ao = NULL );

// Gets the face of the edge
/**
 * Gets the <tt>FACE</tt> of the underlying modeling <tt>EDGE</tt>.
 */
FACE* GetFace() const {return (MyFace);};

PHLV5_OCC* GetOccurrence() const {return (MyOccurrence);};

// Get the number of points in the polyline list
/**
 * Gets the number of points in the polyline list.
 */
int GetNPoints() const;

// Get the start and end parameters of the underlying modeling CURVE
/**
 * Gets the start and end parameters of the underlying modeling <tt>CURVE</tt>.
 * <br><br>
 * @param SParam
 * Start parameter.
 * @param EParam
 * End parameter.
 */
int GetParam(double &SParam, double &EParam) const;

// Get the segments in the form of a linked list
/**
 * Returns the pointer to the first segment.
 */
 PHLV5_SEGMENT* GetSegment() const {return MySegment;}

// Get the segments in the form of an entity list
/**
 * Gets the segments in the form of an entity list.
 * <br><br>
 * @param seg_list
 * Entity list.
 */
void GetSegmentList( ENTITY_LIST& seg_list ) const;

// Get the reverse bit for the edge
/**
 * Gets the sense of the underlying modeling <tt>EDGE</tt>.
 */
int GetSense() const;

// Get a pointer to the polyline points
/**
 * Gets a pointer to the polyline points.
 */
float * GetTabPoint() const;

// Get a specific polyline point
/**
 * Gets a specific polyline point.
 * <br><br>
 * @param i
 * Single coordinate of a polyline point.
 */
float GetVal(int i) const;

/*
// Documentation : To be revisited
*/
/**
 * @nodoc
 */
CURVE* GetCurveInt() const;

// Set the visibility SPAparameter bounds for the edge
/**
 * Sets the start and end parameters of the curve.
 * <br><br>
 * @param SParam
 * Start parameter.
 * @param EParam
 * End parameter.
 */
void SetBounds(double SParam, double EParam);

// Copy Constructor
/**
 * C++ copy constructor.
 * <br><br>
 * @param e
 * PHLV5_EDGE to copy.
 */
PHLV5_EDGE(PHLV5_EDGE const& e);


/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
void clean();
virtual logical savable() const;
/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
 static SPAposition eye;

/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
 static SPAposition target;

/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
 static logical projection_type;

/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
		PHLV5_HOOK_DEC ( PHLV5_EDGE )

/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
		PHLV5_UNHOOK_DEC ( PHLV5_EDGE )

	protected:
		CURVE* MyCurve;
		EDGE* MyEdge;
		BODY* MyBody;
        FACE* MyFace;
		PHLV5_OCC * MyOccurrence;
		PHLV5_SEGMENT* MySegment;
		float* myPts;
		int myNP;
		double mySParam;
		double myEParam;
		int MySense;
        ENTITY_LIST all_edge_list;
        SPAbox bbox;
        ENTITY* MySilh;
		logical Persistable;

/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
		PHLV5_CHAIN_DEC ( PHLV5_EDGE )

		// Don't doc
/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
		ENTITY_FUNCTIONS ( PHLV5_EDGE, PHLV5 )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
		void set_curve(CURVE* Cu);

/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
        void set_face(FACE* fa);

/**
  * @nodoc
*/
        void set_edge(EDGE* ed);

/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
		void set_body(BODY* by);

/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
		void set_segment(PHLV5_SEGMENT* sg);

		void set_occurrence(PHLV5_OCC* occ);
/*
// Documentation : To be revisited
*/
/**
  * @nodoc
 */
		EDGE * get_silhouette( );
		void set_silhouette( EDGE * edge );

};
/*! @} */
#endif



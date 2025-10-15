#ifndef __CODE_H__
#define __CODE_H__

#include <meshgems/basic_types.h>

/**

\defgroup code Message code
\ingroup message
@{

 Diagnostic code (error or information) used by the \ref
 meshgems_message_t structure.

 The sign of a MeshGems code (used by the \ref meshgems_message_t
structure) is the first information it provides:
 + a null code number indicates an unstructured message (ie 
  a pure text message without any attached data),
 + a strictly negative code number indicates an error,
 + a strictly postitive code number indicates an information.

A non zero code number can be written as follows :
code = (1000000 * q + r), where 0 <= r  <  1000000.

Here, `q` identifies the module and `r` identifies the
diagnostic independently of the module and defines the base number of
the message.
The mapping between `q` and MeshGems module/components is as follows :
 + 1 for MG-Tetra,
 + 3 for MG-CADSurf,
 + 4 for MG-Hexa,
 + 5 for MG-Cleaner,
 + 6 for MG-SurfOpt,
 + 7 for MG-Adapt,
 + 8 for MG-Hybrid,
 + 9 for MG-Tetra_HPC,
 + 10 for MG-Resilient.
 + 11 for MG-Extruder.
 + 12 for MG-Hybrid_CFD.

\section code_list List of MeshGems code

The following table presents the list of root MeshGems code, without
the component part and without the potential sign. The integer and
real data are given with the corresponding in brackets. For example,
[i] in the column `Integer data` means that this information can be
retrieved by calling the function \ref
meshgems_message_get_integer_data on the corresponding \ref
meshgems_message_t with istart = iend = i as parameters.

<table>
<caption> List of MeshGems codes </caption>
<tr>
  <th> Code number </th>
  <th> Meaning </th>
  <th> Integer data </th>
  <th> Real data </th>
  <th> Components </th>
  <th> Comment </th>
</tr>
<tr>
  <td> 3000 </td>
  <td> Unsufficient memory </td>
  <td> </td>
  <td> </td>
  <td> CADSurf </td>
  <td> </td>
</tr>
<tr>
  <td> 5341 </td>
  <td> A CAD face contains some adjacent and very close CAD edges </td>
  <td> [1] : the id of the face
  </td>
  <td> </td>
  <td> CADSurf </td>
  <td> </td>
</tr>
<tr>
  <td> 5342 </td>
  <td> A CAD face contains some adjacent and very close CAD edges. Requires the input topology to be kept </td>
  <td> [1] : the id of the face\n [2] : the id of the first CAD edge\n [3] : the id of the second CAD edge </td>
  <td> [1] : the approximated X coordinates of the location of the problem\n
  [2] : the approximated Y coordinates of the location of the problem\n
  [3] : the approximated Z coordinates of the location of the problem </td>
  <td> CADSurf </td>
  <td> </td>
</tr>
<tr>
  <td> 8001 </td>
  <td> The CAD face cannot be meshed </td>
  <td> [1] : the id of the discarded face</td>
  <td> </td>
  <td> CADSurf </td>
  <td> Check the face definition in the CAD system </td>
</tr>
<tr>
  <td> 8002 </td>
  <td> A CAD face was discarded during the CAD preprocessing because a correct definition could not be recovered </td>
  <td> [1] : the id of the discarded face</td>
  <td> </td>
  <td> CADSurf </td>
  <td> Check the face definition in the CAD system </td>
</tr>
<tr>
  <td> 8003 </td>
  <td> A CAD face was optimized out during the CAD preprocessing </td>
  <td> [1] : the id of the discarded face</td>
  <td> </td>
  <td> CADSurf </td>
  <td> Check the component's parameters, and the face definition in the CAD system </td>
</tr>
<tr>
  <td> 8004 </td>
  <td> The required discretization of the CAD entity cannot be respected </td>
  <td> [1] : the id of the entity which could not be discretized as requested </td>
  <td> </td>
  <td> CADSurf </td>
  <td> </td>
</tr>
<tr>
  <td> 8005 </td>
  <td> The periodicity constraint on the given faces cannot be respected </td>
  <td>
   [1] : the index of the face periodicity\n
   [2] : size n1 of the first set of periodic faces\n
   [3] : id of the first face of the first set of periodic faces\n
   [...] : id of the face of the first set of periodic faces\n
   [n1+2] : id of the last face of the first set of periodic faces\n
   [n1+3] : size n2 of the second set of periodic faces\n
   [n1+4] : id of the first face of the second set of periodic faces\n
   [...] : id of the face of the second set of periodic faces\n
   [n1+3+n2] : id of the last face of the second set of periodic faces\n
  </td>
  <td> </td>
  <td> CADSurf </td>
  <td> Check the periodicity of the two faces, the transformation, or retry with another periodic tolerance if necessary</td>
</tr>
<tr>
  <td> 8006 </td>
  <td> The periodicity constraint on the given edges cannot be respected </td>
  <td>
   [1] : the index of the edge periodicity\n
   [2] : size n1 of the first set of periodic edges\n
   [3] : id of the first edge of the first set of periodic edges\n
   [...] : id of the edge of the first set of periodic edges\n
   [n1+2] : id of the last edge of the first set of periodic edges\n
   [n1+3] : size n2 of the second set of periodic edges\n
   [n1+4] : id of the first edge of the second set of periodic edges\n
   [...] : id of the edge of the second set of periodic edges\n
   [n1+3+n2] : id of the last edge of the second set of periodic edges</td>
  <td> </td>
  <td> CADSurf </td>
  <td> Check the periodicity of the two edges, the transformation, or retry with
  another periodic tolerance if necessary </td>
</tr>
<tr>
  <td> 8041 </td>
  <td> A tiny CAD edge has been removed on the given face, during the CAD preprocessing </td>
  <td> [1] : the id of the concerned CAD face </td>
  <td> </td>
  <td> CADSurf </td>
  <td> </td>
</tr>
<tr>
  <td> 8042 </td>
  <td> A tiny CAD edge on the given face was left in the CAD as a result of the 
  specified options, during the CAD preprocessing </td>
  <td> [1] : the id of the concerned CAD face </td>
  <td> </td>
  <td> CADSurf </td>
  <td> </td>
</tr>
<tr>
  <td> 8100 </td>
  <td> There are most likely some intersections in the resulting mesh </td>
  <td> [1] : the number of suspected intersections in the resulting mesh </td>
  <td> </td>
  <td> CADSurf </td>
  <td> </td>
</tr>
<tr>
  <td> 8111 </td>
  <td> A quadratic node is not exactly on the geometry </td>
  <td> [1] : the index of the vertex in the resulting mesh </td>
  <td> </td>
   <td> CADSurf, Tetra </td>
 <td> </td>
</tr>
<tr>
  <td> 8423 </td>
  <td> A triangle with an invalid Jacobian is present in the resulting mesh</td>
  <td> [1] : the index of the triangle in the resulting mesh </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> 8424 </td>
  <td> A quadrangle with an invalid Jacobian is present in the resulting mesh </td>
  <td> [1] : the index of the quadrangle in the resulting mesh </td>
  <td> </td>
  <td> CADSurf </td>
 <td> </td>
</tr>
<tr>
  <td> 9001 </td>
  <td> Progress percentage report </td>
  <td> [1] : the percentage of progress of the meshing process </td>
  <td> </td>
  <td> CADSurf, Tetra </td>
  <td> </td>
</tr>
<tr>
  <td> 9011 </td>
  <td> Reading progress percentage report </td>
  <td>
  [1] : the percentage of progress while reading the mesh \n
  [2] : the elapse time spent up to now while reading the mesh
  </td>
  <td> </td>
  <td> Core </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>
<tr>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
  <td> </td>
</tr>




</table>


*/


/* RANGE from +-01000000 to +-01999999 reserved for Tetra */
/* RANGE from +-02000000 to +-02999999 reserved for PointCAD */
/* RANGE from +-03000000 to +-03999999 reserved for CADSurf */
/* RANGE from +-04000000 to +-04999999 reserved for Hexa */
/* RANGE from +-05000000 to +-05999999 reserved for Cleaner */
/* RANGE from +-06000000 to +-06999999 reserved for SurfOpt */
/* RANGE from +-07000000 to +-07999999 reserved for Adapt */
/* RANGE from +-08000000 to +-08999999 reserved for Hybrid */
/* RANGE from +-09000000 to +-09999999 reserved for Tetra_HPC */
/* RANGE from +-10000000 to +-10999999 reserved for Resilient */
/* RANGE from +-11000000 to +-11999999 reserved for Extruder */
/* RANGE from +-99000000 to +-99999999 reserved for Proximity */

/**
 * \name Usefull macros
 */
/**@{*/

/** Definition of the code base number for a component */
#define MESHGEMS_CODE_BASE 1000000

/** Definition of the code base number for MG-Tetra */
#define MESHGEMS_CODE_TETRA_BASE   (1*MESHGEMS_CODE_BASE)
/** Definition of the code base number for MG-PrecAD */
#define MESHGEMS_CODE_POINTCAD_BASE  (2*MESHGEMS_CODE_BASE)
/** Definition of the code base number for MG-CADSurf */
#define MESHGEMS_CODE_CADSURF_BASE (3*MESHGEMS_CODE_BASE)
/** Definition of the code base number for MG-Hexa */
#define MESHGEMS_CODE_HEXA_BASE    (4*MESHGEMS_CODE_BASE)
/** Definition of the code base number for MG-Cleaner */
#define MESHGEMS_CODE_CLEANER_BASE (5*MESHGEMS_CODE_BASE)
/** Definition of the code base number for MG-SurfOpt */
#define MESHGEMS_CODE_SURFOPT_BASE (6*MESHGEMS_CODE_BASE)
/** Definition of the code base number for MG-Adapt */
#define MESHGEMS_CODE_ADAPT_BASE   (7*MESHGEMS_CODE_BASE)
/** Definition of the code base number for MG-Hybrid */
#define MESHGEMS_CODE_HYBRID_BASE   (8*MESHGEMS_CODE_BASE)
/** Definition of the code base number for MG-Tetra_HPC */
#define MESHGEMS_CODE_TETRA_HPC_BASE   (9*MESHGEMS_CODE_BASE)
/** Definition of the code base number for MG-Resilient */
#define MESHGEMS_CODE_RESILIENT_BASE   (10*MESHGEMS_CODE_BASE)
/** Definition of the code base number for MG-Extruder */
#define MESHGEMS_CODE_EXTRUDER_BASE   (11*MESHGEMS_CODE_BASE)
/** Definition of the code base number for MG-Hybrid_CFD */
#define MESHGEMS_CODE_HYBRID_CFD_BASE   (12*MESHGEMS_CODE_BASE)

/* FIXME: pas de commentaire pour la prox? -- ** Definition of the code base number for MG-Proximity */
#define MESHGEMS_CODE_PROXIMITY_BASE   (99*MESHGEMS_CODE_BASE)
/** Definition of the code base number for MG-Extruder */
#define MESHGEMS_CODE_CVM_BASE   (98*MESHGEMS_CODE_BASE)

/** Return the code without the code base indicating the component */
#define MESHGEMS_CODE(x_) ((x_) >= 0 ? ((x_) % MESHGEMS_CODE_BASE) : -((-(x_)) % MESHGEMS_CODE_BASE))
/** Return the absolute value of a code, without the code base indicating the component */
#define MESHGEMS_ABS_CODE(x_) (((x_)>=0 ? (x_) : -(x_)) % MESHGEMS_CODE_BASE)
/** Return the index of the component from the given code number */
#define MESHGEMS_MODULE(x_) ((((x_)>=0 ? (x_) : -(x_)) / MESHGEMS_CODE_BASE) * MESHGEMS_CODE_BASE)
/** Return 1 is the code represents an error, 0 else */
#define MESHGEMS_CODE_IS_ERROR(x_) ((x_) < 0)


/** Build a code number for MG-Tetra from a code number wihtout component information */
#define MESHGEMS_TETRA_CODE(x_)    ((x_) >= 0 ? (MESHGEMS_CODE_TETRA_BASE+(x_)) : -(MESHGEMS_CODE_TETRA_BASE-(x_)))
/** Build a code number for MG-PreCAD from a code number wihtout component information */
#define MESHGEMS_POINTCAD_CODE(x_)   ((x_) >= 0 ? (MESHGEMS_CODE_POINTCAD_BASE+(x_)) : -(MESHGEMS_CODE_POINTCAD_BASE-(x_)))
/** Build a code number for MG-CADSurf from a code number wihtout component information */
#define MESHGEMS_CADSURF_CODE(x_)  ((x_) >= 0 ? (MESHGEMS_CODE_CADSURF_BASE+(x_)) : -(MESHGEMS_CODE_CADSURF_BASE-(x_)))
/** Build a code number for MG-Hexa from a code number wihtout component information */
#define MESHGEMS_HEXA_CODE(x_)     ((x_) >= 0 ? (MESHGEMS_CODE_HEXA_BASE+(x_)) : -(MESHGEMS_CODE_HEXA_BASE-(x_)))
/** Build a code number for MG-Cleaner from a code number wihtout component information */
#define MESHGEMS_CLEANER_CODE(x_)  ((x_) >= 0 ? (MESHGEMS_CODE_CLEANER_BASE+(x_)) : -(MESHGEMS_CODE_CLEANER_BASE-(x_)))
/** Build a code number for MG-SurfOpt from a code number wihtout component information */
#define MESHGEMS_SURFOPT_CODE(x_)  ((x_) >= 0 ? (MESHGEMS_CODE_SURFOPT_BASE+(x_)) : -(MESHGEMS_CODE_SURFOPT_BASE-(x_)))
/** Build a code number for MG-Adapt from a code number wihtout component information */
#define MESHGEMS_ADAPT_CODE(x_)  ((x_) >= 0 ? (MESHGEMS_CODE_ADAPT_BASE+(x_)) : -(MESHGEMS_CODE_ADAPT_BASE-(x_)))
/** Build a code number for MG-Hybrid from a code number wihtout component information */
#define MESHGEMS_HYBRID_CODE(x_)  ((x_) >= 0 ? (MESHGEMS_CODE_HYBRID_BASE+(x_)) : -(MESHGEMS_CODE_HYBRID_BASE-(x_)))
/** Build a code number for MG-Tetra_HPC from a code number wihtout component information */
#define MESHGEMS_TETRA_HPC_CODE(x_)  ((x_) >= 0 ? (MESHGEMS_CODE_TETRA_HPC_BASE+(x_)) : -(MESHGEMS_CODE_TETRA_HPC_BASE-(x_)))
/** Build a code number for MG-Resilient from a code number wihtout component information */
#define MESHGEMS_RESILIENT_CODE(x_)  ((x_) >= 0 ? (MESHGEMS_CODE_RESILIENT_BASE+(x_)) : -(MESHGEMS_CODE_RESILIENT_BASE-(x_)))
/** Build a code number for MG-Extruder from a code number wihtout component information */
#define MESHGEMS_EXTRUDER_CODE(x_)  ((x_) >= 0 ? (MESHGEMS_CODE_EXTRUDER_BASE+(x_)) : -(MESHGEMS_CODE_EXTRUDER_BASE-(x_)))
/** Build a code number for MG-Hybrid_CFD from a code number wihtout component information */
#define MESHGEMS_HYBRID_CFD_CODE(x_)  ((x_) >= 0 ? (MESHGEMS_CODE_HYBRID_CFD_BASE+(x_)) : -(MESHGEMS_CODE_HYBRID_CFD_BASE-(x_)))

#define MESHGEMS_PROXIMITY_CODE(x_)  ((x_) >= 0 ? (MESHGEMS_CODE_PROXIMITY_BASE+(x_)) : -(MESHGEMS_CODE_PROXIMITY_BASE-(x_)))
#define MESHGEMS_CVM_CODE(x_)  ((x_) >= 0 ? (MESHGEMS_CODE_CVM_BASE+(x_)) : -(MESHGEMS_CODE_CVM_BASE-(x_)))

/**@}*/

/**@}*/

#endif

/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef __VA_MESH_HXX__
#define __VA_MESH_HXX__

/**
 * @file va_mesh.hxx
 * @addtogroup Mesh
 * @{
 */

#include "dcl_vabridge.h"
#include "mmgr.hxx"


class VA_MESH;
class VA_SURFMESH;
class VA_TETMESH;
class VA_VOLMESH;

class va_surfmesh;
class va_tetmesh;
class va_volmesh;

/**
 *  Abstract base class.
 */
class DECL_VABRIDGE va_mesh: public ACIS_OBJECT
{
public:

    /**
     * Constructor
     */
    explicit va_mesh( VA_MESH* p_mesh);
    /**
     * Destructor
     */
    virtual ~va_mesh(void);

    /**
     * Gets underlying VKI %surface mesh object.
     * @return VA_SURFMESH*
     */
    virtual VA_SURFMESH* get_SURFMESH() const = 0;

    /**
     * Gets underlying VKI tet mesh object.
     * @return VA_TETMESH*
     */
    virtual VA_TETMESH* get_TETMESH() const = 0;

    /**
     * Gets underlying VKI vol mesh object.
     * @return VA_VOLMESH*
     */
    virtual VA_VOLMESH* get_VOLMESH() const = 0;

    /**
     * Gets underlying abstract VKI mesh object.
     * @return VA_MESH
     */
    VA_MESH* get_mesh() const;

private:
    VA_MESH* m_pMesh;
};

/**
 * @brief Bridge class that provides for the automatic generation of linear or parabolic
 * triangular or quadrilateral elements given an input %surface geometry.
 *
 * @role The input %surface geometry consists of a tessellation of the %surface geometry into
 * triangles. There are no limits placed on the size and shape of the input triangles;
 * they only need to represent the %surface geometry to the desired degree of precision.
 * Any edge of any input triangulation may be flagged to be preserved in the final mesh.
 * The input triangulation must be conformal, but not necessarily closed.
 *
 * @sa va_surface_mesh_options, va_tetmesh
 *
 */
class DECL_VABRIDGE va_surfmesh : public va_mesh
{
public:
    /**
     * Constructor
     */
    explicit va_surfmesh( VA_MESH* p_mesh);
    /**
     * Destructor
     */
    virtual ~va_surfmesh(void);

    virtual VA_SURFMESH* get_SURFMESH() const;
    virtual VA_TETMESH* get_TETMESH() const;
    virtual VA_VOLMESH* get_VOLMESH() const;
};

/**
 * @brief Bridge class that provides for the automatic generation of linear or parabolic
 * tetrahedral elements given a %surface mesh as input geometry.
 *
 * @role The mesh generation procedure is guaranteed to not alter the %surface mesh in any way.
 * The node numbering of the %surface mesh is preserved in the node numbering and connectivity
 * of the resultant tetrahedral mesh. The input %surface mesh is checked for closure, that is,
 * all input %surface triangles must be connected to at least one triangle across each edge.
 *
 * @sa va_tet_mesh_options, va_surfmesh
 */
class DECL_VABRIDGE va_tetmesh : public va_mesh
{
public:
    /**
     * Constructor
     * @param p_mesh Pointer to underlying Mesh object
     */
     explicit va_tetmesh( VA_MESH* p_mesh);

    /**
     * Destructor
     */
    virtual ~va_tetmesh(void);

    virtual VA_SURFMESH* get_SURFMESH() const;
    virtual VA_TETMESH* get_TETMESH() const;
    virtual VA_VOLMESH* get_VOLMESH() const;
};

/**
 * @brief Bridge class that provides for the automatic generation of linear or parabolic,
 * triangular and tetrahedral elements.
 *
 * @sa va_tetmesh, va_surfmesh
 */
class DECL_VABRIDGE va_volmesh : public va_mesh
{
public:
    /**
     * Constructor
     * @param p_mesh Pointer to underlying Mesh object
     */
     explicit va_volmesh( VA_MESH* p_mesh);

    /**
     * Destructor
     */
    virtual ~va_volmesh(void);

    virtual VA_SURFMESH* get_SURFMESH() const;
    virtual VA_TETMESH* get_TETMESH() const;
    virtual VA_VOLMESH* get_VOLMESH() const;
};


/** @}*/
#endif // __VA_MESH_HXX__

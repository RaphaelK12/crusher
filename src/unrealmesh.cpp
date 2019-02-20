// ===========================================================================
// $Source$
// $Revision$
// $Date$
// ===========================================================================
// Copyright (C) 1999 Tom Conder. All Rights Reserved.
//
// BY USING ANY PORTION OF THIS SOFTWARE, YOU AGREE TO THE FOLLOWING
// TERMS AND CONDITIONS:
// 
// Tom Conder, "THE AUTHOR", grants you, "THE USER", a non-exclusive,
// royalty free, license to use this software in source and binary code
// form, provided the user does not utilize the software in a manner
// which is disparaging to the author and the user acknowledges the
// author in any derivative work.
// 
// This software is provided "AS IS," without a warranty of any kind. ALL
// EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
// ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. THE AUTHOR SHALL NOT
// BE LIABLE FOR ANY DAMAGES SUFFERED BY THE USER AS A RESULT OF USING,
// MODIFYING OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT
// WILL THE AUTHOR BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR
// DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE
// DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY,
// ARISING OUT OF THE USE OF OR INABILITY TO USE SOFTWARE, EVEN IF THE
// AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
// ===========================================================================
// Project:    Crusher md2 viewer
//
// File:       unrealmesh.cpp
//
// Written:    12 Jan. 1999 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This module contains code to handle reading meshes from Unreal package
//	files.
// ===========================================================================
#include "stdafx.h"
#include "defs.h"
#include "errors.h"
#include "3dmath.h"
#include "unrealmesh.h"
#include <stdio.h>

CUnrealMesh::CUnrealMesh()
{
	m_sNumPolygons = 0;
	m_sNumVertices = 0;
}

CUnrealMesh::~CUnrealMesh()
{
	if (m_sNumPolygons == 0)
	{
		return;
	}

	delete [] m_points;
}

CUnrealMesh&
CUnrealMesh::operator= (const CUnrealMesh& rhs)
{
	int i;

	if (this != &rhs)
	{
		if (m_sNumPolygons != 0)
		{
			delete [] m_points;
			delete [] m_vertices;
			delete [] m_tri_index;
		}

		m_sNumPolygons = rhs.m_sNumPolygons;
		m_sNumVertices = rhs.m_sNumVertices;

		m_tri_index = new UJMeshTri[m_sNumPolygons];

		for (i=0; i < m_sNumPolygons; i++)
		{
			memcpy (&m_tri_index[i], &rhs.m_tri_index[i], sizeof(UJMeshTri));
		}

		m_vertices = new UJMeshVert[m_sNumVertices];
		m_points = new POINT_3D[m_sNumVertices];

		for (i=0; i < m_sNumVertices; i++)
		{
			memcpy (&m_vertices[i], &rhs.m_vertices[i], sizeof(UJMeshVert));
		}
	}

	return *this;
}

UINT
CUnrealMesh::Load (LPCTSTR tDataFilename, LPCTSTR tAnivFilename)
{
	FILE		*pAnivFile;
	FILE		*pDataFile;
	UDataHeader	udtDataHdr;
	UAnivHeader	udtAnivHdr;
	int			i;

	// ** read _d.3d file ---------------------------------------------

    pDataFile = fopen(tDataFilename, "rb");
    if (pDataFile == (FILE *) 0)
    {
        // Unable to open file for reading
        return iERR_GENERIC;
    }

	fread (&udtDataHdr, sizeof(UDataHeader), 1, pDataFile);

	m_sNumPolygons = udtDataHdr.sNumPolygons;
	m_sNumVertices = udtDataHdr.sNumVertices;

	m_tri_index = new UJMeshTri[m_sNumPolygons];

	for (i=0; i < m_sNumPolygons; i++)
	{
		fread (&m_tri_index[i], sizeof (UJMeshTri), 1, pDataFile);
	}

	fclose (pDataFile);

	// ** read _a.3d file ---------------------------------------------

    pAnivFile = fopen(tAnivFilename, "rb");
    if (pAnivFile == (FILE *) 0)
    {
        // Unable to open file for reading
        return iERR_GENERIC;
    }

	fread (&udtAnivHdr, sizeof(UAnivHeader), 1, pAnivFile);

	m_vertices = new UJMeshVert[m_sNumVertices];
	m_points = new POINT_3D[m_sNumVertices];

	for (i=0; i < m_sNumVertices; i++)
	{
		fread (&m_vertices[i], sizeof (UJMeshVert), 1, pAnivFile);
	}

	fclose (pAnivFile);

	return iRC_OK;
}

// ===========================================================================
// Name.......: VisibilitySort()
// Description:	This function sorts the model faces according to visibility
//				order.
// Parameters.: pMall				- a pointer to the mult matrix
// Returns....: UINT				- a return code
// ===========================================================================
// TODO: replace Matrix3D * - with - const Matrix3D &
UINT
CUnrealMesh::VisibilitySort (Matrix3D *pMall)
{
    int			i;
    POINT_3D	pt;

    if (m_sNumPolygons == 0)
    {
        return iRC_OK;
    }

    // transform all points
    for (i=0; i < m_sNumVertices; i++)
    {
        pt.x = (double) (m_vertices[i] & 0x7ff);
        pt.y = (double) ((m_vertices[i] >> 11) & 0x7ff);
        pt.z = (double) ((m_vertices[i] >> 22) & 0x3ff);

        pt = pMall->transform(pt);

        m_points[i].x = pt.x;
        m_points[i].y = pt.y;
        m_points[i].z = pt.z;
    }

    return iRC_OK;
}
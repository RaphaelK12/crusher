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
// File:       unrealmesh.h
//
// Written:    12 Jan. 1999 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This header file contains definitions for Epic Games Unreal I file
//	format. Some structures here originally appears in other sources.
// ===========================================================================
#ifndef __UNREALMESH_H
#define __UNREALMESH_H

typedef struct
{
	unsigned short	iVertex[3];		// Vertex indices.
	char			cType;			// James' mesh type.
	char			cColor;			// Color for flat and Gouraud shaded.
	unsigned char	uTex[3][2];  	// Texture UV coordinates.
	char			cTextureNum;	// Source texture offset.
	char			cFlags;			// Unreal mesh flags (currently unused).
} UJMeshTri;

typedef struct
{
	unsigned short	iVertex[3];		// Vertex indices.
	unsigned char	uTex[3][2];  	// Texture UV coordinates.
	unsigned long	dwPolyFlags;	// Surface flags
	int				iTextureIndex;	// Source texture offset.
} UMeshTri;

typedef unsigned long UJMeshVert;

typedef struct
{
    unsigned short  sNumPolygons;
    unsigned short  sNumVertices;
    unsigned short  sBogusRot;
    unsigned short  sBogusFrame;
    unsigned long   lBogusNormX;
    unsigned long   lBogusNormY;
    unsigned long   lBogusNormZ;
    unsigned long   lFixScale;
    unsigned long   lUnused[3];
    unsigned char   cUnknown[12];
} UDataHeader;

typedef struct
{
	unsigned short m_sNumFrames;
	unsigned short m_sFrameSize;
} UAnivHeader;

class CUnrealMesh
{
public:
    unsigned short  m_sNumPolygons;
    unsigned short  m_sNumVertices;

	UJMeshTri		*m_tri_index;
	UJMeshVert		*m_vertices;
	POINT_3D		*m_points;

public:
	CUnrealMesh();
	~CUnrealMesh();

	UINT Load				(LPCTSTR, LPCTSTR);
	UINT VisibilitySort		(Matrix3D *);

	CUnrealMesh (const CUnrealMesh&); // copy constructor
	CUnrealMesh & operator= (const CUnrealMesh&); // copy assignment
};

#endif // __UNREALMESH_H
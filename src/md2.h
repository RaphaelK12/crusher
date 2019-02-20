// ===========================================================================
// $Source: d:\\cvs\\master/crusher/src/md2.h,v $
// $Revision: 2.6 $
// $Date: 1998/12/08 04:16:53 $
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
// File:       md2.h
//
// Written:    16 Jun. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This header file contains definitions for id Software's md2 file format.
//    Some structures found here orginally appeared in the Quake 2 source
//    code distribution.
// ===========================================================================
#ifndef __MD2_H
#define __MD2_H

#include <string>
using std::string;

// ========================================================================
// Definitions from id Software's Code
// ========================================================================

// .MD2 triangle model file format

#define IDALIASHEADER		(('2'<<24)+('P'<<16)+('D'<<8)+'I')
#define ALIAS_VERSION	8

#define	MAX_TRIANGLES	4096
#define MAX_VERTS		2048
#define MAX_FRAMES		512
#define MAX_MD2SKINS	32
#define	MAX_SKINNAME	64

typedef struct
{
	short	s;
	short	t;
} dstvert_t;

typedef struct 
{
	short	index_xyz[3];
	short	index_st[3];
} dtriangle_t;

typedef struct
{
	byte	v[3];			// scaled byte to fit in frame mins/maxs
	byte	lightnormalindex;
} dtrivertx_t;

typedef struct
{
	float		scale[3];	// multiply byte verts by this
	float		translate[3];	// then add this
	char		name[16];	// frame name from grabbing
	dtrivertx_t	verts[1];	// variable sized
} daliasframe_t;

typedef struct
{
	int			ident;
	int			version;

	int			skinwidth;
	int			skinheight;
	int			framesize;		// byte size of each frame

	int			num_skins;
	int			num_xyz;
	int			num_st;			// greater than num_xyz for seams
	int			num_tris;
	int			num_glcmds;		// dwords in strip/fan command list
	int			num_frames;

	int			ofs_skins;		// each skin is a MAX_SKINNAME string
	int			ofs_st;			// byte offset from start for stverts
	int			ofs_tris;		// offset for dtriangles
	int			ofs_frames;		// offset for first frame
	int			ofs_glcmds;	
	int			ofs_end;		// end of file
} dmdl_t;

// ========================================================================
// End of id Software's Code Definitions
// ========================================================================

typedef struct
{
	POINT_3D *tris;
} framelist_t;

class CMd2Model
{
private:
	int			m_framesize;

	int			m_num_skins;
	int			m_num_xyz;
	int			m_num_st;

	char		m_skins[MAX_MD2SKINS][MAX_SKINNAME];
	framelist_t	*m_framelist;

	int			m_frame;

public:
	int			m_num_frames;
	int			m_num_tris;
	dtriangle_t	*m_tri_index;
	dstvert_t	*m_texture_st;

	int			m_skinwidth;
	int			m_skinheight;

	char		m_modelname[iMAX_PATH_LEN];
	char		m_skinname[iMAX_PATH_LEN];

	PCXBitmap	m_pcxTex;
	Vec3D		*m_normals;
	POINT_3D	*m_points;

private:
	UINT CheckTexture		(const PCXBitmap &);
	UINT WriteDataFile		(const string&, const string&);
	UINT WriteAnivFile		(const string&, const string&);
	UINT WriteUCFile		(const string&, const string&, const string&, BOOL);
	UINT WritePCXFile		(const string&, const string&);

public:
	CMd2Model();
	~CMd2Model();

	int  GetFrame			();
	UINT Load				(LPCTSTR);
	UINT LoadTexture		(LPTSTR);
	void NextFrame			();
	void PrevFrame			();
	void SetFrame			(int);
	UINT VisibilitySort		(Matrix3D *);
	UINT ExportToUnrealMesh	(const string&, const string&, const string&, BOOL);

	CMd2Model (const CMd2Model&); // copy constructor
	CMd2Model & operator= (const CMd2Model&); // copy assignment
};

#endif // __MD2_H
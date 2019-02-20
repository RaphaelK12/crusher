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
// File:       md2.cpp
//
// Written:    16 Jun. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This modules handles models stored in md2 format.
// ===========================================================================
#include "stdafx.h"
#include "defs.h"
#include "errors.h"
#include "3dmath.h"
#include "pcx.h"
#include "palette.h"
#include "md2.h"
#include "crusher.h"
#include "engine.h"
#include "render.h"
#include "unrealmesh.h"
#include <stdio.h>

POINT_3D	*g_points;

int   CompareZValues	(const void *, const void *);

// ===========================================================================
// Name.......: CompareZValues()
// Description:	This function is used by the sort function to compare the
//				z-coordinate values of two triangles.
// Parameters.: arg1				- the first triangle
//				arg2				- the second triangle
// Returns....: int					- -1 for less than, 0 for equals, 1 for
//									  greater than
// ===========================================================================
int
CompareZValues (const void *arg1, const void *arg2)
{
    dtriangle_t *tri;
    double dValZ1;
    double dValZ2;

    // use the average z value of the first triangle
    tri = (dtriangle_t *) arg1;
    dValZ1 = (g_points[tri->index_xyz[0]].z +
              g_points[tri->index_xyz[1]].z +
              g_points[tri->index_xyz[2]].z) / 3;

    // use the average z value of the second triangle
    tri = (dtriangle_t *) arg2;
    dValZ2 = (g_points[tri->index_xyz[0]].z +
              g_points[tri->index_xyz[1]].z +
              g_points[tri->index_xyz[2]].z) / 3;

    if (dValZ1 < dValZ2)
    {
        return 1;
    }
    else if (dValZ1 > dValZ2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
} // CompareZValues


// ===========================================================================
// Name.......: CMd2Model()
// Description:	This is the constructor.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
CMd2Model::CMd2Model()
{
    strcpy (m_skinname, tDEFAULT);

    m_frame = 0;
    m_num_frames = 0;
}

// ===========================================================================
// Name.......: ~CMd2Model()
// Description:	This is the deconstructor. It frees memory allocated to the
//				model.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
CMd2Model::~CMd2Model()
{
    int i;

    if (m_num_frames == 0)
    {
        return;
    }

    for (i=0; i < m_num_frames; i++)
    {
        delete [] m_framelist[i].tris;
    }

    m_num_frames = 0;

    delete [] m_points;
    delete [] m_framelist;
    delete [] m_tri_index;
    delete [] m_normals;
    delete [] m_texture_st;
}

// ===========================================================================
// Name.......: operator=()
// Description:	This function is the copy assignment.
// Parameters.: rhs					- right hand side
// Returns....: NIL
// ===========================================================================
CMd2Model&
CMd2Model::operator= (const CMd2Model& rhs)
{
    int i;
    int j;

    if (this != &rhs)
    {
        if (m_num_frames != 0)
        {
            m_num_frames = 0;

            for (i=0; i < m_num_frames; i++)
            {
                delete [] m_framelist[i].tris;
            }

            delete [] m_points;
            delete [] m_framelist;
            delete [] m_tri_index;
            delete [] m_normals;
            delete [] m_texture_st;
        }

        strcpy (m_modelname, rhs.m_modelname);
        strcpy (m_skinname, rhs.m_skinname);

        m_skinwidth = rhs.m_skinwidth;
        m_skinheight = rhs.m_skinheight;
        m_framesize = rhs.m_framesize;

        m_num_skins = rhs.m_num_skins;
        m_num_xyz = rhs.m_num_xyz;
        m_num_st = rhs.m_num_st;
        m_num_tris = rhs.m_num_tris;
        m_num_frames = rhs.m_num_frames;

        // skin names
        if (m_num_skins)
        {
            for (i=0; i < m_num_skins; i++)
            {
                strcpy (m_skins[i], rhs.m_skins[i]);
            }
        }

        // texture coordinates (dstvert_t)
        m_texture_st = new dstvert_t[m_num_st];
        memcpy (m_texture_st, rhs.m_texture_st, sizeof (dstvert_t) * m_num_st);

        // triangle array indexes
        m_tri_index = new dtriangle_t[m_num_tris];
        m_normals = new Vec3D[m_num_tris];
        memcpy (m_tri_index, rhs.m_tri_index, sizeof(dtriangle_t) * m_num_tris);
        m_framelist = new framelist_t[m_num_frames];
        m_points = new POINT_3D[m_num_xyz];

        // triangle vertices
        for (i=0; i < m_num_frames; i++)
        {
            m_framelist[i].tris = new POINT_3D[m_num_xyz];

            for (j=0; j < m_num_xyz; j++)
            {
                m_framelist[i].tris[j].x = rhs.m_framelist[i].tris[j].x;
                m_framelist[i].tris[j].y = rhs.m_framelist[i].tris[j].y;
                m_framelist[i].tris[j].z = rhs.m_framelist[i].tris[j].z;
            }
        }

        m_pcxTex = rhs.m_pcxTex;
    }

    return *this;
} // operator=

// ===========================================================================
// Name.......: CheckTexture()
// Description:	This function checks the texture to the model file.
// Parameters.: NIL
// Returns....: UINT				- a return code
// ===========================================================================
UINT
CMd2Model::CheckTexture (const PCXBitmap &pcxTex)
{
    UINT rc;

    rc = iRC_OK;

    if (pcxTex.m_iHeight < m_skinheight)
    {
        rc = iERR_MD2_TEXHEIGHT;
    }
    else if (pcxTex.m_iWidth < m_skinwidth)
    {
        rc = iERR_MD2_TEXWIDTH;
    }

    return rc;
}

// ===========================================================================
// Name.......: GetFrame()
// Description:	This function returns the current frame number.
// Parameters.: NIL
// Returns....: int				- frame number
// ===========================================================================
// TODO: rewrite this as a get property
int
CMd2Model::GetFrame()
{
    return m_frame;
}

// ===========================================================================
// Name.......: Load()
// Description:	This function loads a md2 model from a file.
// Parameters.: tFilename			- a string containing the filename
// Returns....: UINT				- a return code
// ===========================================================================
UINT
CMd2Model::Load (LPCSTR tFilename)
{
    dmdl_t			mdlHeader;
    FILE			*pMd2File;
    byte			buffer[MAX_VERTS*4+128];
    daliasframe_t	*out;
    int				i;
    int				j;

    m_frame = 0;

    pMd2File = fopen(tFilename, "rb");
    if (pMd2File == (FILE *) 0)
    {
        // Unable to open file for reading
        return iERR_MD2_OPENFILE;
    }

    // read model header
    fread(&mdlHeader, sizeof(dmdl_t), 1, pMd2File);

    m_skinwidth = mdlHeader.skinwidth;
    m_skinheight = mdlHeader.skinheight;
    m_framesize = mdlHeader.framesize;

    m_num_skins = mdlHeader.num_skins;
    m_num_xyz = mdlHeader.num_xyz;
    m_num_st = mdlHeader.num_st;
    m_num_tris = mdlHeader.num_tris;
    m_num_frames = mdlHeader.num_frames;

    // check file header
    if (mdlHeader.ident != IDALIASHEADER)
    {
        // Invalid md2 header
        return iERR_MD2_IDENT;
    }

    // read skin names
    fseek(pMd2File, mdlHeader.ofs_skins, SEEK_SET);
    if (m_num_skins)
    {
        for (i=0; i < m_num_skins; i++)
        {
            fread(m_skins[i], MAX_SKINNAME, 1, pMd2File);
        }
    }

    // read texture coordinates (dstvert_t)
    m_texture_st = new dstvert_t[m_num_st];
    if (m_texture_st == NULL)
    {
        return iERR_MD2_ST;
    }

    fseek (pMd2File, mdlHeader.ofs_st, SEEK_SET);
    fread (m_texture_st, m_num_st * sizeof(dstvert_t), 1, pMd2File);

    // read triangle array indexes
    m_tri_index = new dtriangle_t[m_num_tris];
    if (m_tri_index == NULL)
    {
        return iERR_MD2_TRIS;
    }

    m_normals = new Vec3D[m_num_tris];
    if (m_normals == (Vec3D *) 0)
    {
        return iERR_MD2_TRIS;
    }

    fseek(pMd2File, mdlHeader.ofs_tris, SEEK_SET);
    for (i=0; i < m_num_tris; i++)
    {
        fread (&m_tri_index[i], sizeof(dtriangle_t), 1, pMd2File);
    }

    m_framelist = new framelist_t[m_num_frames];
    if (m_framelist == NULL)
    {
        return iERR_MD2_FRAMELIST;
    }

    m_points = new POINT_3D[m_num_xyz];
    if (m_points == NULL)
    {
        return iERR_MD2_POINTS;
    }

    // read triangle vertices
    fseek(pMd2File, mdlHeader.ofs_frames, SEEK_SET);
    for (i=0; i < m_num_frames; i++)
    {
        m_framelist[i].tris = new POINT_3D[m_num_xyz];
        if (m_framelist[i].tris == NULL)
        {
            return iERR_MD2_TRIS;
        }

        out = (daliasframe_t *) buffer;

        fread(out, m_framesize, 1, pMd2File);

        for (j=0; j < m_num_xyz; j++)
        {
            m_framelist[i].tris[j].x = out->verts[j].v[0] * out->scale[0] + out->translate[0];
            m_framelist[i].tris[j].y = out->verts[j].v[1] * out->scale[1] + out->translate[1];
            m_framelist[i].tris[j].z = out->verts[j].v[2] * out->scale[2] + out->translate[2];
        }
    }

    fclose(pMd2File);

    return iRC_OK;
}

// ===========================================================================
// Name.......: LoadTexture()
// Description:	This function loads the texture.
// Parameters.: tFilename				- texture filename
// Returns....: UINT					- a return code
// ===========================================================================
UINT
CMd2Model::LoadTexture (LPTSTR tFilename)
{
    UINT		rc;
    PCXBitmap	pcxTemp;

    if ((rc = pcxTemp.Load (tFilename)) != iRC_OK)
    {
        return rc;
    }

    if ((rc = CheckTexture (pcxTemp)) != iRC_OK)
    {
        return rc;
    }

    m_pcxTex = pcxTemp;

    return iRC_OK;
} // LoadTexture

// ===========================================================================
// Name.......: NextFrame()
// Description:	This function advances the model one frame.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
void
CMd2Model::NextFrame()
{
    m_frame = (m_frame+1) % m_num_frames;
}

// ===========================================================================
// Name.......: PrevFrame()
// Description:	This function retreats the model one frame.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
void
CMd2Model::PrevFrame()
{
    if (m_frame == 0)
    {
        m_frame = m_num_frames - 1;
    }
    else
    {
        m_frame = m_frame - 1;
    }
}

// ===========================================================================
// Name.......: SetFrame()
// Description:	This function sets the model frame number.
// Parameters.: iFrame				- new value for frame
// Returns....: NIL
// ===========================================================================
// TODO: rewrite this as a let property
void
CMd2Model::SetFrame(int iFrame)
{
    m_frame = iFrame % m_num_frames;
}

// ===========================================================================
// Name.......: VisibilitySort()
// Description:	This function sorts the model faces according to visibility
//				order.
// Parameters.: pcxTex				- a pointer to the texture map
//				pMall				- a pointer to the mult matrix
// Returns....: UINT				- a return code
// ===========================================================================
// TODO: replace Matrix3D * - with - const Matrix3D &
UINT
CMd2Model::VisibilitySort (Matrix3D *pMall)
{
    int			i;
    POINT_3D	pt;
	POINT_3D	P0, P1, P2;

    if (m_num_frames == 0)
    {
        return iRC_OK;
    }

    // transform all points
    for (i=0; i < m_num_xyz; i++)
    {
        pt.x = m_framelist[m_frame].tris[i].x;
        pt.y = m_framelist[m_frame].tris[i].y;
        pt.z = m_framelist[m_frame].tris[i].z;

        pt = pMall->transform(pt);

        m_points[i].x = pt.x;
        m_points[i].y = pt.y;
        m_points[i].z = pt.z;
    }

    g_points = m_points;

    // sort triangles by z-coordinate
    qsort ((void *) m_tri_index,
           (size_t) m_num_tris,
           sizeof (dtriangle_t),
           CompareZValues);

	// transform normals
	for (i=0; i < m_num_tris; i++)
	{
		P0.x = m_points[m_tri_index[i].index_xyz[0]].x;
		P0.y = m_points[m_tri_index[i].index_xyz[0]].y;
		P0.z = m_points[m_tri_index[i].index_xyz[0]].z;

		P1.x = m_points[m_tri_index[i].index_xyz[1]].x;
		P1.y = m_points[m_tri_index[i].index_xyz[1]].y;
		P1.z = m_points[m_tri_index[i].index_xyz[1]].z;

		P2.x = m_points[m_tri_index[i].index_xyz[2]].x;
		P2.y = m_points[m_tri_index[i].index_xyz[2]].y;
		P2.z = m_points[m_tri_index[i].index_xyz[2]].z;

		m_normals[i].NormalPt(P0, P1, P2);
		m_normals[i].Normalize();
	}

    return iRC_OK;
}

// ===========================================================================
// Name.......: ExportToUnrealMesh()
// Description:	This function export a md2 model to Unreal I format.
// Parameters.: tProjectDir			- a string containing the project dir
//				tBaseName			- a string containing the base name
//				tSkinName			- a string containing the skin name
//				bIncSeq				- boolean for including player sequences
// Returns....: UINT				- a return code
// ===========================================================================
UINT
CMd2Model::ExportToUnrealMesh (const string &tProjectDir, const string &tBaseName, const string &tSkinName, BOOL bIncSeq)
{
	UINT		rc;

	// write _d.3d file
	if ((rc = WriteDataFile(tProjectDir, tBaseName)) != iRC_OK)
	{
		return rc;
	}

	// write _a.3d file
	if ((rc = WriteAnivFile(tProjectDir, tBaseName)) != iRC_OK)
	{
		return rc;
	}

	// write .uc file
	if ((rc = WriteUCFile(tProjectDir, tBaseName, tSkinName, bIncSeq)) != iRC_OK)
	{
		return rc;
	}

	// write .pcx file
	if ((rc = WritePCXFile(tProjectDir, tSkinName)) != iRC_OK)
	{
		return rc;
	}

	return iRC_OK;
}

// ===========================================================================
// Name.......: WriteDataFile()
// Description:	This function writes a data file.
// Parameters.: tProjectDir			- a string containing the project dir
//				tBaseName			- a string containing the base name
// Returns....: UINT				- a return code
// ===========================================================================
UINT
CMd2Model::WriteDataFile (const string &tProjectDir, const string &tBaseName)
{
	FILE		*pDataFile;
	string		tDataFilename;
	UDataHeader	udtDataHdr;
	UJMeshTri	udtMeshTri;
	int			i;

	tDataFilename = tProjectDir;
	tDataFilename += "\\Models\\";
	tDataFilename += tBaseName;
	tDataFilename += "_d.3d";

    pDataFile = fopen(tDataFilename.c_str(), "wb");
    if (pDataFile == (FILE *) 0)
    {
        return iERR_MD2_OPEN_DATAFILE;
    }

	memset (&udtDataHdr, 0, sizeof(udtDataHdr));

	udtDataHdr.sNumPolygons = m_num_tris;
	udtDataHdr.sNumVertices = m_num_xyz;

	fwrite (&udtDataHdr, sizeof(UDataHeader), 1, pDataFile);

	for (i=0; i < m_num_tris; i++)
	{
		udtMeshTri.iVertex[0] = m_tri_index[i].index_xyz[0];
		udtMeshTri.iVertex[1] = m_tri_index[i].index_xyz[2];
		udtMeshTri.iVertex[2] = m_tri_index[i].index_xyz[1];

		udtMeshTri.cType = 0;
		udtMeshTri.cColor = 0;
		udtMeshTri.uTex[0][0] = (unsigned char) m_texture_st[m_tri_index[i].index_st[0]].s & 0xFF;
		udtMeshTri.uTex[0][1] = (unsigned char) m_texture_st[m_tri_index[i].index_st[0]].t & 0xFF;
		udtMeshTri.uTex[1][0] = (unsigned char) m_texture_st[m_tri_index[i].index_st[2]].s & 0xFF;
		udtMeshTri.uTex[1][1] = (unsigned char) m_texture_st[m_tri_index[i].index_st[2]].t & 0xFF;
		udtMeshTri.uTex[2][0] = (unsigned char) m_texture_st[m_tri_index[i].index_st[1]].s & 0xFF;
		udtMeshTri.uTex[2][1] = (unsigned char) m_texture_st[m_tri_index[i].index_st[1]].t & 0xFF;
		udtMeshTri.cTextureNum = 0;
		udtMeshTri.cFlags = 0;

		fwrite (&udtMeshTri, sizeof (UJMeshTri), 1, pDataFile);
	}

	fclose (pDataFile);

	return iRC_OK;
}

// ===========================================================================
// Name.......: WriteAnivFile()
// Description:	This function writes an aniv file.
// Parameters.: tProjectDir			- a string containing the project dir
//				tBaseName			- a string containing the base name
// Returns....: UINT				- a return code
// ===========================================================================
UINT
CMd2Model::WriteAnivFile (const string &tProjectDir, const string &tBaseName)
{
	FILE		*pAnivFile;
	UAnivHeader	udtAnivHdr;
	UJMeshVert	udtMeshVert;
	string		tAnivFilename;
	int			i;
	int			j;

	tAnivFilename = tProjectDir;
	tAnivFilename += "\\Models\\";
	tAnivFilename += tBaseName;
	tAnivFilename += "_a.3d";

    pAnivFile = fopen(tAnivFilename.c_str(), "wb");
    if (pAnivFile == (FILE *) 0)
    {
        return iERR_MD2_OPEN_ANIVFILE;
    }

	udtAnivHdr.m_sFrameSize = m_num_xyz * sizeof(unsigned long);
	udtAnivHdr.m_sNumFrames = m_num_frames;
	fwrite (&udtAnivHdr, sizeof(UAnivHeader), 1, pAnivFile);

	for (i=0; i < m_num_frames; i++)
	{
		for (j=0; j < m_num_xyz; j++)
		{
			udtMeshVert = (int)(m_framelist[i].tris[j].x * 16.0) & 0x7ff;
			udtMeshVert |= ((int)(m_framelist[i].tris[j].y * 16.0) & 0x7ff) << 11;
			udtMeshVert |= ((int)(m_framelist[i].tris[j].z * 8.0) & 0x3ff) << 22;

			fwrite (&udtMeshVert, sizeof (udtMeshVert), 1, pAnivFile);
		}

	}

	fclose (pAnivFile);

	return iRC_OK;
}

// ===========================================================================
// Name.......: WriteUCFile()
// Description:	This function writes a class file suitable for Unreal I.
// Parameters.: tProjectDir			- a string containing the project dir
//				tBaseName			- a string containing the base name
//				tSkinName			- a string containing the skin name
//				bIncSeq				- boolean for including player sequences
// Returns....: UINT				- a return code
// ===========================================================================
UINT
CMd2Model::WriteUCFile (const string &tProjectDir, const string &tBaseName, const string &tSkinName, BOOL bIncSeq)
{
	FILE		*pUCFile;
	string		tUCFilename;
    string		tCommentLine;
    string		tExecMeshSeq;

	tUCFilename = tProjectDir;
	tUCFilename += "\\Classes\\";
	tUCFilename += tBaseName;
	tUCFilename += ".uc";

    pUCFile = fopen(tUCFilename.c_str(), "wb");
    if (pUCFile == (FILE *) 0)
    {
        return iERR_MD2_OPEN_CLASSFILE;
    }

    tCommentLine = "//";
    tCommentLine += string(77, '=' );
    tCommentLine += "\n";

    fputs(tCommentLine.c_str(), pUCFile );
    fprintf(pUCFile, "// %s.\n", tBaseName.c_str());
    fputs(tCommentLine.c_str(), pUCFile );
    
    fprintf(pUCFile, "class %s expands Actor;\n\n", tBaseName.c_str() );
    
    fprintf(pUCFile, "#exec MESH IMPORT MESH=%s "
                 "ANIVFILE=MODELS\\%s_a.3d "
                 "DATAFILE=MODELS\\%s_d.3d X=0 Y=0 Z=0\n",
                 tBaseName.c_str(), tBaseName.c_str(), tBaseName.c_str() );
    fprintf(pUCFile, "#exec MESH ORIGIN MESH=%s X=0 Y=0 Z=0\n\n",
                 tBaseName.c_str() );

    tExecMeshSeq = "#exec MESH SEQUENCE MESH=";
    tExecMeshSeq += tBaseName.c_str();
    tExecMeshSeq += " SEQ=";

    fprintf(pUCFile, "%s%s STARTFRAME=0 NUMFRAMES=%d\n",
                 tExecMeshSeq.c_str(), "All", m_num_frames);

	if (bIncSeq == TRUE)
	{
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Stand", 0, 39);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Run", 40, 6);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Attack", 46, 8);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Pain1", 54, 4);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Pain2", 58, 4);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Pain3", 62, 4);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Jump6", 66, 6);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Flip", 72, 12);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Salute", 84, 11);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Taunt", 95, 17);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Wave", 112, 11);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Point", 123, 12);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Crstnd", 135, 19);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Crwalk", 154, 6);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Crattak", 160, 9);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Crpain", 169, 4);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Crdeath", 173, 5);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Death1", 178, 6);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Death2", 184, 6);
		fprintf(pUCFile, "%s%s STARTFRAME=%d NUMFRAMES=%d\n",
			tExecMeshSeq.c_str(), "Death3", 190, 8);
	}

    fprintf(pUCFile, "\n" );

    fprintf(pUCFile, "#exec TEXTURE IMPORT NAME=J%s "
                 "FILE=MODELS\\%s GROUP=Skins ",
                 tBaseName.c_str(), tSkinName.c_str());
    fprintf(pUCFile, "FLAGS=2 // skins\n\n" );

    fprintf(pUCFile, "#exec MESHMAP NEW   MESHMAP=%s MESH=%s\n",
                 tBaseName.c_str(), tBaseName.c_str() );
    fprintf(pUCFile, "#exec MESHMAP SCALE MESHMAP=%s X=0.1 Y=0.1 Z=0.2\n\n",
                 tBaseName.c_str());

    fprintf(pUCFile, "#exec MESHMAP SETTEXTURE MESHMAP=%s "
                 "NUM=%d TEXTURE=J%s\n",
                  tBaseName.c_str(), 0, tBaseName.c_str());

    fprintf(pUCFile, "\ndefaultproperties\n{\n" );
    fprintf(pUCFile, "    DrawType=DT_Mesh\n" );
    fprintf(pUCFile, "    Mesh=%s\n", tBaseName.c_str());
    fprintf(pUCFile, "}\n" );

	fclose (pUCFile);

	return iRC_OK;
}

// ===========================================================================
// Name.......: WritePCXFile()
// Description:	This function write a file containing the pcx image.
// Parameters.: tProjectDir			- a string containing the project dir
//				tSkinName			- a string containing the skin name
// Returns....: UINT				- a return code
// ===========================================================================
UINT
CMd2Model::WritePCXFile (const string &tProjectDir, const string &tSkinName)
{
	UINT		rc;
	string		tPCXFilename;

	tPCXFilename = tProjectDir;
	tPCXFilename += "\\Models\\";
	tPCXFilename += tSkinName;

	rc = m_pcxTex.Save (tPCXFilename.c_str());
	if (rc != iRC_OK)
	{
		return rc;
	}

	return iRC_OK;
}
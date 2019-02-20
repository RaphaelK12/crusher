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
// File:       render.cpp
//
// Written:    16 Jun. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This module contains code to draw the model.
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
#include "registry.h"
#include "tri.h"
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>

// definitions
const double dHORZ_SCALE = 0.0068;
const double dVERT_SCALE = 0.0091;
const double dNUM_LEVELS = iMAX_COLOR - 1.0;

static BYTE			*pDIB;
static HBITMAP		hDIBSection;
static Matrix3D		Mall;
static RECT			rectClip;
static int			iDIBHeight;
static int			iDIBPitch;
static int			iDIBWidth;

static BOOL			fInited = FALSE;
static DWORD		dwIntensity = iFLATSHADE_TRI_COLOR;

//  :: LIGHTING COEFFICIENTS - DETERMINED EXPERIMENTALLY
//  ra - double for ambient reflection coefficient
//  rd - double for diffuse reflection coefficient
//  rs - double for specular reflection coefficient
static double ra	= 0.7;
static double rd	= 0.8;
static double rs	= 0.7;

// LIGHT QUANTIZATION VARIABLES
static double dMax	= 1.7;  // maximum intensity; determined experimentally

static void CopyPCXPixels			(BYTE *, int, PCXBitmap &);
static UINT CreateCompatibleDIB		(HDC, int, int);
static UINT CreateGreyscaleDIB		(HDC, int, int);
static void DrawModel				(CEngine &, const CMd2Model &);
static void DrawTriangle			(const triangle_t *, const PCXBitmap &, UINT);
static void FillDIB					(BYTE *, DWORD);
static UINT FindFirstSkinTexture	(LPTSTR, LPTSTR);
static int IsBackface				(triangle_t vertices[]);

static DWORD GetIntensity			();
static void SetIntensity			(const Vec3D &, const POINT_3D &);

// ===========================================================================
// Name.......: DrawFrame()
// Description:	This function draws a frame.
// Parameters.: hdcScreen				- handle to screen device context
//				mdlMd2					- a pointer to the model
// Returns....: NIL
// ===========================================================================
UINT
DrawFrame(HDC hdcScreen, CEngine &engViewer, CMd2Model &mdlMd2)
{
    HBITMAP		hOldBitmap;
    HDC			hMemDC;
    RECT		rect;

    if (fInited == TRUE)
    {
        if ((hMemDC = CreateCompatibleDC (hdcScreen)) == NULL)
        {
            return iERR_RENDER_COMPAT_DC;
        }

        hOldBitmap = (HBITMAP) SelectObject(hMemDC, hDIBSection);
        if (hOldBitmap == NULL)
        {
            return iERR_RENDER_BITMAP;
        }

        // clear the bitmap
		if (engViewer.RendState() == iREND_TEXTURED)
		{
			FillDIB (pDIB, iBACKGRND_COLOR);
		}
		else
		{
			FillDIB (pDIB, iMAX_COLOR);
		}

        // set the transform matrix to the unit matrix
        Mall.unit();

        Mall.xrot (engViewer.XRot());
        Mall.yrot (engViewer.YRot());
        Mall.zrot (engViewer.ZRot());
        Mall.translate (iDIBWidth/2.0 + engViewer.HorzOffset(),
                        iDIBHeight/2.0 + engViewer.VertOffset(),
                        1.0);

        // scale the model
        if (iDIBWidth < iDIBHeight)
        {
            Mall.scale (dHORZ_SCALE * iDIBWidth);
        }
        else
        {
            Mall.scale (dVERT_SCALE * iDIBHeight);
        }

        mdlMd2.VisibilitySort (&Mall);

		DrawModel (engViewer, mdlMd2);

        BitBlt (hdcScreen, 0, 0, iDIBWidth, iDIBHeight, hMemDC, 0, 0, SRCCOPY);

        SelectObject(hMemDC, hOldBitmap);
        DeleteDC (hMemDC);
    }
    else
    {
        rect.left = 0;
        rect.top = 0;
        rect.right = iDIBWidth;
        rect.bottom = iDIBHeight;

        SetBkColor (hdcScreen, RGB(75,79,127));
        ExtTextOut (hdcScreen, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
    }

    return iRC_OK;
} // DrawFrame

static void
DrawModel (CEngine &engViewer, const CMd2Model &md2Mdl)
{
	int			i;
    int			j;
    triangle_t	pTriangle[3];
	UINT		iRendState;

	// draw triangles
	iRendState = engViewer.RendState();
    for (i=0; i < md2Mdl.m_num_tris; i++)
    {
        for (j=0; j < 3; j++)
        {
            pTriangle[j].x = (long) md2Mdl.m_points[md2Mdl.m_tri_index[i].index_xyz[j]].x;
            pTriangle[j].y = (long) md2Mdl.m_points[md2Mdl.m_tri_index[i].index_xyz[j]].y;

            pTriangle[j].s = md2Mdl.m_texture_st[md2Mdl.m_tri_index[i].index_st[j]].s;
            pTriangle[j].t = md2Mdl.m_texture_st[md2Mdl.m_tri_index[i].index_st[j]].t;
        }

        if (iRendState == iREND_FLATSHADE)
        {
            SetIntensity (md2Mdl.m_normals[i],
							md2Mdl.m_points[md2Mdl.m_tri_index[i].index_xyz[0]]);
        }

        if (! IsBackface (pTriangle))
        {
            DrawTriangle (pTriangle, md2Mdl.m_pcxTex, iRendState);
        }
    }
}

// ===========================================================================
// Name.......: DrawTriangle()
// Description:	This function draws a triangle.
// Parameters.: pTriangle				- array of triangle vertices
//				pcxTex					- a pointer to the texture map
//				iRendState				- state to render triangle
// Returns....: NIL
// ===========================================================================
static void
DrawTriangle (const triangle_t *pTriangle, const PCXBitmap &pcxTex, UINT iRendState)
{
    DWORD	dwColor;

    if (iRendState == iREND_WIREFRAME)
    {
        ClippedWireframeTri8 (pDIB,
                              iDIBPitch,
                              pTriangle,
                              iWIREFRAME_TRI_COLOR,
                              &rectClip);
    }
    else if (iRendState == iREND_FLATSHADE)
    {
        // TODO: get the intensity
        dwColor = GetIntensity();

        ClippedFlatShadedTri8 (pDIB,
                               iDIBPitch,
                               pTriangle,
                               dwColor,
                               &rectClip);
    }
    else
    {
        ClippedTexturedTri8 (pDIB,
                             iDIBPitch,
                             pTriangle,
                             pcxTex,
                             &rectClip);
    }
} // DrawTriangle

// ===========================================================================
// Name.......: End3D()
// Description:	This function cleans up 3D operations.
// Parameters.: pcxTex					- a pointer to the texture map
//				mdlMd2					- a pointer to the model
// Returns....: NIL
// ===========================================================================
void
End3D(CMd2Model &mdlMd2)
{
    if (fInited == TRUE)
    {
        SaveRegValue(tREG_KEY_MODEL, mdlMd2.m_modelname);
        SaveRegValue(tREG_KEY_SKIN, mdlMd2.m_skinname);
    }

    fInited = FALSE;
} // End3D

// ===========================================================================
// Name.......: Init3D()
// Description:	This function initializes 3D operations.
// Parameters.: hInWnd					- handle to window
// Returns....: UINT					- a return code
// ===========================================================================
UINT
Init3D(HWND hInWnd)
{
    HDC				hDC;
    UINT			rc;
    int				iInHeight;
    int				iInWidth;
    RECT rect;

    fInited = FALSE;

    Initialize_CosSin();

    GetClientRect(hInWnd, &rect);

    iInWidth = rect.right - rect.left;
    iInHeight = rect.bottom - rect.top;

    if ((hDC = GetDC(hInWnd)) == NULL)
    {
        return iERR_RENDER_DC;
    }

    if ((rc = CreateCompatibleDIB (hDC, iInWidth, iInHeight)) != iRC_OK)
    {
        ReleaseDC(hInWnd, hDC);
        return rc;
    }

    rectClip.left = 0;
    rectClip.top = 0;
    rectClip.right = iDIBWidth;
    rectClip.bottom = iDIBHeight;

    ReleaseDC(hInWnd, hDC);

    fInited = TRUE;
    return iRC_OK;
} // Init3D

// ===========================================================================
// Name.......: LoadModel()
// Description:	This function loads the Model.
// Parameters.: tModelFilename			- name of model file
//				tTextureFilename		- name of texture file; if null,
//										  finds first skin texture in model
//										  dir
//				mdlMd2					- the model
//				engViewer				- the viewer
// Returns....: UINT					- a return code
// ===========================================================================
UINT
LoadModel(LPTSTR tModelFilename, LPTSTR tTextureFilename, CMd2Model &mdlMd2, CEngine &engViewer)
{
    UINT		rc;
    CMd2Model	mdlTemp;
    TCHAR		tFilename[iMAX_PATH_LEN];
	BYTE		uMask;

    if ((rc = mdlTemp.Load (tModelFilename)) != iRC_OK)
    {
        return rc;
    }

    strcpy (mdlTemp.m_modelname, tModelFilename);

    mdlMd2 = mdlTemp;

    if (tTextureFilename == (LPSTR) 0)
    {
        if ((rc = FindFirstSkinTexture (tModelFilename, tFilename)) != iRC_OK)
        {
            EnableRendMenuItem (iREND_TEXTURED, FALSE);

			uMask = engViewer.RendMask();
			engViewer.RendMask (uMask & iMASK_TEXTURED);

			if (engViewer.RendState() == iREND_TEXTURED)
			{
	            engViewer.RendState(iREND_FLATSHADE);
			}

            return iRC_OK;
        }
    }
    else if (strcmp (tTextureFilename, tDEFAULT) == 0)
    {
        EnableRendMenuItem (iREND_TEXTURED, FALSE);

		uMask = engViewer.RendMask();
		engViewer.RendMask (uMask & iMASK_TEXTURED);

		if (engViewer.RendState() == iREND_TEXTURED)
		{
	        engViewer.RendState(iREND_FLATSHADE);
		}

        return iRC_OK;
    }
    else
    {
        strcpy (tFilename, tTextureFilename);
    }

    if ((rc = mdlMd2.LoadTexture (tFilename)) != iRC_OK)
    {
        EnableRendMenuItem (iREND_TEXTURED, FALSE);

		uMask = engViewer.RendMask();
		engViewer.RendMask (uMask & iMASK_TEXTURED);

		if (engViewer.RendState() == iREND_TEXTURED)
		{
	        engViewer.RendState(iREND_FLATSHADE);
		}
    }
    else
    {
        strcpy (mdlMd2.m_skinname, tFilename);
        EnableRendMenuItem (iREND_TEXTURED, TRUE);

		uMask = engViewer.RendMask();
		engViewer.RendMask (uMask | iREND_TEXTURED);
    }

    return iRC_OK;
} // LoadModel

// ===========================================================================
// Name.......: Resize()
// Description:	This function resizes the DIB to match the window client area.
// Parameters.: hWnd					- handle to the window
//				iInWidth				- the width of the new window
//				iInHeight				- the height of the new window
// Returns....: UINT					- a return code
// ===========================================================================
UINT
Resize (HDC hDC, UINT iRendState, int iInWidth, int iInHeight)
{
    UINT	rc;
    int		iNewWidth;
    int		iNewHeight;

    // keep the dimensions DWORD aligned
    iNewWidth = (iInWidth+3) & ~3;
    if (iNewWidth < 10)
    {
        iNewWidth = 10;
    }

    iNewHeight = (iInHeight+3) & ~3;
    if (iNewHeight < 10)
    {
        iNewHeight = 10;
    }

    // delete the old DIB
    GdiFlush();
    SelectObject (hDC, hDIBSection);
    DeleteObject (hDIBSection);

	if (iRendState == iREND_TEXTURED)
	{
		if ((rc = CreateCompatibleDIB (hDC, iNewWidth, iNewHeight))
				!= iRC_OK)
		{
			return rc;
		}
	}
	else
	{
		if ((rc = CreateGreyscaleDIB (hDC, iNewWidth, iNewHeight))
				!= iRC_OK)
		{
			return rc;
		}
	}

    rectClip.left = 0;
    rectClip.top = 0;
    rectClip.right = iDIBWidth;
    rectClip.bottom = iDIBHeight;

    return iRC_OK;
} // Resize

// ===========================================================================
// Name.......: SwitchToColor()
// Description:	This function switches the DIB to color
// Parameters.: hWnd					- handle to the window
// Returns....: UINT					- a return code
// ===========================================================================
UINT
SwitchToColor (HDC hDC)
{
    UINT	rc;

    // delete the old DIB
    GdiFlush();
    SelectObject (hDC, hDIBSection);
    DeleteObject (hDIBSection);

    // create the new DIB
    if ((rc = CreateCompatibleDIB (hDC, iDIBWidth, iDIBHeight))
            != iRC_OK)
    {
        return rc;
    }

    return iRC_OK;
} // SwitchToColor

// ===========================================================================
// Name.......: SwitchToGreyscale()
// Description:	This function switches the DIB to greyscale
// Parameters.: hWnd					- handle to the window
// Returns....: UINT					- a return code
// ===========================================================================
UINT
SwitchToGreyscale (HDC hDC)
{
    UINT	rc;

    // delete the old DIB
    GdiFlush();
    SelectObject (hDC, hDIBSection);
    DeleteObject (hDIBSection);

    // create the new DIB
    if ((rc = CreateGreyscaleDIB (hDC, iDIBWidth, iDIBHeight))
            != iRC_OK)
    {
        return rc;
    }

    return iRC_OK;
} // SwitchToGreyscale

// ===========================================================================
// Name.......: GetIntensity()
// Description:	This function returns the current intensity.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
static DWORD
GetIntensity ()
{
    return dwIntensity;
} // GetIntensity

// ===========================================================================
// Name.......: SetIntensity()
// Description:	This function calculates the lighting intensity of the given
//				triangle.
// Parameters.: P1						- the first point
//				P2						- the second point
//				P3						- the third point
// Returns....: NIL
// ===========================================================================
static void
SetIntensity (const Vec3D &un, const POINT_3D &P0)
{
    double		I;			// total intensity of light
    double		Ia;			// ambient intensity of light
    double		Id;			// diffuse intensity of light
    double		Is;			// specular intensity of light
    POINT_3D	ptLight;	// location of the light source
//    Vec3D		un;			// normalized vector for normal to triangle
    Vec3D		ur;			// normalized vector for reflection direction
    Vec3D		us;			// normalized vector from pt to the light source
    Vec3D		uv;			// normalized vector from the point to the eye

    ptLight.x = 0;
    ptLight.y = 0;
    ptLight.z = -300;

    Ia = 2.0;

    us.DiffPt(ptLight, P0);
    us.Normalize();

//    un.NormalPt(P0, P1, P2);
//    un.Normalize();

    if (Dot(un,us) < 0)
    {
        I = Ia * ra;
    }
    else
    {
        Id = rd * Dot(un,us);

        ur = (Vec3D) un * 2.0 * Dot(us,un) - us;
        ur.Normalize();

        uv.DiffPt(ptLight, P0);
        uv.Normalize();

        Is = 0.2;

        if (Dot(ur,uv) < 0)
        {
            I = Ia * ra + Is * Id;
        }
        else
        {
            I = Ia * ra + Is * (Id + rs * Dot(ur,uv));
        }
    }

    // determined experimentally
    dwIntensity = (DWORD) (I / dMax * dNUM_LEVELS);
} // SetIntensity

// ===========================================================================
// Name.......: CopyPCXPixels()
// Description:	This function copys pixels from a PCX pixel array to the
//				given array.
// Parameters.: pGiven					- the given byte array
//				iPitch					- the pitch of the given surface
//				pcxTex					- a pointer to the texture map
// Returns....: NIL
// ===========================================================================
static void
CopyPCXPixels (BYTE *pGiven, int iPitch, PCXBitmap &pcxTex)
{
    BYTE			*pSrc;
    int				i;

    pSrc = pcxTex.m_pPixels;
    for (i=0; i < pcxTex.m_iHeight; i++)
    {
        memcpy ((void *)pGiven, (void *)pSrc, pcxTex.m_iWidth * sizeof(BYTE));
        pGiven += iPitch;
        pSrc += pcxTex.m_iWidth;
    }
} // CopyPCXPixels

// ===========================================================================
// Name.......: CreateCompatibleDIB()
// Description:	This function creates a DIB compatible with the given device
//				context.
// Parameters.: hDC						- the given device context
//				iInWidth				- the desired width
//				iInHeight				- the desired height
// Returns....: UINT					- a return code
// ===========================================================================
static UINT
CreateCompatibleDIB (HDC hDC, int iInWidth, int iInHeight)
{
    BITMAPINFO		*pbmiDIB;
    BYTE			*ppvBits;

    // validate hDC
    if (GetObjectType(hDC) != OBJ_DC)
    {
        // hDC is not a valid DC
        return iERR_RENDER_DC;
    }

    pbmiDIB = (BITMAPINFO *)
              malloc(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));
    if (pbmiDIB == NULL)
    {
        // unable to allocate memory for bitmap
        return iERR_RENDER_ALLOC_DIB;
    }

    pbmiDIB->bmiHeader.biSize               = sizeof(BITMAPINFOHEADER);
    pbmiDIB->bmiHeader.biWidth              = iInWidth;
    pbmiDIB->bmiHeader.biHeight             = iInHeight;
    pbmiDIB->bmiHeader.biPlanes             = 1;
    pbmiDIB->bmiHeader.biCompression        = BI_RGB;
    pbmiDIB->bmiHeader.biBitCount           = 8;
    pbmiDIB->bmiHeader.biSizeImage          = 0;
    pbmiDIB->bmiHeader.biXPelsPerMeter      = 0;
    pbmiDIB->bmiHeader.biYPelsPerMeter      = 0;
    pbmiDIB->bmiHeader.biClrUsed            = 0;
    pbmiDIB->bmiHeader.biClrImportant       = 0;

    // set the palette
    Set256Palette (pbmiDIB->bmiColors);

    hDIBSection = CreateDIBSection(hDC,
                                   pbmiDIB,
                                   DIB_RGB_COLORS,
                                   (VOID **) &ppvBits,
                                   NULL,
                                   0);

    GdiFlush();

    if (! hDIBSection)
    {
        // unable to create a DIBSection
        free (pbmiDIB);
        return iERR_RENDER_DIBSECTION;
    }

    if (pbmiDIB->bmiHeader.biHeight > 0)
    {
        // bottom-up
        pDIB = (ppvBits + (iInHeight - 1) * iInWidth);
        iDIBPitch = -iInWidth;
    }
    else
    {
        // top-down
        pDIB = ppvBits;
        iDIBPitch = iInWidth;
    }

    // clear the DIB
    memset ((void *)ppvBits, 0, iInWidth * iInHeight);

    iDIBWidth = iInWidth;
    iDIBHeight = iInHeight;

    free (pbmiDIB);
    return iRC_OK;
} // CreateCompatibleDIB

// ===========================================================================
// Name.......: CreateGreyscaleDIB()
// Description:	This function creates a DIB compatible with the given device
//				context.
// Parameters.: hDC						- the given device context
//				iInWidth				- the desired width
//				iInHeight				- the desired height
// Returns....: UINT					- a return code
// ===========================================================================
static UINT
CreateGreyscaleDIB (HDC hDC, int iInWidth, int iInHeight)
{
    BITMAPINFO		*pbmiDIB;
    BYTE			*ppvBits;

    // validate hDC
    if (GetObjectType(hDC) != OBJ_DC)
    {
        // hDC is not a valid DC
        return iERR_RENDER_DC;
    }

    pbmiDIB = (BITMAPINFO *)
              malloc(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));
    if (pbmiDIB == NULL)
    {
        // unable to allocate memory for bitmap
        return iERR_RENDER_ALLOC_DIB;
    }

    pbmiDIB->bmiHeader.biSize               = sizeof(BITMAPINFOHEADER);
    pbmiDIB->bmiHeader.biWidth              = iInWidth;
    pbmiDIB->bmiHeader.biHeight             = iInHeight;
    pbmiDIB->bmiHeader.biPlanes             = 1;
    pbmiDIB->bmiHeader.biCompression        = BI_RGB;
    pbmiDIB->bmiHeader.biBitCount           = 8;
    pbmiDIB->bmiHeader.biSizeImage          = 0;
    pbmiDIB->bmiHeader.biXPelsPerMeter      = 0;
    pbmiDIB->bmiHeader.biYPelsPerMeter      = 0;
    pbmiDIB->bmiHeader.biClrUsed            = 0;
    pbmiDIB->bmiHeader.biClrImportant       = 0;

    // set the palette
    SetGreyscalePalette (pbmiDIB->bmiColors);

    hDIBSection = CreateDIBSection(hDC,
                                   pbmiDIB,
                                   DIB_RGB_COLORS,
                                   (VOID **) &ppvBits,
                                   NULL,
                                   0);

    GdiFlush();

    if (! hDIBSection)
    {
        // unable to create a DIBSection
        free (pbmiDIB);
        return iERR_RENDER_DIBSECTION;
    }

    if (pbmiDIB->bmiHeader.biHeight > 0)
    {
        // bottom-up
        pDIB = (ppvBits + (iInHeight - 1) * iInWidth);
        iDIBPitch = -iInWidth;
    }
    else
    {
        // top-down
        pDIB = ppvBits;
        iDIBPitch = iInWidth;
    }

    // clear the DIB
    memset ((void *)ppvBits, 0, iInWidth * iInHeight);

    iDIBWidth = iInWidth;
    iDIBHeight = iInHeight;

    free (pbmiDIB);
    return iRC_OK;
} // CreateGreyscaleDIB

// ===========================================================================
// Name.......: FillDIB()
// Description:	This function clears pixels in the given byte array.
// Parameters.: pGiven					- the given byte array
//				value					- the value to use as "fill"
// Returns....: NIL
// ===========================================================================
static void
FillDIB (BYTE *pGiven, DWORD value)
{
    int				i;
    int				iNewWidth;

    iNewWidth =	iDIBWidth>>2;

    for (i=0; i < iDIBHeight; i++)
    {
        memset ((void *)pGiven, value, iNewWidth * sizeof(DWORD));
        pGiven += iDIBPitch;
    }
} // FillDIB

// ===========================================================================
// Name.......: FindFirstSkinTexture()
// Description:	This function returns the name of the first skin texture in
//				the given directory.
// Parameters.: sFilename				- given directory; returned filename
// Returns....: UINT					- a return code
// ===========================================================================
static UINT
    FindFirstSkinTexture (LPTSTR tModelFilename, LPTSTR tSkinFilename)
{
    LPWIN32_FIND_DATA	lpFindFileData;
    WIN32_FIND_DATA		fdFindData;
    LPSTR				pStr;

    lpFindFileData = &fdFindData;

    pStr = tModelFilename + strlen(tModelFilename) - 1;
    while (pStr != tModelFilename && *pStr != '\\')
    {
        pStr--;
    }
    *pStr = '\0';
    strcat (tModelFilename, "\\*_i.pcx");

    if (FindFirstFile (tModelFilename, lpFindFileData) == INVALID_HANDLE_VALUE)
    {
        return iERR_RENDER_FIND_FIRST;
    }

    strcpy (tSkinFilename, tModelFilename);
    pStr = tSkinFilename + strlen(tSkinFilename) - 1;
    while (pStr != tSkinFilename && *pStr != '\\')
    {
        pStr--;
    }

    if (*pStr == '\\')
    {
        *(pStr+1) = '\0';
    }
    else
    {
        *pStr = '\0';
    }
    strcat (tSkinFilename, lpFindFileData->cFileName);

    pStr = tSkinFilename + strlen(tSkinFilename) - 1;
    while (*pStr != '_')
    {
        pStr--;
    }

    *pStr = '\0';

    strcat (tSkinFilename, ".pcx");

    return iRC_OK;
} // FindFirstSkinTexture

// ===========================================================================
// Name.......: IsBackface()
// Description:	This function determines if a polygon is a backface is
//				therefore not visible. It returns 0 if the polygon is
//              visible, 1 if not visible. Note: this only works for a
//              convex model.
// Parameters.: POINT *pPoints	-	array of 2D vertices
// Returns....: int				-	boolean for backface
// ===========================================================================
static int
IsBackface (triangle_t pPoints[])
{
    int z;

    z = (pPoints[0].x - pPoints[1].x) * (pPoints[2].y - pPoints[1].y) -
        (pPoints[0].y - pPoints[1].y) * (pPoints[2].x - pPoints[1].x);

    return (z > 0);
} // IsBackface
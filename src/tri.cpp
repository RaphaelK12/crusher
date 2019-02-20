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
// File:       tri.cpp
//
// Written:    06 Jul. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This module contains code to rasterize triangles. Part of this code
// originally appeared as part of Microsoft's DirectX sample source code.
// ===========================================================================
#include "stdafx.h"
#include "fixed.h"
#include "defs.h"
#include "3dmath.h"
#include "pcx.h"
#include "tri.h"

#define SWAP(x,y)   ((x)^=(y)^=(x)^=(y))
//SWAP has a compiler generated problem on alpha.... do it the old fashioned way:
#define SWAPPT(a,b) {POINT_2D p; p=a;a=b;b=p;} //(SWAP(a.y,b.y), SWAP(a.x,b.x))

//#define SWAPTRIPT(a,b) {triangle_t p; p.x=a.x; p.y=a.y; p.s=a.s; p.t=a.t; a.x=b.x; a.y=b.y; a.s=b.s; a.t=b.t; b.x=p.x; b.y=p.y; b.s=p.s; b.t=p.t;}
#define SWAPTRIPT(a,b) {triangle_t t; t=a; a=b; b=t;}

#define Abs(a)  ((a < 0) ? -a : a)
#define Sgn(a)  ((a < 0) ? -1 : 1)

typedef struct
{
        POINT_2D pt1;
        POINT_2D pt2;
} segment_t;

typedef struct
{
        int num;
        triangle_t pt[6];
} polypoint_t;

inline void       FillScan8		(BYTE *, int, int, DWORD);
inline void       FillTexScan8	(BYTE *, int, int, BYTE *, int, int, int, int, int);
static triangle_t FindIntersect	(triangle_t, triangle_t, segment_t);
static BOOL       IsInside		(triangle_t, segment_t);
static void       SHClip		(const polypoint_t *, polypoint_t *, segment_t);
static void       Line			(BYTE *, int, POINT_2D, POINT_2D, int);

// ===========================================================================
// Name.......: ClippedFlatShadedTri8()
// Description:	This function clips and rasterizes a flat shaded triangle
//				into a 8bpp memory buffer.
// Parameters.: p					- the given byte array
//				next_scan			- the pitch
//				pTriPts				- an array of triangle points
//				pRect	 			- pointer to the clipping rectangle
// Returns....: NIL
// ===========================================================================
void
ClippedFlatShadedTri8(BYTE *p, int next_scan, const triangle_t *pTriPts,
                      DWORD value, const RECT *pRect)
{
    polypoint_t		polyNew;
    polypoint_t		polyTri;
    segment_t		segBoundary;
    POINT_2D		P0;
    POINT_2D		P1;
    POINT_2D		P2;
    int				i;

    // copy triangle into polyTri
    polyTri.num = 3;
    memcpy (&polyTri.pt[0], &pTriPts[0], sizeof(triangle_t));
    memcpy (&polyTri.pt[1], &pTriPts[1], sizeof(triangle_t));
    memcpy (&polyTri.pt[2], &pTriPts[2], sizeof(triangle_t));

    // left
    segBoundary.pt1.x = pRect->left;
    segBoundary.pt1.y = pRect->top;

    segBoundary.pt2.x = pRect->left;
    segBoundary.pt2.y = pRect->bottom-1;

    SHClip (&polyTri, &polyNew, segBoundary);
    memcpy (&polyTri, &polyNew, sizeof(polypoint_t));

    // top
    segBoundary.pt1.x = pRect->right - 1;
    segBoundary.pt1.y = pRect->top;

    segBoundary.pt2.x = pRect->left;
    segBoundary.pt2.y = pRect->top;

    SHClip (&polyTri, &polyNew, segBoundary);
    memcpy (&polyTri, &polyNew, sizeof(polypoint_t));

    // right
    segBoundary.pt1.x = pRect->right - 1;
    segBoundary.pt1.y = pRect->bottom - 1;

    segBoundary.pt2.x = pRect->right - 1;
    segBoundary.pt2.y = pRect->top;

    SHClip (&polyTri, &polyNew, segBoundary);
    memcpy (&polyTri, &polyNew, sizeof(polypoint_t));

    // bottom
    segBoundary.pt1.x = pRect->left;
    segBoundary.pt1.y = pRect->bottom - 1;

    segBoundary.pt2.x = pRect->right - 1;
    segBoundary.pt2.y = pRect->bottom - 1;

    SHClip (&polyTri, &polyNew, segBoundary);
    memcpy (&polyTri, &polyNew, sizeof(polypoint_t));

    for (i=0; i < polyTri.num-2; i++)
    {
        P0.x = polyTri.pt[0].x;
        P0.y = polyTri.pt[0].y;
        P1.x = polyTri.pt[i+1].x;
        P1.y = polyTri.pt[i+1].y;
        P2.x = polyTri.pt[i+2].x;
        P2.y = polyTri.pt[i+2].y;

        Triangle8 (p, next_scan, P0, P1, P2, value);
    }
} // ClippedFlatShadedTri8

// ===========================================================================
// Name.......: ClippedTexturedTri8()
// Description:	This function clips and rasterizes a textured triangle into
//				a 8bpp memory buffer.
// Parameters.: p					- the given byte array
//				next_scan			- the pitch
//				pTriPts				- an array of triangle points
//				pcxTex 				- pointer to the texture map
//				pRect	 			- pointer to the clipping rectangle
// Returns....: NIL
// ===========================================================================
void
ClippedTexturedTri8(BYTE *p, int next_scan, const triangle_t *pTriPts,
                    const PCXBitmap &pcxTex, const RECT *pRect)
{
    polypoint_t		polyNew;
    polypoint_t		polyTri;
    segment_t		segBoundary;
    triangle_t		triTemp[3];
    int				i;

    // copy triangle into polyTri
    polyTri.num = 3;
    memcpy (&polyTri.pt[0], &pTriPts[0], sizeof(triangle_t));
    memcpy (&polyTri.pt[1], &pTriPts[1], sizeof(triangle_t));
    memcpy (&polyTri.pt[2], &pTriPts[2], sizeof(triangle_t));

    // left
    segBoundary.pt1.x = pRect->left;
    segBoundary.pt1.y = pRect->top;

    segBoundary.pt2.x = pRect->left;
    segBoundary.pt2.y = pRect->bottom;

    SHClip (&polyTri, &polyNew, segBoundary);
    memcpy (&polyTri, &polyNew, sizeof(polypoint_t));

    // top
    segBoundary.pt1.x = pRect->right;
    segBoundary.pt1.y = pRect->top;

    segBoundary.pt2.x = pRect->left;
    segBoundary.pt2.y = pRect->top;

    SHClip (&polyTri, &polyNew, segBoundary);
    memcpy (&polyTri, &polyNew, sizeof(polypoint_t));

    // right
    segBoundary.pt1.x = pRect->right;
    segBoundary.pt1.y = pRect->bottom;

    segBoundary.pt2.x = pRect->right;
    segBoundary.pt2.y = pRect->top;

    SHClip (&polyTri, &polyNew, segBoundary);
    memcpy (&polyTri, &polyNew, sizeof(polypoint_t));

    // bottom
    segBoundary.pt1.x = pRect->left;
    segBoundary.pt1.y = pRect->bottom;

    segBoundary.pt2.x = pRect->right;
    segBoundary.pt2.y = pRect->bottom;

    SHClip (&polyTri, &polyNew, segBoundary);
    memcpy (&polyTri, &polyNew, sizeof(polypoint_t));


    for (i=0; i < polyTri.num-2; i++)
    {
        memcpy (&triTemp[0], &polyTri.pt[0], sizeof(triangle_t));
        memcpy (&triTemp[1], &polyTri.pt[i+1], sizeof(triangle_t));
        memcpy (&triTemp[2], &polyTri.pt[i+2], sizeof(triangle_t));

        TexturedTri8 (p, next_scan, triTemp, pcxTex);
    }
} // ClippedTexturedTri8

// ===========================================================================
// Name.......: ClippedWireframeTri8()
// Description:	This function clips and rasterizes a wireframe triangle into
//				a 8bpp memory buffer.
// Parameters.: p					- the given byte array
//				next_scan			- the pitch
//				pTriPts				- an array of triangle points
//				pcxTex 				- pointer to texture map
//				pRect	 			- pointer to the clipping rectangle
// Returns....: NIL
// ===========================================================================
void
ClippedWireframeTri8(BYTE *p, int next_scan, const triangle_t *pTriPts,
                     DWORD value, const RECT *pRect)
{
    polypoint_t		polyNew;
    polypoint_t		polyTri;
    segment_t		segBoundary;
    POINT_2D		P0;
    POINT_2D		P1;
    int				i;

    // copy triangle into polyTri
    polyTri.num = 3;
    memcpy (&polyTri.pt[0], &pTriPts[0], sizeof(triangle_t));
    memcpy (&polyTri.pt[1], &pTriPts[1], sizeof(triangle_t));
    memcpy (&polyTri.pt[2], &pTriPts[2], sizeof(triangle_t));

    // left
    segBoundary.pt1.x = pRect->left;
    segBoundary.pt1.y = pRect->top;

    segBoundary.pt2.x = pRect->left;
    segBoundary.pt2.y = pRect->bottom-1;

    SHClip (&polyTri, &polyNew, segBoundary);
    memcpy (&polyTri, &polyNew, sizeof(polypoint_t));

    // top
    segBoundary.pt1.x = pRect->right - 1;
    segBoundary.pt1.y = pRect->top;

    segBoundary.pt2.x = pRect->left;
    segBoundary.pt2.y = pRect->top;

    SHClip (&polyTri, &polyNew, segBoundary);
    memcpy (&polyTri, &polyNew, sizeof(polypoint_t));

    // right
    segBoundary.pt1.x = pRect->right - 1;
    segBoundary.pt1.y = pRect->bottom - 1;

    segBoundary.pt2.x = pRect->right - 1;
    segBoundary.pt2.y = pRect->top;

    SHClip (&polyTri, &polyNew, segBoundary);
    memcpy (&polyTri, &polyNew, sizeof(polypoint_t));

    // bottom
    segBoundary.pt1.x = pRect->left;
    segBoundary.pt1.y = pRect->bottom - 1;

    segBoundary.pt2.x = pRect->right - 1;
    segBoundary.pt2.y = pRect->bottom - 1;

    SHClip (&polyTri, &polyNew, segBoundary);
    memcpy (&polyTri, &polyNew, sizeof(polypoint_t));

    for (i=0; i < polyTri.num-1; i++)
    {
        P0.x = polyTri.pt[i].x;
        P0.y = polyTri.pt[i].y;
        P1.x = polyTri.pt[i+1].x;
        P1.y = polyTri.pt[i+1].y;

        Line (p, next_scan, P0, P1, iWIREFRAME_TRI_COLOR);
    }

    if (polyTri.num > 0)
    {
        P0.x = polyTri.pt[polyTri.num-1].x;
        P0.y = polyTri.pt[polyTri.num-1].y;
        P1.x = polyTri.pt[0].x;
        P1.y = polyTri.pt[0].y;
        Line (p, next_scan, P0, P1, iWIREFRAME_TRI_COLOR);
    }
} // ClippedWireframeTri8

// ===========================================================================
// Name.......: Point8()
// Description:	This function rasterizes a solid color point into a 8bpp
//				memory buffer. This code does clipping.
// Parameters.: p					- the given byte array
//				P0					- the point
//				value				- the given color
//				pRect				- pointer to the clipping triangle
// Returns....: NIL
// ===========================================================================
void
Point8 (BYTE *p, int next_scan, POINT_2D P0, DWORD value, const RECT *pRect)
{
    register int	x;
    register int	y;

	if (P0.x <= pRect->left)
	{
		return;
	}

	if (P0.x >= pRect->right)
	{
		return;
	}

	if (P0.y <= pRect->top)
	{
		return;
	}

	if (P0.y >= pRect->bottom)
	{
		return;
	}

    x = (int) P0.x;
    y = (int) P0.y * next_scan;

    *(p + x + y) = (BYTE) value;
} // Point8

// ===========================================================================
// Name.......: Triangle8()
// Description:	This function rasterizes a solid color triangle into a 8bpp
//				memory buffer. This code does no clipping. It rasterizes a
//				solid color triangle into a memory buffer with any pitch.
//				Also, it is careful to always write DWORD aligned. So it is
//				safe to be used on video memory. Original author: Microsoft
// Parameters.: p					- the given byte array
//				next_scan			- the pitch
//				P0					- the first triangle vertex
//				P0					- the second triangle vertex
//				P0					- the third triangle vertex
//				c					- the given color
// Returns....: NIL
// ===========================================================================
void
Triangle8(BYTE *p, int next_scan, POINT_2D P0, POINT_2D P1, POINT_2D P2, DWORD c)
{
    Fixed   d,d0;
    Fixed   x,x0;
    int     y;

    // expand the color to a DWORD
    c |= c<<8;
    c |= c<<16;

    //  sort points so P0.y <= P1.y <= P2.y
    if (P0.y > P1.y)
    {
        SWAPPT(P0,P1);
    }

    if (P1.y > P2.y)
    {
        SWAPPT(P1,P2);
    }

    if (P0.y > P1.y)
    {
        SWAPPT(P0,P1);
    }

    //  check for quick out?
    if (P2.y - P0.y == 0)
    {
        return;
    }

    //  compute "long" side walk from P0 to P2
    d = (Fixed)(P2.x - P0.x) / (Fixed)(P2.y - P0.y);

    x  = P0.x;
    y  = P0.y;
    p += P0.y * next_scan;   // point p to correct scan.

    //  do the top
    if (P0.y < P1.y)
    {
        d0 = (Fixed)(P1.x - P0.x) / (Fixed)(P1.y - P0.y);
        x0 = P0.x;

        // check for left or right fill
        if (d < d0)
        {
            while (y < P1.y)
            {
                FillScan8(p, x, x0, c);
                y++;
                p  += next_scan;
                x  += d;
                x0 += d0;
            }
        }
        else
        {
            while (y < P1.y)
            {
                FillScan8(p, x0, x, c);
                y++;
                p  += next_scan;
                x  += d;
                x0 += d0;
            }
        }
    }

    // do the bottom.
    if (P2.y - P1.y == 0)
    {
        return;
    }

    d0 = (Fixed)(P2.x - P1.x) / (Fixed)(P2.y - P1.y);
    x0 = P1.x;

    // check for left or right fill
    if (x < x0)
    {
        while (y < P2.y)
        {
            FillScan8(p, x, x0, c);
            y++;
            p  += next_scan;
            x  += d;
            x0 += d0;
        }
    }
    else
    {
        while (y < P2.y)
        {
            FillScan8(p, x0, x, c);
            y++;
            p  += next_scan;
            x  += d;
            x0 += d0;
        }
    }
} // Triangle8

// ===========================================================================
// Name.......: TexturedTri8()
// Description:	This function rasterizes a textured triangle into a 8bpp
//				memory buffer. This code does no clipping.
// Parameters.: p					- the given byte array
//				next_scan			- the pitch
//				pTriPts				- an array of triangle points
//				pcxTex			- pointer to texture map
// Returns....: NIL
// ===========================================================================
void
TexturedTri8(BYTE *p, int next_scan, const triangle_t *pTriPts,
             const PCXBitmap &pcxTex)
{
    BYTE		*pTex;
    Fixed		d, d0;
    Fixed		s, s0;
    Fixed		t, t0;
    Fixed		us, us0;
    Fixed		ut, ut0;
    Fixed		x, x0;
    int			iTexWidth;
    int			y;
    triangle_t	P0, P1, P2;

    iTexWidth	= pcxTex.m_iWidth;
    pTex		= pcxTex.m_pPixels;

    P0.x = pTriPts[0].x;
    P0.y = pTriPts[0].y;
    P0.s = pTriPts[0].s;
    P0.t = pTriPts[0].t;

    P1.x = pTriPts[1].x;
    P1.y = pTriPts[1].y;
    P1.s = pTriPts[1].s;
    P1.t = pTriPts[1].t;

    P2.x = pTriPts[2].x;
    P2.y = pTriPts[2].y;
    P2.s = pTriPts[2].s;
    P2.t = pTriPts[2].t;

    //  sort points so P0.y <= P1.y <= P2.y
    if (P0.y > P1.y)
    {
        SWAPTRIPT(P0,P1);
    }

    if (P1.y > P2.y)
    {
        SWAPTRIPT(P1,P2);
    }

    if (P0.y > P1.y)
    {
        SWAPTRIPT(P0,P1);
    }

    //  check for quick out?
    if (P2.y - P0.y == 0)
    {
        return;
    }

    //  compute "long" side walk from P0 to P2
    d  = (Fixed)(P2.x - P0.x) / (Fixed)(P2.y - P0.y);
    us = (Fixed)(P2.s - P0.s) / (Fixed)(P2.y - P0.y);
    ut = (Fixed)(P2.t - P0.t) / (Fixed)(P2.y - P0.y);

    x  = P0.x;
    s  = P0.s;
    t  = P0.t;

    y  = P0.y;
    p += P0.y * next_scan;   // point p to correct scan.

    //  do the top
    if (P0.y < P1.y)
    {
        d0 = (Fixed)(P1.x - P0.x) / (Fixed)(P1.y - P0.y);
        x0 = P0.x;
        us0 = (Fixed)(P1.s - P0.s) / (Fixed)(P1.y - P0.y);
        s0 = P0.s;
        ut0 = (Fixed)(P1.t - P0.t) / (Fixed)(P1.y - P0.y);
        t0 = P0.t;

        while (y < P1.y)
        {
            FillTexScan8(p, x, x0, pTex, s, s0, t, t0, iTexWidth);
            y++;
            p  += next_scan;
            x  += d;
            x0 += d0;

            s  += us;
            s0 += us0;
            t  += ut;
            t0 += ut0;
        }
    }

    // do the bottom.
    if (P2.y - P1.y == 0)
    {
        return;
    }

    d0 = (Fixed)(P2.x - P1.x) / (Fixed)(P2.y - P1.y);
    x0 = P1.x;
    us0 = (Fixed)(P2.s - P1.s) / (Fixed)(P2.y - P1.y);
    s0 = P1.s;
    ut0 = (Fixed)(P2.t - P1.t) / (Fixed)(P2.y - P1.y);
    t0 = P1.t;

    // check for left or right fill
    while (y < P2.y)
    {
        FillTexScan8(p, x, x0, pTex, s, s0, t, t0, iTexWidth);
        y++;
        p  += next_scan;
        x  += d;
        x0 += d0;

        s  += us;
        s0 += us0;
        t  += ut;
        t0 += ut0;
    }
} // TexturedTri8

// ===========================================================================
// Name.......: WireframeTri8()
// Description:	This function rasterizes a wireframe triangle into a 8bpp
//				memory buffer. This code does no clipping. It rasterizes a
//				solid color triangle into a memory buffer with any pitch.
//				Also, it is careful to always write DWORD aligned. So it is
//				safe to be used on video memory. No anti-aliasing is done.
// Parameters.: p					- the given byte array
//				next_scan			- the pitch
//				P0					- the first triangle vertex
//				P1					- the second triangle vertex
//				P2					- the third triangle vertex
//				c					- the given color
// Returns....: NIL
// ===========================================================================
void
WireframeTri8(BYTE *p, int next_scan, POINT_2D P0, POINT_2D P1, POINT_2D P2, DWORD c)
{
    Line (p, next_scan, P0, P1, c);
    Line (p, next_scan, P1, P2, c);
    Line (p, next_scan, P2, P0, c);
} // WireframeTri8

// ===========================================================================
// Name.......: FillScan8()
// Description:	This function fills a scanline with a given color. It does
//				optimal DWORD writes, making sure to stay aligned so it is
//				safe for video memory. Original author: Microsoft
// Parameters.: p					- the given byte array
//				x0					- start of the scan
//				x1					- end of the scan
//				c					- the given color
// Returns....: NIL
// ===========================================================================
inline void
FillScan8(BYTE *p, int x0, int x1, DWORD c)
{
    int dx;
    int z;

    dx = x1-x0+1;
    p += x0;

    if (dx > 4)
    {
        if (z = (DWORD)p & 0x03)
        {
            while (z++ < 4)
            {
                *p++ = (BYTE)c;
                dx--;
            }
        }

        while (dx >= 4)
        {
            *((DWORD*)p) = c;
            p += 4;
            dx -= 4;
        }
    }

    while (dx-- > 0)
    {
        *p++ = (BYTE)c;
    }
} // FillScan8

// ===========================================================================
// Name.......: FillTexScan8()
// Description:	This function fills a scanline with a texture.
// Parameters.: p					- the given byte array
//				x0					- start of the scan
//				x1					- end of the scan
//				pTex				- pointer to texture map
//				s0					- s coordinate of first texture point
//				s1					- s coordinate of second texture point
//				t0					- t coordinate of first texture point
//				t1					- t coordinate of second texture point
//				next_text_scan		- the pitch of the texture map
// Returns....: NIL
// ===========================================================================
inline void
FillTexScan8(BYTE *p, int x0, int x1, BYTE *pTex, int s0, int s1, int t0,
             int t1, int next_tex_scan)
{
    int		dx;
    int		temp;
    int		x;
    Fixed	s;
    Fixed	t;
    Fixed	ds;
    Fixed	dt;

    if (x0 > x1)
    {
        temp = x0;
        x0 = x1;
        x1 = temp;

        temp = s0;
        s0 = s1;
        s1 = temp;

        temp = t0;
        t0 = t1;
        t1 = temp;
    }

    dx = x1-x0+1;
    p += x0;

    ds = (Fixed)(s1-s0) / (Fixed)dx;
    dt = (Fixed)(t1-t0) / (Fixed)dx;
    s = (Fixed) s0;
    t = (Fixed) t0;

    for (x=x0; x < x1; x++)
    {
        *p++ = (BYTE) pTex [(int)t * next_tex_scan + (int)s];
        s += ds;
        t += dt;
    }
} // FillTexScan8

// ===========================================================================
// Name.......: FindIntersect()
// Description:	This function returns the point of intersection of the
//				given line segment with the line obtained by extending the
//				segment boundary.
// Parameters.: pt0					- the first point on line segment
//				pt1					- the second point on line segment
//				segBoundary			- the segment boundary
// Returns....: triangle_t			- point of intersection
// ===========================================================================
static triangle_t FindIntersect (triangle_t pt0, triangle_t pt1,
                                 segment_t segBoundary)
{
    triangle_t	temp;
    triangle_t	n;
    triangle_t	c;
    LONG		d;
    double		t;

    // TODO: optimize this

    n.x = -(segBoundary.pt2.y - segBoundary.pt1.y);
    n.y = segBoundary.pt2.x - segBoundary.pt1.x;

    d = n.y * segBoundary.pt1.y + n.x * segBoundary.pt1.x;

    c.x = pt1.x - pt0.x;
    c.y = pt1.y - pt0.y;

    t = (double)(d - (n.x * pt0.x + n.y * pt0.y)) / (double)(n.x * c.x + n.y * c.y);

    temp.x = (int) ((double)pt0.x + (double)(pt1.x - pt0.x) * t);
    temp.y = (int) ((double)pt0.y + (double)(pt1.y - pt0.y) * t);
    temp.s = (int) ((double)pt0.s + (double)(pt1.s - pt0.s) * t);
    temp.t = (int) ((double)pt0.t + (double)(pt1.t - pt0.t) * t);

    return temp;
} // FindIntersect

// ===========================================================================
// Name.......: IsInside()
// Description:	This function returns TRUE if the point is on the "in" side
//				of the line defined by the segment boundary. The boundary
//				must be specified so that the "in" side will be on the right
//				as one moves from the first boundary point to the second.
// Parameters.: pt					- the point
//				segBoundary			- the line segment
// Returns....: BOOL				- bool indicating inside
// ===========================================================================
static BOOL IsInside (triangle_t pt, segment_t segBoundary)
{
    long	lDirectionX;
    long	lDirectionY;
    long	lDotProd;
    POINT	ptNormal;

    lDirectionX = segBoundary.pt1.x - segBoundary.pt2.x;
    lDirectionY = segBoundary.pt1.y - segBoundary.pt2.y;

    ptNormal.x = -lDirectionY;
    ptNormal.y = lDirectionX;

    lDotProd = (pt.x - segBoundary.pt1.x) * ptNormal.x
               + (pt.y - segBoundary.pt1.y) * ptNormal.y;

    if (lDotProd < 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
} // IsInside

// ===========================================================================
// Name.......: Line()
// Description:	This function draws a line. No clipping is done.
// Parameters.: p					- the given byte array
//				next_scan			- the pitch of the texture map
//				P0					- the first point
//				P1					- the second point
//				value				- the value of the given color
// Returns....: NIL
// ===========================================================================
static void
    Line (BYTE *p, int next_scan, POINT_2D P0, POINT_2D P1, int value)
{
    register int	d;
    register int	x;
    register int	y;
    register int	ax;
    register int	ay;
    register int	sx;
    register int	sy;
    register int	dx;
    register int	dy;
    register int	x1;
    register int	y1;
    BYTE			bColor;

    bColor = (BYTE) value;

    dx = (int) (P1.x - P0.x);
    ax = Abs(dx) << 1;
    sx = Sgn(dx);

    dy = (int) (P1.y - P0.y);
    ay = Abs(dy) << 1;
    sy = Sgn(dy) * next_scan;

    x = (int) P0.x;
    y = (int) P0.y * next_scan;
    x1 = (int) P1.x;
    y1 = (int) P1.y * next_scan;

    if (ax > ay)
    {
        d = ay - (ax >> 1);
        while (x != x1)
        {
            *(p + x + y) = bColor;
            if (d >= 0)
            {
                y += sy;
                d -= ax;
            }
            x += sx;
            d += ay;
        }
    }
    else
    {
        d = ax - (ay >> 1);
        while (y != y1)
        {
            *(p + x + y) = bColor;
            if (d >= 0)
            {
                x += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
        }
    }
}

// ===========================================================================
// Name.......: SHClip()
// Description:	This function clips the given polygon against the segment
//				using the Sutherland-Hodgeman clipppIng algorithm.
// Parameters.: pIn					- the first point on line segment
//				pOut				- the second point on line segment
//				segBoundary			- the segment boundary
// Returns....: NIL
// ===========================================================================
static void SHClip (const polypoint_t *pIn, polypoint_t *pOut, segment_t segBoundary)
{
    triangle_t	ptEnd;
    triangle_t	ptStart;
    triangle_t	ptIntersect;
    int			j;

    pOut->num = 0;

    ptStart = pIn->pt[pIn->num - 1];

    for (j=0; j < pIn->num; j++)
    {
        ptEnd = pIn->pt[j];

        if (IsInside(ptEnd, segBoundary))
        {
            if (IsInside(ptStart, segBoundary))
            {
                pOut->pt[pOut->num] = ptEnd;
                pOut->num++;
            }
            else
            {
                ptIntersect = FindIntersect(ptEnd, ptStart, segBoundary);
                pOut->pt[pOut->num] = ptIntersect;
                pOut->num++;
                pOut->pt[pOut->num] = ptEnd;
                pOut->num++;
            }
        }
        else if (IsInside(ptStart, segBoundary))
        {
            ptIntersect = FindIntersect (ptEnd, ptStart, segBoundary);
            pOut->pt[pOut->num] = ptIntersect;
            pOut->num++;
        }

        ptStart = ptEnd;
    }
} // SH_Clip
 
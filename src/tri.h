// ===========================================================================
// $Source: d:\\cvs\\master/crusher/src/tri.h,v $
// $Revision: 2.0 $
// $Date: 1998/09/23 00:28:23 $
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
// File:       tri.h
//
// Written:    06 Jul. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//   This header file contains definitions for triangle rasterizer code.
// ===========================================================================
#ifndef __TRI_H
#define __TRI_H

void ClippedFlatShadedTri8	(BYTE *, int, const triangle_t *, DWORD, const RECT *);
void ClippedTexturedTri8	(BYTE *, int, const triangle_t *, const PCXBitmap &, const RECT *);
void ClippedWireframeTri8	(BYTE *, int, const triangle_t *, DWORD, const RECT *);
void Point8					(BYTE *, int, POINT_2D, DWORD, const RECT *);
void Triangle8				(BYTE *, int, POINT_2D, POINT_2D, POINT_2D, DWORD);
void WireframeTri8			(BYTE *, int, POINT_2D, POINT_2D, POINT_2D, DWORD);
void TexturedTri8			(BYTE *, int, const triangle_t *, const PCXBitmap &);

#endif // __TRI_H

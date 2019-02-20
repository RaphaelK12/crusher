// ===========================================================================
// $Source: d:\\cvs\\master/crusher/src/palette.h,v $
// $Revision: 2.3 $
// $Date: 1998/11/15 07:42:33 $
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
// File:       palette.h
//
// Written:    04 Oct. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This is the header file for the 256 color palette functions.
// ===========================================================================
#ifndef __PALETTE_H
#define __PALETTE_H

UINT Realize256Palette (HDC);
UINT Restore256Palette (HDC);
void Set256Palette (RGBQUAD *);
void SetGreyscalePalette (RGBQUAD *);
BOOL Uses256Palette ();
UINT InitPalette (HDC);

#endif // __PALETTE_H

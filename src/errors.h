// ===========================================================================
// $Source: d:\\cvs\\master/crusher/src/errors.h,v $
// $Revision: 2.2 $
// $Date: 1998/11/30 02:28:12 $
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
// File:       errors.h
//
// Written:    16 Jun. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This header file contains error code definition used throughout the
//    program.
// ===========================================================================
#ifndef __ERRORS_H
#define __ERRORS_H

const UINT iRC_OK = TRUE;

// ========================================================================
// Generic Errors
// ========================================================================
#define ERR_GENERIC_BASE		2

// Generic error
const UINT iERR_GENERIC = ERR_GENERIC_BASE + 1;

// Generic memory allocation error
const UINT iERR_GENERIC_ALLOC = ERR_GENERIC_BASE + 2;

// ========================================================================
// Render Errors
// ========================================================================
#define ERR_RENDER_BASE			64

// Unable to get device context
const UINT iERR_RENDER_DC			= ERR_RENDER_BASE + 1;

// Unable to create compatible device context
const UINT iERR_RENDER_COMPAT_DC	= ERR_RENDER_BASE + 2;

// Unable to select bitmap
const UINT iERR_RENDER_BITMAP		= ERR_RENDER_BASE + 3;

// Unable to realize palette
const UINT iERR_RENDER_REALIZE		= ERR_RENDER_BASE + 4;

// Unable to create DIBSection
const UINT iERR_RENDER_DIBSECTION	= ERR_RENDER_BASE + 5;

// Unable to allocate memory for the bitmap
const UINT iERR_RENDER_ALLOC_DIB	= ERR_RENDER_BASE + 6;

// Unable to allocate memory for the palette
const UINT iERR_RENDER_ALLOC_PAL	= ERR_RENDER_BASE + 7;

// No path in registry
const UINT iERR_RENDER_REG			= ERR_RENDER_BASE + 8;

// Unable to find first file
const UINT iERR_RENDER_FIND_FIRST	= ERR_RENDER_BASE + 9;

// Invalid texture path
const UINT iERR_RENDER_TEX_PATH		= ERR_RENDER_BASE + 10;

// Unable to select palette
const UINT iERR_RENDER_PAL_SEL		= ERR_RENDER_BASE + 11;

// Unable to create palette
const UINT iERR_RENDER_PAL_CREATE	= ERR_RENDER_BASE + 12;

// Unable to realize palette
const UINT iERR_RENDER_PAL_REALIZE	= ERR_RENDER_BASE + 13;

// Unable to open registry entry
const UINT iERR_RENDER_REG_OPEN		= ERR_RENDER_BASE + 14;

// Unable to query registry entry
const UINT iERR_RENDER_REG_QUERY	= ERR_RENDER_BASE + 15;

// Unable to open file for writing
const UINT iERR_RENDER_FILE_WRITE	= ERR_RENDER_BASE + 16;

// Unable to find associated browser
const UINT iERR_RENDER_BROWSER		= ERR_RENDER_BASE + 17;

// Unable to open web page
const UINT iERR_RENDER_URL			= ERR_RENDER_BASE + 18;

// ========================================================================
// MD2 Errors
// ========================================================================
#define ERR_MD2_BASE			128

// Error allocating frame info
const UINT iERR_MD2_FRAMELIST		= ERR_MD2_BASE + 1;

// Error allocating gl cmds
const UINT iERR_MD2_GLCMDS			= ERR_MD2_BASE + 2;

// Not a valid model file
const UINT iERR_MD2_INVALIDMODEL	= ERR_MD2_BASE + 3;

// Unable to open file for reading
const UINT iERR_MD2_OPENFILE		= ERR_MD2_BASE + 4;

// Error allocating points
const UINT iERR_MD2_POINTS			= ERR_MD2_BASE + 5;

// Error allocating triangles
const UINT iERR_MD2_TRIS			= ERR_MD2_BASE + 6;

// Error allocating memory for texture coordinates
const UINT iERR_MD2_ST				= ERR_MD2_BASE + 7;

// Invalid md2 header
const UINT iERR_MD2_IDENT			= ERR_MD2_BASE + 8;

// Invalid texture height
const UINT iERR_MD2_TEXHEIGHT		= ERR_MD2_BASE + 9;

// Invalid texture width
const UINT iERR_MD2_TEXWIDTH		= ERR_MD2_BASE + 10;

// Unable to load texture
const UINT iERR_MD2_TEXTURE			= ERR_MD2_BASE + 11;

// Error allocating memory for normals
const UINT iERR_MD2_NORMALS			= ERR_MD2_BASE + 12;

// Error opening data file for writing
const UINT iERR_MD2_OPEN_DATAFILE	= ERR_MD2_BASE + 13;

// Error opening aniv file for writing
const UINT iERR_MD2_OPEN_ANIVFILE	= ERR_MD2_BASE + 14;

// Error opening class file for writing
const UINT iERR_MD2_OPEN_CLASSFILE	= ERR_MD2_BASE + 15;

// ========================================================================
// PCX Errors
// ========================================================================
#define ERR_PCX_BASE			192

// Not a valid device context
const UINT iERR_PCX_DC				= ERR_PCX_BASE + 1;

// Unable to open file for reading
const UINT iERR_PCX_OPENFILE		= ERR_PCX_BASE + 2;

// Not a valid PCX file
const UINT iERR_PCX_IDENTIFIER		= ERR_PCX_BASE + 3;

// Invalid PCX Header
const UINT iERR_PCX_HEADER			= ERR_PCX_BASE + 4;

// Must be an 8 bit PCX file
const UINT iERR_PCX_EIGHTBIT		= ERR_PCX_BASE + 5;

// Invalid PCX version number
const UINT iERR_PCX_VERSION			= ERR_PCX_BASE + 6;

// Invalid palette
const UINT iERR_PCX_PALETTE			= ERR_PCX_BASE + 7;

// Unable to allocate memory for pixel array
const UINT iERR_PCX_PIXEL_ARRAY		= ERR_PCX_BASE + 8;

// Unable to allocate memory for bitmap
const UINT iERR_PCX_BITMAP			= ERR_PCX_BASE + 9;

// ========================================================================
// Unreal Package Errors
// ========================================================================
#define ERR_PKG_BASE			254

// Unable to open file for reading
const UINT iERR_PKG_OPENFILE		= ERR_PKG_BASE + 1;

// Not a valid package file
const UINT iERR_PKG_INVALID_FILE	= ERR_PKG_BASE + 2;

// Version not supported
const UINT iERR_PKG_VERSION			= ERR_PKG_BASE + 3;

// Unable to allocate memory for names
const UINT iERR_PKG_ALLOC_NAMES		= ERR_PKG_BASE + 4;

// Unable to allocate memory for imports
const UINT iERR_PKG_ALLOC_IMPORTS	= ERR_PKG_BASE + 5;

// Unable to allocate memory for exports
const UINT iERR_PKG_ALLOC_EXPORTS	= ERR_PKG_BASE + 6;

// Error reading compact index
const UINT iERR_PKG_COMPACT_INDEX	= ERR_PKG_BASE + 7;


// prototypes
void DisplayErrorMessage (UINT);

#endif // __ERRORS_H

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
// File:       errors.cpp
//
// Written:    06 Jul. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This module contains code to display error messages.
// ===========================================================================
#include "stdafx.h"
#include "defs.h"
#include "errors.h"
#include "3dmath.h"
#include "pcx.h"
#include "md2.h"
#include "crusher.h"

// ===========================================================================
// Name.......: DisplayErrorMessage()
// Description:	This function displays an error message.
// Parameters.: rcGiven					- the given error code
// Returns....: NIL
// ===========================================================================
void
DisplayErrorMessage (UINT rcGiven)
{
    TCHAR lpMsg[1024];

    switch (rcGiven)
    {
    case iERR_GENERIC:
        wsprintf (lpMsg, "Error: An error was encountered.");
        break;

    case iERR_GENERIC_ALLOC:
        wsprintf (lpMsg, "Error: Unable to allocate memory.");
        break;

    case iERR_RENDER_DC:
        wsprintf (lpMsg, "Error: Unable to get device context.");
        break;

    case iERR_RENDER_COMPAT_DC:
        wsprintf (lpMsg, "Error: Unable to create compatible device context.");
        break;

    case iERR_RENDER_BITMAP:
        wsprintf (lpMsg, "Error: Unable to select bitmap.");
        break;

    case iERR_RENDER_REALIZE:
        wsprintf (lpMsg, "Error: Unable to realize palette.");
        break;

    case iERR_RENDER_DIBSECTION:
        wsprintf (lpMsg, "Error: Unable to create DIBSection.");
        break;

    case iERR_RENDER_ALLOC_DIB:
        wsprintf (lpMsg, "Error: Unable to allocate memory for the bitmap.");
        break;

    case iERR_RENDER_ALLOC_PAL:
        wsprintf (lpMsg, "Error: Unable to allocate memory for the palette.");
        break;

    case iERR_RENDER_REG:
        wsprintf (lpMsg, "Error: No path in registry.");
        break;

    case iERR_RENDER_FIND_FIRST:
        wsprintf (lpMsg, "Error: Unable to find first file.");
        break;

    case iERR_RENDER_TEX_PATH:
        wsprintf (lpMsg, "Error: Invalid texture path.");
        break;

    case iERR_RENDER_PAL_SEL:
        wsprintf (lpMsg, "Error: Unable to select palette.");
        break;

    case iERR_RENDER_PAL_CREATE:
        wsprintf (lpMsg, "Error: Unable to create palette.");
        break;

    case iERR_RENDER_PAL_REALIZE:
        wsprintf (lpMsg, "Error: Unable to realize palette.");
        break;

    case iERR_RENDER_REG_OPEN:
        wsprintf (lpMsg, "Error: Unable to open registry entry.");
        break;

    case iERR_RENDER_REG_QUERY:
        wsprintf (lpMsg, "Error: Unable to query registry entry.");
        break;

    case iERR_RENDER_FILE_WRITE:
        wsprintf (lpMsg, "Error: Unable to open file for writing.");
        break;

    case iERR_RENDER_BROWSER:
        wsprintf (lpMsg, "Error: Unable to find associated browser.");
        break;

    case iERR_RENDER_URL:
        wsprintf (lpMsg, "Error: Unable to open web page.");
        break;

    case iERR_MD2_FRAMELIST:
        wsprintf (lpMsg, "Error: Unable to allocate memory for frame information.");
        break;

    case iERR_MD2_GLCMDS:
        wsprintf (lpMsg, "Error: Unable to allocate memory for gl commands.");
        break;

    case iERR_MD2_INVALIDMODEL:
        wsprintf (lpMsg, "Error: This file is not a valid md2 file.");
        break;

    case iERR_MD2_OPENFILE:
        wsprintf (lpMsg, "Error: Unable to open MD2 file for reading");
        break;

    case iERR_MD2_POINTS:
        wsprintf (lpMsg, "Error: Unable to allocate memory for points.");
        break;

    case iERR_MD2_TRIS:
        wsprintf (lpMsg, "Error: Unable to allocate memory for triangles.");
        break;

    case iERR_MD2_ST:
        wsprintf (lpMsg, "Error: Unable to allocate memory for texture coordinates.");
        break;

    case iERR_MD2_IDENT:
        wsprintf (lpMsg, "Error: Invalid md2 header.");
        break;

    case iERR_MD2_TEXHEIGHT:
        wsprintf (lpMsg, "Error: Invalid texture height.");
        break;

    case iERR_MD2_TEXWIDTH:
        wsprintf (lpMsg, "Error: Invalid texture width.");
        break;

    case iERR_MD2_TEXTURE:
        wsprintf (lpMsg, "Error: Unable to load texture.");
        break;

    case iERR_MD2_NORMALS:
        wsprintf (lpMsg, "Error: Error allocating memory for normals.");
        break;

	case iERR_MD2_OPEN_DATAFILE:
        wsprintf (lpMsg, "Error: Error opening data file for writing.");
		break;

	case iERR_MD2_OPEN_ANIVFILE:
        wsprintf (lpMsg, "Error: Error opening aniv file for writing.");
		break;

	case iERR_MD2_OPEN_CLASSFILE:
        wsprintf (lpMsg, "Error: Error opening class file for writing.");
		break;

    case iERR_PCX_DC:
        wsprintf (lpMsg, "Error: Not a valid device context.");
        break;

    case iERR_PCX_BITMAP:
        wsprintf (lpMsg, "Error: Unable to allocate memory for bitmap.");
        break;

    case iERR_PCX_OPENFILE:
        wsprintf (lpMsg, "Error: Unable to open PCX file for reading.");
        break;

    case iERR_PCX_IDENTIFIER:
        wsprintf (lpMsg, "Error: Not a valid PCX file.");
        break;

    case iERR_PCX_HEADER:
        wsprintf (lpMsg, "Error: Invalid PCX header.");
        break;

    case iERR_PCX_EIGHTBIT:
        wsprintf (lpMsg, "Error: Must be an 8-bit PCX file.");
        break;

    case iERR_PCX_VERSION:
        wsprintf (lpMsg, "Error: Invalid PCX version number.");
        break;

    case iERR_PCX_PALETTE:
        wsprintf (lpMsg, "Error: Invalid PCX palette.");
        break;

    case iERR_PCX_PIXEL_ARRAY:
        wsprintf (lpMsg, "Error: Unable to allocate memory for pixel array.");
        break;

    case iERR_PKG_OPENFILE:
        wsprintf (lpMsg, "Error: Unable to open file for reading.");
        break;

    case iERR_PKG_INVALID_FILE:
        wsprintf (lpMsg, "Error: Not a valid package file.");
        break;

    case iERR_PKG_VERSION:
        wsprintf (lpMsg, "Error: This package file version is not supported.");
        break;

    case iERR_PKG_ALLOC_NAMES:
        wsprintf (lpMsg, "Error: Unable to allocate memory for names.");
        break;

    case iERR_PKG_ALLOC_IMPORTS:
        wsprintf (lpMsg, "Error: Unable to allocate memory for imports.");
        break;

    case iERR_PKG_ALLOC_EXPORTS:
        wsprintf (lpMsg, "Error: Unable to allocate memory for exports.");
        break;

    case iERR_PKG_COMPACT_INDEX:
        wsprintf (lpMsg, "Error: Error reading compact index.");
        break;

        default:
        wsprintf (lpMsg, "Error: Unknown Error. Err = %d.", rcGiven);
        break;
    }

    OutputDebugString (lpMsg);
    DisplayMessage (lpMsg);
}
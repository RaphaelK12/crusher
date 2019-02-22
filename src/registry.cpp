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
// File:       registry.cpp
//
// Written:    10 Oct. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This module handles the registry.
// ===========================================================================
#include "stdafx.h"
#include "defs.h"
#include "errors.h"
#include "registry.h"
#include <regstr.h>

// ========================================================================
// Name.......: LoadRegValue()
// Description: This function loads a value from the registry.
// Parameters.: tValue					- the given value
//				tData					- the data to be returned
// Returns....: UINT					- a return code
// ========================================================================
UINT
LoadRegValue (LPCTSTR tValue, LPTSTR tData)
{
    DWORD	dwDisp = iREG_KEY_BUF_SIZE;
    DWORD	dwType;
    HKEY	hkCrusher;
    LONG	lResult;

    lResult = RegOpenKeyExA(HKEY_CURRENT_USER,
                            tREG_KEY_CRUSHER,
                            0,
                            KEY_QUERY_VALUE,
                            &hkCrusher);

    if (lResult == ERROR_SUCCESS)
    {
        lResult = RegQueryValueEx (hkCrusher,
                                   tValue,
                                   0,
                                   (LPDWORD) &dwType,
                                   (LPBYTE) tData,
                                   (LPDWORD) &dwDisp);

        RegCloseKey (hkCrusher);

        if (lResult != ERROR_SUCCESS)
        {
            return iERR_RENDER_REG_QUERY;
        }
    }
    else
    {
        return iERR_RENDER_REG_OPEN;
    }

    return iRC_OK;
} // LoadRegValue

// ========================================================================
// Name.......: SaveRegValue()
// Description: This function saves the given value in the registry.
// Parameters.: tValue					- the given value
//				tData					- the data to be stored
// Returns....: NIL
// ========================================================================
void
SaveRegValue (LPCTSTR tValue, LPCTSTR tData)
{
    DWORD	dwDisp = iREG_KEY_BUF_SIZE;
    HKEY	hkClass;
    HKEY	hkCrusher;

    RegCreateKeyEx (HKEY_CURRENT_USER,
                    tREG_KEY_CLASS,
                    0,
                    "",
                    REG_OPTION_NON_VOLATILE,
                    KEY_ALL_ACCESS,
                    NULL,
                    &hkClass,
                    &dwDisp);
    RegCreateKeyEx (hkClass,
                    tREG_KEY_NAME,
                    0,
                    "",
                    REG_OPTION_NON_VOLATILE,
                    KEY_ALL_ACCESS,
                    NULL,
                    &hkCrusher,
                    &dwDisp);
    RegSetValueEx (hkCrusher,
                   tValue,
                   0,
                   REG_SZ,
                   (CONST BYTE *) tData,
                   lstrlen (tData) + 1);

    RegCloseKey (hkClass);
    RegCloseKey (hkCrusher);
} // SaveRegValue
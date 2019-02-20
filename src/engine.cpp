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
// File:       engine.cpp
//
// Written:    11 Dec. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This module contains code for the viewer engine.
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

// ===========================================================================
// Name.......: CEngine()
// Description:	This is the constructor for the CEngine class.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
CEngine::CEngine ()
{
    tModelFilename[0] = '\0';
    tSkinFilename[0] = '\0';

	uRendMask = iREND_FLATSHADE | iREND_TEXTURED | iREND_WIREFRAME;
	uRendState = iREND_TEXTURED;

    ClearXRot();
    ClearYRot();
    ClearZRot();
    ClearHorzOffset();
    ClearVertOffset();
} // CEngine

// ===========================================================================
// Name.......: ~CEngine()
// Description:	This is the destructor for the CEngine class.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
CEngine::~CEngine()
{
    // empty
} // ~CEngine

// ===========================================================================
// Name.......: XRot()
// Description:	This function sets the angle of rotation about the x axis.
// Parameters.: nAng					- the value of the angle
// Returns....: NIL
// ===========================================================================
void
CEngine::XRot (const UINT iGiven)
{
    int nTemp;

    nTemp = iGiven % 360;

    if (nTemp < 0)
    {
        xAng = (UINT) (nTemp + 360);
    }
    else
    {
        xAng = (UINT) nTemp;
    }
} // XRot

// ===========================================================================
// Name.......: AdjustXRot()
// Description:	This function adjusts the angle of rotation about the x axis.
// Parameters.: nAng					- value to adjust the angle
// Returns....: NIL
// ===========================================================================
void
CEngine::AdjustXRot (const int nAng)
{
    int nTemp;

    if (nAng == 0)
    {
        return;
    }

    nTemp = nAng + xAng;

    if (nTemp < 0)
    {
        xAng = (UINT) (nTemp + 360);
    }
    else
    {
        xAng = (UINT) (nTemp % 360);
    }
} // AdjustXRot

// ===========================================================================
// Name.......: ClearXRot()
// Description:	This function clears the angle of rotation about the x axis.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
void
CEngine::ClearXRot()
{
    xAng = 90;
} // ClearXRot

// ===========================================================================
// Name.......: YRot()
// Description:	This function sets the angle of rotation about the y axis.
// Parameters.: iGiven					- the value of the angle
// Returns....: NIL
// ===========================================================================
void
CEngine::YRot (const UINT iGiven)
{
    int nTemp;

    nTemp = iGiven % 360;

    if (nTemp < 0)
    {
        yAng = (UINT) (nTemp + 360);
    }
    else
    {
        yAng = (UINT) nTemp;
    }
} // YRot

// ===========================================================================
// Name.......: AdjustYRot()
// Description:	This function adjusts the angle of rotation about the y axis.
// Parameters.: nAng					- value to adjust the angle
// Returns....: NIL
// ===========================================================================
void
CEngine::AdjustYRot (const int nAng)
{
    int nTemp;

    if (nAng == 0)
    {
        return;
    }

    nTemp = nAng + yAng;

    if (nTemp < 0)
    {
        yAng = (UINT) (nTemp + 360);
    }
    else
    {
        yAng = (UINT) (nTemp % 360);
    }
} // AdjustYRot

// ===========================================================================
// Name.......: ClearYRot()
// Description:	This function clears the angle of rotation about the y axis.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
void
CEngine::ClearYRot()
{
    yAng = 0;
} // ClearYRot

// ===========================================================================
// Name.......: ZRot()
// Description:	This function sets the angle of rotation about the z axis.
// Parameters.: iGiven					- the value of the angle
// Returns....: NIL
// ===========================================================================
void
CEngine::ZRot (const UINT iGiven)
{
    UINT nTemp;

    nTemp = iGiven % 360;

    if (nTemp < 0)
    {
        zAng = nTemp + 360;
    }
    else
    {
        zAng = nTemp;
    }
} // ZRot

// ===========================================================================
// Name.......: AdjustZRot()
// Description:	This function adjusts the angle of rotation about the z axis.
// Parameters.: nAng					- value to adjust the angle
// Returns....: NIL
// ===========================================================================
void
CEngine::AdjustZRot (const int nAng)
{
    int nTemp;

    if (nAng == 0)
    {
        return;
    }

    nTemp = nAng + zAng;

    if (nTemp < 0)
    {
        zAng = (UINT) (nTemp + 360);
    }
    else
    {
        zAng = (UINT) (nTemp % 360);
    }
} // AdjustZRot

// ===========================================================================
// Name.......: ClearZRot()
// Description:	This function clears the angle of rotation about the z axis.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
void
CEngine::ClearZRot()
{
    zAng = 0;
} // ClearZRot

// ===========================================================================
// Name.......: HorzOffset()
// Description:	This function sets the horizontal offest.
// Parameters.: iGiven					- value for the horizontal offset
// Returns....: NIL
// ===========================================================================
void
CEngine::HorzOffset (const UINT iGiven)
{
    iHorzOffset = iGiven;
} // HorzOffset

// ===========================================================================
// Name.......: AdjustHorzOffset()
// Description:	This function adjusts the horizontal offset of the model.
// Parameters.: iOffset					- value to adjust the offset
// Returns....: NIL
// ===========================================================================
void
CEngine::AdjustHorzOffset (const int iOffset)
{
    iHorzOffset = (UINT) iHorzOffset + iOffset;
} // AdjustHorzOffset

// ===========================================================================
// Name.......: ClearHorzOffset()
// Description:	This function clears the horizontal offset of the model.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
void
CEngine::ClearHorzOffset ()
{
    iHorzOffset = 0;
} // ClearHorzOffset

// ===========================================================================
// Name.......: VertOffset()
// Description:	This function sets the vertical offest.
// Parameters.: iGiven					- value for the vertical offset
// Returns....: NIL
// ===========================================================================
void
CEngine::VertOffset (const UINT iGiven)
{
    iVertOffset = iGiven;
} // VertOffset

// ===========================================================================
// Name.......: AdjustVertOffset()
// Description:	This function adjusts the vertical offset of the model.
// Parameters.: iOffset					- value to adjust the offset
// Returns....: NIL
// ===========================================================================
void
CEngine::AdjustVertOffset (const int iOffset)
{
    iVertOffset = (UINT) iVertOffset + iOffset;
} // AdjustVertOffset

// ===========================================================================
// Name.......: ClearVertOffset()
// Description:	This function clears the vertical offset of the model.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
void
CEngine::ClearVertOffset ()
{
    iVertOffset = 0;
} // ClearVertOffset

void
CEngine::ModelFilename (const LPTSTR tGiven)
{
    strcpy (tModelFilename, tGiven);
}

void
CEngine::SkinFilename (const LPTSTR tGiven)
{
    strcpy (tSkinFilename, tGiven);
}

UINT
CEngine::SetValuesFromRegistry()
{
    TCHAR			tTemp[iMAX_PATH_LEN];
    int				iRet;
    BOOL			bFound;
    TCHAR			*ch;

    if (tModelFilename[0] == '\0')
    {
        iRet = LoadRegValue (tREG_KEY_MODEL, tTemp);
        if (iRet != iRC_OK || tTemp[0] == '\0')
        {
            tModelFilename[0] = '\0';
            if (LoadRegValue (tREG_KEY_Q2PATH, tModelFilename) == iRC_OK)
            {
                if (*(tModelFilename + strlen(tModelFilename) -1) != '\\')
                {
                    strcat (tModelFilename, "\\");
                }
                strcat (tModelFilename, tQUAKE2_PLAYER_DIR);
                strcat (tModelFilename, tQUAKE2_MD2_PATHNAME);
                strcat (tModelFilename, "\\");
                strcat (tModelFilename, tQUAKE2_MD2_FILENAME);
            }
            else
            {
                return iERR_RENDER_REG;
            }
        }
        else
        {
            strcpy (tModelFilename, tTemp);
        }

        // check model filename for valid value due to registry values of
        // previous versions
        bFound = FALSE;
        for (ch = tModelFilename; *ch != '\0'; ch++)
        {
            if (*ch == '\\')
            {
                bFound = TRUE;
                break;
            }
        }

        if (bFound == FALSE)
        {
            tModelFilename[0] = '\0';
            if (LoadRegValue (tREG_KEY_Q2PATH, tModelFilename) == iRC_OK)
            {
                if (*(tModelFilename + strlen(tModelFilename) -1) != '\\')
                {
                    strcat (tModelFilename, "\\");
                }
                strcat (tModelFilename, tQUAKE2_PLAYER_DIR);
                strcat (tModelFilename, tQUAKE2_MD2_PATHNAME);
                strcat (tModelFilename, "\\");

                iRet = LoadRegValue (tREG_KEY_MODEL, tTemp);
                if (iRet != iRC_OK || tTemp[0] == '\0')
                {
                    return iERR_RENDER_REG;
                }

                strcat (tModelFilename, tTemp);
            }
            else
            {
                return iERR_RENDER_REG;
            }
        }
    }

    if (tSkinFilename[0] == '\0')
    {
        iRet = LoadRegValue (tREG_KEY_SKIN, tTemp);
        if (iRet != iRC_OK || tTemp[0] == '\0')
        {
            tSkinFilename[0] = '\0';
            if (LoadRegValue (tREG_KEY_Q2PATH, tSkinFilename) == iRC_OK)
            {
                if (*(tSkinFilename + strlen(tSkinFilename) -1) != '\\')
                {
                    strcat (tSkinFilename, "\\");
                }
                strcat (tSkinFilename, tQUAKE2_PLAYER_DIR);
                strcat (tSkinFilename, tQUAKE2_MD2_PATHNAME);
                strcat (tSkinFilename, "\\");
                strcat (tSkinFilename, tQUAKE2_PCX_FILENAME);
            }
            else
            {
                return iERR_RENDER_REG;
            }
        }
        else
        {
            strcpy (tSkinFilename, tTemp);
        }

        // check skin filename for valid value due to registry values of
        // previous versions
        bFound = FALSE;
        for (ch = tSkinFilename; *ch != '\0'; ch++)
        {
            if (*ch == '\\')
            {
                bFound = TRUE;
                break;
            }
        }

        if (bFound == FALSE)
        {
            tSkinFilename[0] = '\0';
            if (LoadRegValue (tREG_KEY_Q2PATH, tSkinFilename) == iRC_OK)
            {
                if (*(tSkinFilename + strlen(tSkinFilename) -1) != '\\')
                {
                    strcat (tSkinFilename, "\\");
                }
                strcat (tSkinFilename, tQUAKE2_PLAYER_DIR);
                strcat (tSkinFilename, tQUAKE2_MD2_PATHNAME);
                strcat (tSkinFilename, "\\");

                iRet = LoadRegValue (tREG_KEY_SKIN, tTemp);
                if (iRet != iRC_OK || tTemp[0] == '\0')
                {
                    return iERR_RENDER_REG;
                }

                strcat (tSkinFilename, tTemp);
            }
            else
            {
                return iERR_RENDER_REG;
            }
        }
    }

    return iRC_OK;
}
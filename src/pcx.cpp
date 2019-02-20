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
// File:       pcx.cpp
//
// Written:    27 Jun. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This module handles pcx files.
// ===========================================================================
#include "stdafx.h"
#include "defs.h"
#include "errors.h"
#include "pcx.h"
#include "palette.h"
#include <stdio.h>
#include <stdlib.h>

static void WriteEnc (unsigned char, unsigned char, FILE *);

// ===========================================================================
// Name.......: PCXBitmap()
// Description:	This function is the constructor.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
PCXBitmap::PCXBitmap()
{
    m_iWidth = 0;
    m_iHeight = 0;
} // PCXBitmap

// ===========================================================================
// Name.......: ~PCXBitmap()
// Description:	This function is the destructor. It frees the memory
//				allocated to the PCX.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
PCXBitmap::~PCXBitmap()
{
    if (m_iWidth != 0)
    {
        delete [] m_pPixels;
    }
} // ~PCXBitmap

// ===========================================================================
// Name.......: operator=()
// Description:	This function is the copy assignment.
// Parameters.: rhs					- right hand side
// Returns....: NIL
// ===========================================================================
PCXBitmap&
PCXBitmap::operator= (const PCXBitmap& rhs)
{
    if (this != &rhs)
    {
        delete [] m_pPixels;

		memcpy (&m_pcxHeader, &rhs.m_pcxHeader, sizeof(pcxheader_s));

        m_iWidth = rhs.m_iWidth;
        m_iHeight = rhs.m_iHeight;

        m_pPixels = new BYTE[rhs.m_iWidth * rhs.m_iHeight];
        if (m_pPixels != (BYTE *)0)
        {
            memcpy (m_pPixels, rhs.m_pPixels, m_iWidth * m_iHeight);
        }
    }

    return *this;
} // operator=

// ===========================================================================
// Name.......: Load()
// Description:	This function loads a PCX file.
// Parameters.: sFilename			- the name of the PCX file
// Returns....: UINT				- an error return code
// ===========================================================================
UINT
PCXBitmap::Load (LPCTSTR tFilename)
{
    BYTE	bCurrByte;
    FILE	*pPCXFile;
    int		iRunLength;
    long	i;
    size_t	szRead;

    // open the PCX file
    pPCXFile = fopen(tFilename, "rb");
    if (pPCXFile == (FILE *) 0)
    {
        // Unable to open file for reading
        return iERR_PCX_OPENFILE;
    }

    szRead = fread (&m_pcxHeader, sizeof(pcxheader_s), 1, pPCXFile);
    if (szRead < 1)
    {
        // not a valid PCX file
        fclose (pPCXFile);
        return iERR_PCX_IDENTIFIER;
    }

    if (m_pcxHeader.bIdentifier != 0x0a)
    {
        // invalid PCX header
        fclose (pPCXFile);
        return iERR_PCX_HEADER;
    }

    if (m_pcxHeader.bBitPerPixel != 8)
    {
        // must be an 8 bit PCX file
        fclose (pPCXFile);
        return iERR_PCX_EIGHTBIT;
    }

    if (m_pcxHeader.bVersion != 5)
    {
        // not a valid version number
        fclose (pPCXFile);
        return iERR_PCX_VERSION;
    }

    m_iWidth = m_pcxHeader.wXEnd -
               m_pcxHeader.wXStart + 1;
    m_iHeight = m_pcxHeader.wYEnd -
                m_pcxHeader.wYStart + 1;

    // fill the pixel array
    fseek (pPCXFile, sizeof(pcxheader_s), SEEK_SET);

    m_pPixels = new BYTE[m_iWidth * m_iHeight];
    if (m_pPixels == (BYTE *) 0)
    {
        fclose (pPCXFile);
        return iERR_PCX_PIXEL_ARRAY;
    }

    i = 0;
    while (i < m_iWidth * m_iHeight)
    {
        bCurrByte = fgetc(pPCXFile);

        if ((bCurrByte & 0xc0) == 0xc0)
        {
            iRunLength = bCurrByte & 0x3f;

            bCurrByte = fgetc(pPCXFile);
            for ( ;iRunLength > 0; iRunLength--)
            {
                m_pPixels[i] = bCurrByte;
                i++;
            }
        }
        else
        {
            m_pPixels[i++] = bCurrByte;
        }
    }

    fclose (pPCXFile);
    return iRC_OK;
}

// ===========================================================================
// Name.......: Save()
// Description:	This function saves a PCX file using the format's RLE
//				compression.
// Parameters.: sFilename			- the name of the PCX file
// Returns....: UINT				- an error return code
// ===========================================================================
UINT
PCXBitmap::Save (LPCTSTR tFilename)
{
	FILE			*pFile;
	unsigned char	uThis;
	unsigned char	uLast;
    unsigned char	uRunCount;
	RGBQUAD			pColors[256];
	int				x;
	int				y;
	BYTE			*pCurr;

    // open the PCX file
    pFile = fopen(tFilename, "wb");
    if (pFile == (FILE *) 0)
    {
        // Unable to open file for writing
        return iERR_PCX_OPENFILE;
    }

	fwrite (&m_pcxHeader, sizeof (pcxheader_s), 1, pFile);

	for (y=0; y < m_iHeight; y++)
	{
		pCurr = m_pPixels + (y * m_iWidth);
		uLast = *(pCurr);
		uRunCount = 1;
		
		for (x=1; x < m_iWidth; x++)
		{
			uThis = *(++pCurr);
			
			if (uThis == uLast)
			{
				uRunCount++;
				
				if (uRunCount == 63)
				{
					WriteEnc (uLast, uRunCount, pFile);
					uRunCount = 0;
				}
			}
			else
			{
				if (uRunCount)
				{
					WriteEnc (uLast, uRunCount, pFile);
				}
				
				uLast = uThis;
				uRunCount = 1;
			}
		}

		if (uRunCount)
		{
			WriteEnc (uLast, uRunCount, pFile);
		}
	}

	fputc (0x0C, pFile);

	Set256Palette (pColors);

	for (x=0; x < 256; x++)
	{
        fputc ((int) pColors[x].rgbRed, pFile);
        fputc ((int) pColors[x].rgbGreen, pFile);
        fputc ((int) pColors[x].rgbBlue, pFile);
	}

    fclose (pFile);

    return iRC_OK;
}

static void
WriteEnc (unsigned char uByte, unsigned char uLength, FILE *pFile)
{
	if (uLength)
	{
		if ((uLength == 1) && ((uByte & 0xc0) != 0xc0))
		{
			putc ((int) uByte, pFile);
		}
		else
		{
			putc ((int) 0xc0 | uLength, pFile);
			putc ((int) uByte, pFile);
		}
	}
}

// ===========================================================================
// $Source: d:\\cvs\\master/crusher/src/pcx.h,v $
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
// File:       pcx.h
//
// Written:    27 Jun. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This header file for the module that handles pcx files
// ===========================================================================

#ifndef __PCX_H
#define __PCX_H

typedef struct pcxheader_s
{
   BYTE bIdentifier;
   BYTE bVersion;
   BYTE bEncoding;
   BYTE bBitPerPixel;
   WORD wXStart;
   WORD wYStart;
   WORD wXEnd;
   WORD wYEnd;
   WORD wHorzRes;
   WORD wVertRes;
   BYTE bPalette[48];
   BYTE bReserved1;
   BYTE bNumBitPlanes;
   WORD wBytesPerLine;
   WORD wPaletteType;
   WORD wHorzScreenSize;
   WORD wVertScreenSize;
   BYTE bReserved2[54];
} pcxheader_t;

class PCXBitmap
{
private:
	pcxheader_t		m_pcxHeader;

public:
	BYTE			*m_pPixels;
	int				m_iHeight;
	int				m_iWidth;

public:
	PCXBitmap();
	~PCXBitmap();

	UINT Load (LPCTSTR);
	UINT Save (LPCTSTR);

	PCXBitmap (const PCXBitmap&); // copy constructor
	PCXBitmap & operator= (const PCXBitmap&); // copy assignment
};

#endif // __PCX_H

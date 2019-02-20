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
// File:       package.cpp
//
// Written:    11 Jan. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This modules handles models stored in Unreal's package format.
// ===========================================================================
#include "stdafx.h"
#include "defs.h"
#include "errors.h"
#include "package.h"
#include <stdio.h>

static UINT ReadExports		(FILE *, export_t *, int, int);
static UINT ReadImports		(FILE *, import_t *, int, int);
static UINT ReadNames		(FILE *, name_t *, int, int);
static int ReadCompactIndex	(FILE *);

// ===========================================================================
// Name.......: CPackage()
// Description:	This is the constructor.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
CPackage::CPackage()
{
}

// ===========================================================================
// Name.......: ~CPackage()
// Description:	This is the destructor.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
CPackage::~CPackage()
{
	int i;

	for (i=0; i < m_hdr.iNameCount; i++)
	{
		delete [] m_names[i].tNameString;
	}
}

// ===========================================================================
// Name.......: Load()
// Description:	This function loads data from a package file.
// Parameters.: tFilename			- a string containing the filename
// Returns....: UINT				- a return code
// ===========================================================================
UINT
CPackage::Load (char *tFilename)
{
	FILE		*pUFile;
	UINT		iResult;

	pUFile = fopen(tFilename, "rb");
	if (pUFile == (FILE *) 0)
	{
		return iERR_PKG_OPENFILE;
	}

	fread (&m_hdr, sizeof(packageheader_t), 1, pUFile);

	if (m_hdr.iTag != 0x9E2A83C1)
	{
		// invalid package file
		return iERR_PKG_INVALID_FILE;
	}

	if (m_hdr.iFileVersion > 63)
	{
		// version number is not a supported version
		return iERR_PKG_VERSION;
	}

	m_names = new name_t[m_hdr.iNameCount];
	if (m_names == NULL)
	{
		// unable to allocate memory for names
		return iERR_PKG_ALLOC_NAMES;
	}

	m_imports = new import_t[m_hdr.iImportCount];
	if (m_imports == NULL)
	{
		// unable to allocate memory for imports
		return iERR_PKG_ALLOC_IMPORTS;
	}

	m_exports = new export_t[m_hdr.iExportCount];
	if (m_exports == NULL)
	{
		// unable to allocate memory for exports
		return iERR_PKG_ALLOC_EXPORTS;
	}

	iResult = ReadNames (pUFile, m_names, m_hdr.iNameOffset, m_hdr.iNameCount);
	if (iResult != iRC_OK)
	{
		return iResult;
	}

	iResult = ReadImports (pUFile, m_imports, m_hdr.iImportOffset, m_hdr.iImportCount);
	if (iResult != iRC_OK)
	{
		return iResult;
	}

	iResult = ReadExports (pUFile, m_exports, m_hdr.iExportOffset, m_hdr.iExportCount);
	if (iResult != iRC_OK)
	{
		return iResult;
	}

	fclose (pUFile);

	return iRC_OK;
}

// ===========================================================================
// Name.......: ObjectName()
// Description:	This function returns the object name matching the given index.
// Parameters.: iGiven				- an index
// Returns....: char*				- a string containing the name
// ===========================================================================
char *
CPackage::ObjectName (int iGiven)
{
	if (iGiven == 0)
	{
		return "None";
	}
	else if (iGiven > 0 && m_names[m_exports[iGiven-1].iObjectName].tNameString != NULL)
	{
		return m_names[m_exports[iGiven-1].iObjectName].tNameString;
	}
	else if (iGiven < 0 && m_names[m_imports[-iGiven-1].iObjectName].tNameString != NULL)
	{
		return m_names[m_imports[-iGiven-1].iObjectName].tNameString;
	}
	else
	{
		return "Nothing";
	}
}

// ===========================================================================
// Name.......: ReadCompactIndex()
// Description:	This function loads a md2 model from a file.
// Parameters.: pFile				- a pointer to the file
// Returns....: int					- the value, or -1 on error
// ===========================================================================
static int
ReadCompactIndex (FILE *pFile)
{
	int		iTemp;
	char	B0;
	char	B1;
	char	B2;
	char	B3;
	char	B4;

	if (feof(pFile))
	{
		// attempt to read past end of file
		return -1;
	}

	// byte 0
	B0 = fgetc (pFile);
	iTemp = 0;
	if (B0 & 0x40)
	{
		// byte 1
		B1 = fgetc(pFile);
		if (B1 & 0x80)
		{
			// byte 2
			B2 = fgetc(pFile);
			if (B2 & 0x80)
			{
				// byte 3
				B3 = fgetc(pFile);
				if (B3 & 0x80)
				{
					// byte 4
					B4 = fgetc(pFile);
					iTemp = B4;
				}
				iTemp = (iTemp << 7) + (B3 & 0x7f);
			}
			iTemp = (iTemp << 7) + (B2 & 0x7f);
		}
		iTemp = (iTemp << 7) + (B1 & 0x7f);
	}

	iTemp = (iTemp << 6) + (B0 & 0x3f);

	if (B0 & 0x80)
	{
		iTemp = -iTemp;
	}

	return iTemp;
}

// ===========================================================================
// Name.......: ReadExports()
// Description:	This function reads the exports.
// Parameters.: pFile				- a pointer to the file
//				exports				- a pointer to the exports array
//				iOffset				- offset in file
//				iCount				- the total number of exports
// Returns....: UINT				- a return code
// ===========================================================================
static UINT
ReadExports (FILE *pFile, export_t *exports, int iOffset, int iCount)
{
	int i;

	fseek (pFile, iOffset, SEEK_SET);
	
	for (i=0; i < iCount; i++)
	{
		if ((exports[i].iClassIndex = ReadCompactIndex(pFile)) == -1)
		{
			return iERR_PKG_COMPACT_INDEX;
		}

		if ((exports[i].iSuperIndex = ReadCompactIndex(pFile)) == -1)
		{
			return iERR_PKG_COMPACT_INDEX;
		}

		fread (&exports[i].iPackageIndex, sizeof(int), 1, pFile);

		if ((exports[i].iObjectName = ReadCompactIndex(pFile)) == -1)
		{
			return iERR_PKG_COMPACT_INDEX;
		}

		fread (&exports[i].iObjectFlags, sizeof(int), 1, pFile);

		if ((exports[i].uSerialSize = ReadCompactIndex(pFile)) == -1)
		{
			return iERR_PKG_COMPACT_INDEX;
		}

		if (exports[i].uSerialSize >= 0)
		{
			if ((exports[i].iSerialOffset = ReadCompactIndex(pFile)) == -1)
			{
				return iERR_PKG_COMPACT_INDEX;
			}
		}
	}

	return iRC_OK;
}

// ===========================================================================
// Name.......: ReadImports()
// Description:	This function reads the imports.
// Parameters.: pFile				- a pointer to the file
//				imports				- a pointer to the imports array
//				iOffset				- offset in file
//				iCount				- the total number of imports
// Returns....: UINT				- a return code
// ===========================================================================
static UINT
ReadImports (FILE *pFile, import_t *imports, int iOffset, int iCount)
{
	int i;

	fseek (pFile, iOffset, SEEK_SET);

	for (i=0; i < iCount; i++)
	{
		if ((imports[i].iClassPackage = ReadCompactIndex(pFile)) == -1)
		{
			return iERR_PKG_COMPACT_INDEX;
		}

		if ((imports[i].iClassName = ReadCompactIndex(pFile)) == -1)
		{
			return iERR_PKG_COMPACT_INDEX;
		}

		fread (&imports[i].iPackageIndex, sizeof(int), 1, pFile);

		if ((imports[i].iObjectName = ReadCompactIndex(pFile)) == -1)
		{
			return iERR_PKG_COMPACT_INDEX;
		}
	}

	return iRC_OK;
}

// ===========================================================================
// Name.......: ReadNames()
// Description:	This function reads the name strings.
// Parameters.: pFile				- a pointer to the file
//				names				- a pointer to the names array
//				iOffset				- offset in file
//				iCount				- the total number of names
// Returns....: UINT				- a return code
// ===========================================================================
static UINT
ReadNames (FILE *pFile, name_t *names, int iOffset, int iCount)
{
	int		i;
	int		j;
	int		iTemp;
	char	ch;

	fseek (pFile, iOffset, SEEK_SET);

	for (i=0; i < iCount; i++)
	{
		names[i].tNameString = new char[iPKG_NAME_SIZE];

		for (j=0; (ch = fgetc(pFile)) != 0 && j < iPKG_NAME_SIZE; j++)
		{
			names[i].tNameString[j] = ch;
		}
		names[i].tNameString[j] = '\0';

		fread (&iTemp, sizeof(int), 1, pFile);
		names[i].iNameFlags = iTemp;
	}

	return iRC_OK;
}

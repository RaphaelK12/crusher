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
// File:       package.h
//
// Written:    11 Jan. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This header file contains definitions for Epic Megagames' package file
//	  format. Most structures found here were derived from documentation found
//	  at http://unreal.epicgames.com/
// ===========================================================================
#ifndef __UPACKAGE_H
#define __UPACKAGE_H

#define RF_LoadForClient	0x00010000
#define RF_LoadForServer	0x00020000
#define RF_LoadForEdit		0x00040000
#define RF_Public			0x00000004
#define RF_Standalone		0x00080000
#define RF_Intrinsic		0x04000000
#define RF_SourceModified	0x00000020
#define RF_Transactional	0x00000001
#define RF_HasStack			0x02000000

typedef struct
{
	int				iTag;
	int				iFileVersion;
	int				iPackageFlags;
	int				iNameCount;
	int				iNameOffset;
	int				iExportCount;
	int				iExportOffset;
	int				iImportCount;
	int				iImportOffset;
	int				iHeritageCount;
	int				iHeritageOffset;
} packageheader_t; 

typedef struct
{
	char			*tNameString;
	int				iNameFlags;
} name_t;

typedef struct
{
	int				iClassPackage;
	int				iClassName;
	int				iPackageIndex;
	int				iObjectName;
} import_t;

typedef struct
{
	int				iClassIndex;
	int				iSuperIndex;
	int				iPackageIndex;
	int				iObjectName;
	int				iObjectFlags;
	unsigned short	uSerialSize;
	int				iSerialOffset;
} export_t;

class CPackage
{
public:
	packageheader_t m_hdr;
	export_t		*m_exports;
	import_t		*m_imports;
	name_t			*m_names;

	CPackage();
	~CPackage();

	UINT Load (char *);
	char *ObjectName (int);
};

#endif // __UPACKAGE_H

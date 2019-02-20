// ===========================================================================
// $Source: d:\\cvs\\master/crusher/src/defs.h,v $
// $Revision: 2.8 $
// $Date: 1998/12/13 20:09:40 $
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
// File:       defs.h
//
// Written:    16 Jun. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This header file contains some basic structures used throughout all
//    program modules.
// ===========================================================================
#ifndef __DEFS_H
#define __DEFS_H

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned int        UINT;
typedef DWORD				COLORREF;

typedef struct
{
   long x, y;
} POINT_2D;

typedef struct
{
   double x, y, z;
}   POINT_3D;

typedef struct
{
	long x, y;
	short s, t;
} triangle_t;

const int		iWIDTH = 512;
const int		iHEIGHT = 384;

const int		iMIN_WIDTH = 100;
const int		iMIN_HEIGHT = 100;

const int		iMAX_PATH_LEN = 512;

const int		iREG_KEY_BUF_SIZE = 256;

const int		iBACKGRND_COLOR = 181;
const int		iWIREFRAME_TRI_COLOR = 253;
const int		iFLATSHADE_TRI_COLOR = 11;

const int		iMAX_COLOR = 254;

const UINT		iPKG_NAME_SIZE = 64;

const BYTE		iREND_WIREFRAME = 1;
const BYTE		iREND_FLATSHADE = 2;
const BYTE		iREND_TEXTURED = 4;

const BYTE		iMASK_WIREFRAME = ~iREND_WIREFRAME;
const BYTE		iMASK_FLATSHADE = ~iREND_FLATSHADE;
const BYTE		iMASK_TEXTURED = ~iREND_TEXTURED;

const LPTSTR	tDEFAULT = "nothing";
const LPTSTR	tVERSION = "Crusher md2 Model Viewer v1.04 beta";

const LPTSTR	tUSAGE = "Usage: crusher.exe [options]\n"
						"where options include:\n"
						"-model filename\tloads the model file\n"
						"-skin filename\tloads the skin file\n"
						"-xrot value\trotates about x-axis by value amount\n"
						"-yrot value\trotates about y-axis by value amount\n"
						"-zrot value\trotates about z-axis by value amount\n"
						"-wireframe\tsets the viewer to wireframe mode\n"
						"-flatshade\t\tset the viewer to flatshading mode\n"
						"-h, -help, -?\tdisplays this screen\n"
						"-v, -V, -version\tdisplays the program version number";

const LPTSTR	tQUAKE2_PATH = "C:\\Quake2\\";
const LPTSTR	tQUAKE2_PLAYER_DIR = "baseq2\\players\\";
const LPTSTR	tQUAKE2_MD2_PATHNAME = "female";
const LPTSTR	tQUAKE2_MD2_FILENAME = "tris.md2";
const LPTSTR	tQUAKE2_PCX_FILENAME = "jezebel.pcx";

// Unreal Mesh Export constants
const LPTSTR	tUNREAL_PROJECT_DIR = "E:\\Games\\Unreal\\NewProj";

const LPTSTR	tREG_KEY_CLASS = "Software\\Gazpacho";
const LPTSTR	tREG_KEY_NAME = "Crusher";
const LPTSTR	tREG_KEY_CRUSHER = "Software\\Gazpacho\\Crusher";
const LPTSTR	tREG_KEY_Q2PATH = "Quake2Path";
const LPTSTR	tREG_KEY_PATH = "LastPath";
const LPTSTR	tREG_KEY_MODEL = "LastModel";
const LPTSTR	tREG_KEY_SKIN = "LastSkin";

const LPTSTR	tREG_KEY_UNR_PROJDIR = "UnrProjDir";

const LPTSTR	tGAZPACHO_NET_URL = "http://www.gazpacho.net/";
const LPTSTR	tCRUSHER_URL = "http://www.gazpacho.net/crusher/";
const LPTSTR	tTEMP_HTML_FILENAME = "C:\\temphtm.HTM";


#endif // __DEFS_H

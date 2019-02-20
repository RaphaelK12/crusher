// ===========================================================================
// $Source: d:\\cvs\\master/crusher/src/engine.h,v $
// $Revision: 2.3 $
// $Date: 1998/12/14 04:41:43 $
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
// Project:    Crusher engine viewer
//
// File:       engine.h
//
// Written:    16 Jun. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This header file contains definitions for the viewer engine.
// ===========================================================================
#ifndef __ENGINE_H
#define __ENGINE_H

class CEngine
{
private:
	BYTE		uRendState;
	BYTE		uRendMask;
	UINT		xAng;
	UINT		yAng;
	UINT		zAng;
	UINT		iHorzOffset;
	UINT		iVertOffset;

public:
	TCHAR		tModelFilename[iMAX_PATH_LEN];
	TCHAR		tSkinFilename[iMAX_PATH_LEN];

public:
	CEngine();
	~CEngine();

	void RendState (const BYTE uGiven)
	{
		uRendState = uGiven & uRendMask;
		SetRendState (uRendState);
	} ;

	BYTE &RendState()
	{
		return uRendState;
	};

	void RendMask (const BYTE uGiven)
	{
		uRendMask = uGiven;
	} ;

	BYTE &RendMask()
	{
		return uRendMask;
	};

	void XRot(const UINT);
	UINT &XRot() { return xAng; };
	void AdjustXRot (const int);
	void ClearXRot ();

	void YRot(const UINT);
	UINT &YRot() { return yAng; };
	void AdjustYRot (const int);
	void ClearYRot ();

	void ZRot(const UINT);
	UINT &ZRot() { return zAng; };
	void AdjustZRot (const int);
	void ClearZRot ();

	void HorzOffset (const UINT);
	UINT &HorzOffset() { return iHorzOffset; };
	void AdjustHorzOffset (const int);
	void ClearHorzOffset ();

	void VertOffset (const UINT);
	UINT &VertOffset() { return iVertOffset; };
	void AdjustVertOffset (const int);
	void ClearVertOffset ();

	void ModelFilename (const LPTSTR);
	LPTSTR ModelFilename () { return tModelFilename; };

	void SkinFilename (const LPTSTR);
	LPTSTR SkinFilename () { return tSkinFilename; };

	UINT SetValuesFromRegistry();

	CEngine (const CEngine&); // copy constructor
	CEngine & operator= (const CEngine&); // copy assignment
};

#endif // __ENGINE_H
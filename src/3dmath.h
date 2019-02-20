// ===========================================================================
// $Source: d:\\cvs\\master/crusher/src/3dmath.h,v $
// $Revision: 2.1 $
// $Date: 1998/10/04 21:45:20 $
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
// File:       3dmath.h
//
// Written:    16 Jun. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This is the header file for the 3dmath module.
// ===========================================================================
#ifndef __3DMATH_H
#define __3DMATH_H

class Vec3D
{
public:
	double x, y, z;

	Vec3D();

	// TODO: add copy constructor
	// TODO: add assignment constructor

	Vec3D	DiffPt		(POINT_3D ptA, POINT_3D ptB);
	double	Magnitude	();
	Vec3D	NormalPt	(POINT_3D ptA, POINT_3D ptB, POINT_3D ptC);
	void	Normalize	();
	Vec3D	operator-	(const Vec3D &vecB);
	Vec3D	operator*	(const Vec3D &vecB);
	Vec3D	operator*	(const double fScalar);
	Vec3D	operator=	(const Vec3D &vecB);
	Vec3D	operator=	(POINT_3D ptB);
	void	Unit		();
};

class Matrix3D
{
public:
	double m11, m12, m13, m14;
	double m21, m22, m23, m24;
	double m31, m32, m33, m34;
	double m41, m42, m43, m44;

public:
	Matrix3D();

	void scale (double scale_factor);
	void scale (double xTheta, double yTheta, double zTheta);
	void translate (double x, double y, double z);
	void xrot (UINT theta);
	void yrot (UINT theta);
	void zrot (UINT theta);
	Matrix3D operator* (Matrix3D &matB);
	void unit ();
	POINT_3D transform (const POINT_3D &ptB);
	POINT_3D transform (const float ptB[]);
};


void	Initialize_CosSin	();
double	Cos					(UINT);
double	Sin					(UINT);

Vec3D	Cross				(const Vec3D &, const Vec3D &);
double	Dot					(const Vec3D &, const Vec3D &);
Vec3D	Transform			(const Matrix3D &, const Vec3D &);

#endif // __3DMATH_H

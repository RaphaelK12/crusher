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
// File:       3dmath.cpp
//
// Written:    16 Jun. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This module contains code for 3D math operations.
// ===========================================================================
#include "stdafx.h"
#include "defs.h"
#include "errors.h"
#include "3dmath.h"
#include <math.h>

#define M_PI	            3.14159265358979323846

double CosTable[360];
double SinTable[360];

//==============================================================================
// Matrix3D

// ===========================================================================
// Name.......: Matrix3D()
// Description:	This function is the Matrix3D constructor
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
Matrix3D::Matrix3D ()
{
    unit();
}

// ===========================================================================
// Name.......: scale()
// Description:	This function scales all dimensions by scale factor.
// Parameters.: scale_factor		- the scale factor
// Returns....: NIL
// ===========================================================================
void
Matrix3D::scale (double scale_factor)
{
    m11 *= scale_factor;
    m12 *= scale_factor;
    m13 *= scale_factor;
    m14 *= scale_factor;

    m21 *= scale_factor;
    m22 *= scale_factor;
    m23 *= scale_factor;
    m24 *= scale_factor;

    m31 *= scale_factor;
    m32 *= scale_factor;
    m33 *= scale_factor;
    m34 *= scale_factor;
}

// ===========================================================================
// Name.......: scale()
// Description:	This function scales scales along each axis independently.
// Parameters.: xTheta			- the scale factor along the x axis
//				yTheta			- the scale factor along the y axis
//				zTheta			- the scale factor along the z axis
// Returns....: NIL
// ===========================================================================
void
Matrix3D::scale (double xTheta, double yTheta, double zTheta)
{
    m11 *= xTheta;
    m12 *= xTheta;
    m13 *= xTheta;
    m14 *= xTheta;

    m21 *= yTheta;
    m22 *= yTheta;
    m23 *= yTheta;
    m24 *= yTheta;

    m31 *= zTheta;
    m32 *= zTheta;
    m33 *= zTheta;
    m34 *= zTheta;
}

// ===========================================================================
// Name.......: translate()
// Description:	This function translates the origin.
// Parameters.: xTheta			- the amount to translate the x axis
//				yTheta			- the amount to translate the y axis
//				zTheta			- the amount to translate the z axis
// Returns....: NIL
// ===========================================================================
void
Matrix3D::translate (double xTheta, double yTheta, double zTheta)
{
    m41 += xTheta;
    m42 += yTheta;
    m43 += zTheta;
}

// ===========================================================================
// Name.......: xrot()
// Description:	This function rotates about the x axis.
// Parameters.: theta			- the amount to rotate about the x axis
// Returns....: NIL
// ===========================================================================
void
Matrix3D::xrot (UINT theta)
{
    double fCos, fSin;
    double temp21, temp22, temp23, temp24;
    double temp31, temp32, temp33, temp34;

    fCos = Cos(theta);
    fSin = Sin(theta);

    temp21 = m21 * fCos + m31 * fSin;
    temp22 = m22 * fCos + m32 * fSin;
    temp23 = m23 * fCos + m33 * fSin;
    temp24 = m24 * fCos + m34 * fSin;

    temp31 = m31 * fCos - m21 * fSin;
    temp32 = m32 * fCos - m22 * fSin;
    temp33 = m33 * fCos - m23 * fSin;
    temp34 = m34 * fCos - m24 * fSin;

    m21 = temp21;
    m22 = temp22;
    m23 = temp23;
    m24 = temp24;

    m31 = temp31;
    m32 = temp32;
    m33 = temp33;
    m34 = temp34;
}

// ===========================================================================
// Name.......: yrot()
// Description:	This function rotates about the y axis.
// Parameters.: theta			- the amount to rotate about the y axis
// Returns....: NIL
// ===========================================================================
void
Matrix3D::yrot (UINT theta)
{
    double fCos, fSin;
    double temp11, temp12, temp13, temp14;
    double temp31, temp32, temp33, temp34;

    fCos = Cos(theta);
    fSin = Sin(theta);

    temp11 = m11 * fCos + m31 * fSin;
    temp12 = m12 * fCos + m32 * fSin;
    temp13 = m13 * fCos + m33 * fSin;
    temp14 = m14 * fCos + m34 * fSin;

    temp31 = m31 * fCos - m11 * fSin;
    temp32 = m32 * fCos - m12 * fSin;
    temp33 = m33 * fCos - m13 * fSin;
    temp34 = m34 * fCos - m14 * fSin;

    m11 = temp11;
    m12 = temp12;
    m13 = temp13;
    m14 = temp14;

    m31 = temp31;
    m32 = temp32;
    m33 = temp33;
    m34 = temp34;
}

// ===========================================================================
// Name.......: zrot()
// Description:	This function rotates about the z axis.
// Parameters.: theta			- the amount to rotate about the z axis
// Returns....: NIL
// ===========================================================================
void
Matrix3D::zrot (UINT theta)
{
    double fCos, fSin;
    double temp11, temp12, temp13, temp14;
    double temp21, temp22, temp23, temp24;

    fCos = Cos(theta);
    fSin = Sin(theta);

    temp11 = m11 * fCos - m21 * fSin;
    temp12 = m12 * fCos - m22 * fSin;
    temp13 = m13 * fCos - m23 * fSin;
    temp14 = m14 * fCos - m24 * fSin;

    temp21 = m21 * fCos + m11 * fSin;
    temp22 = m22 * fCos + m12 * fSin;
    temp23 = m23 * fCos + m13 * fSin;
    temp24 = m24 * fCos + m14 * fSin;

    m11 = temp11;
    m12 = temp12;
    m13 = temp13;
    m14 = temp14;

    m21 = temp21;
    m22 = temp22;
    m23 = temp23;
    m24 = temp24;
}

// ===========================================================================
// Name.......: operator * ()
// Description:	This function multiples two matrices
// Parameters.: matB			- the second matrix
// Returns....: Matrix3D		- the product of the mulitplication
// ===========================================================================
Matrix3D
Matrix3D::operator* (Matrix3D &matB)
{
    double temp11, temp12, temp13, temp14;
    double temp21, temp22, temp23, temp24;
    double temp31, temp32, temp33, temp34;

    temp11 = m11 * matB.m11 + m21 * matB.m12 + m31 * matB.m13 + m41 * matB.m14;
    temp12 = m12 * matB.m11 + m22 * matB.m12 + m32 * matB.m13 + m42 * matB.m14;
    temp13 = m13 * matB.m11 + m23 * matB.m12 + m33 * matB.m13 + m43 * matB.m14;
    temp14 = m14 * matB.m11 + m24 * matB.m12 + m34 * matB.m13 + m44 * matB.m14;

    temp21 = m11 * matB.m21 + m21 * matB.m22 + m31 * matB.m23 + m41 * matB.m24;
    temp22 = m12 * matB.m21 + m22 * matB.m22 + m32 * matB.m23 + m42 * matB.m24;
    temp23 = m13 * matB.m21 + m23 * matB.m22 + m33 * matB.m23 + m43 * matB.m24;
    temp24 = m14 * matB.m21 + m24 * matB.m22 + m34 * matB.m23 + m44 * matB.m24;

    temp31 = m11 * matB.m31 + m21 * matB.m32 + m31 * matB.m33 + m41 * matB.m34;
    temp32 = m12 * matB.m31 + m22 * matB.m32 + m32 * matB.m33 + m42 * matB.m34;
    temp33 = m13 * matB.m31 + m23 * matB.m32 + m33 * matB.m33 + m43 * matB.m34;
    temp34 = m14 * matB.m31 + m24 * matB.m32 + m34 * matB.m33 + m44 * matB.m34;

    m11 = temp11;
    m12 = temp12;
    m13 = temp13;
    m14 = temp14;

    m21 = temp21;
    m22 = temp22;
    m23 = temp23;
    m24 = temp24;

    m31 = temp31;
    m32 = temp32;
    m33 = temp33;
    m34 = temp34;

    return *this;
}

// ===========================================================================
// Name.......: operator * ()
// Description:	This function multiples two matrices
// Parameters.: matB			- the second matrix
// Returns....: Matrix3D		- the product of the mulitplication
// ===========================================================================
// ===========================================================================
// Name.......: unit()
// Description:	This function initializes a matrix.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
void
Matrix3D::unit()
{
    m11 = (double)1;
    m12 = 0;
    m13 = 0;
    m14 = 0;

    m21 = 0;
    m22 = (double)1;
    m23 = 0;
    m24 = 0;

    m31 = 0;
    m32 = 0;
    m33 = (double)1;
    m34 = 0;

    m41 = 0;
    m42 = 0;
    m43 = 0;
    m44 = (double)1;
}

// ===========================================================================
// Name.......: transform()
// Description:	This function multiplies a point by a matrix.
// Parameters.: ptB				- 3D point
// Returns....: POINT_3D		- the resultant 3D point
// ===========================================================================
POINT_3D
Matrix3D::transform (const POINT_3D &ptB)
{
    POINT_3D result;

    result.x = m11 * ptB.x + m21 * ptB.y + m31 * ptB.z + m41; // m41 * 1
    result.y = m12 * ptB.x + m22 * ptB.y + m32 * ptB.z + m42; // m42 * 1
    result.z = m13 * ptB.x + m23 * ptB.y + m33 * ptB.z + m43; // m43 * 1

    return result;
}

// ===========================================================================
// Name.......: transform()
// Description:	This function multiplies a point by a matrix.
// Parameters.: ptB				- an array containing x, y and z components
// Returns....: POINT_3D		- the resultant 3D point
// ===========================================================================
POINT_3D
Matrix3D::transform (const float ptB[])
{
    POINT_3D result;

    result.x = m11 * ptB[0] + m21 * ptB[1] + m31 * ptB[2] + m41; // m41 * 1
    result.y = m12 * ptB[0] + m22 * ptB[1] + m32 * ptB[2] + m42; // m42 * 1
    result.z = m13 * ptB[0] + m23 * ptB[1] + m33 * ptB[2] + m43; // m43 * 1

    return result;
}

//==============================================================================
// Vec 3D

// ===========================================================================
// Name.......: Vec3D()
// Description:	This function is the constructor for Vec3D
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
Vec3D::Vec3D ()
{
    Unit();
}

// ===========================================================================
// Name.......: DiffPt()
// Description:	This function sets the vector equal to the difference of two
//				points.
// Parameters.: ptA				- the first point
//				ptB				- the second point
// Returns....: Vec3D			- the difference
// ===========================================================================
Vec3D
Vec3D::DiffPt (POINT_3D ptA, POINT_3D ptB)
{
    x = ptA.x - ptB.x;
    y = ptA.y - ptB.y;
    z = ptA.z - ptB.z;

    return *this;
}

// ===========================================================================
// Name.......: Magnitude()
// Description:	This function returns the magnitude of the vector.
// Parameters.: NIL
// Returns....: double			- the magnitude
// ===========================================================================
double
Vec3D::Magnitude ()
{
    double q;

    q = x * x;
    q += y * y;
    q += z * z;

    q = sqrt (q);
    return (q);
}

// ===========================================================================
// Name.......: NormalPt()
// Description:	This function sets the vector equal to the normal of the given
//				triangle.
// Parameters.: ptA				- the first point
//				ptB				- the second point
//				ptC				- the third point
// Returns....: Vec3D			- the normal vector
// ===========================================================================
Vec3D
Vec3D::NormalPt	(POINT_3D ptA, POINT_3D ptB, POINT_3D ptC)
{
    POINT_3D v1;
    POINT_3D v2;

    v1.x = ptA.x - ptB.x;
    v1.y = ptA.y - ptB.y;
    v1.z = ptA.z - ptB.z;

    v2.x = ptC.x - ptB.x;
    v2.y = ptC.y - ptB.y;
    v2.z = ptC.z - ptB.z;

    // dot
    x = v1.y * v2.z - v1.z * v2.y;
    y = v1.z * v2.x - v1.x * v2.z;
    z = v1.x * v2.y - v1.y * v2.x;

    return *this;
}

// ===========================================================================
// Name.......: Magnitude()
// Description:	This function normalizes the vector.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
void
Vec3D::Normalize ()
{
    double c;

    c = Magnitude ();

    x = x / c;
    y = y / c;
    z = z / c;
}

// ===========================================================================
// Name.......: operator - ()
// Description:	This function returns the difference of two vectors.
// Parameters.: vecB			- the second vector
// Returns....: Vec3D			- the difference
// ===========================================================================
Vec3D
Vec3D::operator- (const Vec3D &vecB)
{
    Vec3D diff;

    diff.x = x - vecB.x;
    diff.y = y - vecB.y;
    diff.z = z - vecB.z;

    return diff;
}

// ===========================================================================
// Name.......: operator * ()
// Description:	This function returns the product of a vector multiplied by a
//				scalar.
// Parameters.: fScalar			- the scalar
// Returns....: Vec3D			- the product
// ===========================================================================
Vec3D
Vec3D::operator* (const double fScalar)
{
    x = x * fScalar;
    y = y * fScalar;
    z = z * fScalar;

    return *this;
}

// ===========================================================================
// Name.......: operator * ()
// Description:	This function returns the product of two vectors.
// Parameters.: vecB			- the second vector
// Returns....: Vec3D			- the product
// ===========================================================================
Vec3D
Vec3D::operator* (const Vec3D &vecB)
{
    Vec3D product;

    product.x = x * vecB.x;
    product.y = y * vecB.y;
    product.z = z * vecB.z;

    return product;
}

// ===========================================================================
// Name.......: operator = ()
// Description:	This function sets the vector equal to another.
// Parameters.: vecB			- the vector
// Returns....: Vec3D			- the result
// ===========================================================================
Vec3D
Vec3D::operator= (const Vec3D &vecB)
{
    x = vecB.x;
    y = vecB.y;
    z = vecB.z;

    return *this;
}

// ===========================================================================
// Name.......: operator = ()
// Description:	This function sets the vector equal to the value of a point.
// Parameters.: ptB				- the point
// Returns....: Vec3D			- the result
// ===========================================================================
Vec3D
Vec3D::operator= (POINT_3D ptB)
{
    x = ptB.x;
    y = ptB.y;
    z = ptB.z;

    return *this;
}

// ===========================================================================
// Name.......: Unit()
// Description:	This function makes the vector the identity vector.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
void
Vec3D::Unit ()
{
    // Initialize vector
    x = 0;
    y = 0;
    z = 0;
}

//==============================================================================
// Helper Math Functions

// ===========================================================================
// Name.......: Initialize_CosSin()
// Description:	This function initializes the sine and cosine lookup tables.
// Parameters.: NIL
// Returns....: NIL
// ===========================================================================
void Initialize_CosSin()
{
    int i;

    for (i = 0; i <= 360; i++)
    {
        CosTable[i] = cos(M_PI/180.0*i);
    }

    for (i = 0; i <= 360; i++)
    {
        SinTable[i] = sin(M_PI/180.0*i);
    }
}

// ===========================================================================
// Name.......: Sin()
// Description:	This function returns the sine of an angle.
// Parameters.: Angle			- the given angle
// Returns....: double			- the sine of the angle
// ===========================================================================
double Sin (UINT Angle)
{
    return SinTable[Angle];
}

// ===========================================================================
// Name.......: Cos()
// Description:	This function returns the cosine of an angle.
// Parameters.: Angle			- the given angle
// Returns....: double			- the cosine of the angle
// ===========================================================================
double Cos (UINT Angle)
{
    return CosTable[Angle];
}

// ===========================================================================
// Name.......: Cross()
// Description:	This function returns the cross product of two vectors.
// Parameters.: vecA			- the first vector
//				vecB			- the second vector
// Returns....: Vec3D			- the cross product
// ===========================================================================
Vec3D
Cross (const Vec3D &vecA, const Vec3D &vecB)
{
    Vec3D result;

    result.x = vecA.y * vecB.z - vecA.z * vecB.y;
    result.y = vecA.z * vecB.x - vecA.x * vecB.z;
    result.z = vecA.x * vecB.y - vecA.y * vecB.x;

    return result;
}

// ===========================================================================
// Name.......: Dot()
// Description:	This function is the constructor for Vec3D
// Parameters.: vecA			- the first vector
//				vecB			- the second vector
// Returns....: Vec3D			- the dot product
// ===========================================================================
double
Dot (const Vec3D &vecA, const Vec3D &vecB)
{
    double d;

    d = vecA.x * vecB.x;
    d += vecA.y * vecB.y;
    d += vecA.z * vecB.z;

    return (d);
}

// ===========================================================================
// Name.......: Transform()
// Description:	This function multiples a vector by a matrix.
// Parameters.: ptB				- 3D point
// Returns....: Vec3D		- the resultant 3D point
// ===========================================================================
Vec3D
Transform (const Matrix3D &matA, const Vec3D &vecA)
{
    Vec3D result;

    result.x = matA.m11 * vecA.x + matA.m21 * vecA.y + matA.m31 * vecA.z + matA.m41; // m41 * 1
    result.y = matA.m12 * vecA.x + matA.m22 * vecA.y + matA.m32 * vecA.z + matA.m42; // m42 * 1
    result.z = matA.m13 * vecA.x + matA.m23 * vecA.y + matA.m33 * vecA.z + matA.m43; // m43 * 1

    return result;
}
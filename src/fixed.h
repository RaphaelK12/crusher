// ===========================================================================
// $Source: d:\\cvs\\master/crusher/src/fixed.h,v $
// $Revision: 2.0 $
// $Date: 1998/09/23 00:28:22 $
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
// File:       fixed.h
//
// Written:    06 Jul. 1998 - Tom Conder <blitz@gazpacho.net>
//
// Description:
//    This header file contains definitions for a 16.16 fixed point class.
//	This code originally appeared in Microsoft's DirectX sample source.
// ===========================================================================
#ifndef __FIXED_H
#define __FIXED_H

inline long fixed_mul(long a, long b) {return MulDiv(a, b, 65536);}
inline long fixed_div(long a, long b) {return MulDiv(a, 65536, b);}

class Fixed {
//    private:
public:
        long fx;
    public:
        Fixed()             {}
        ~Fixed()            {}

        Fixed(long l)       {fx = l<<16;}
        Fixed(int i)        {fx = (long)i<<16;}
        Fixed(double d)     {fx = (long)(d * 65536.0);}

        int Int()           {return (int)(fx >> 16);}
        int Frac()          {return (int)(fx & 0xFFFF);}

        operator int()      {return (int)(fx >> 16);}
        operator double()   {return (double)fx / 65536.0;}

        Fixed operator +(Fixed a) {Fixed c; c.fx = fx + a.fx; return c;}
        Fixed operator -(Fixed a) {Fixed c; c.fx = fx - a.fx; return c;}
        Fixed operator *(Fixed a) {Fixed c; c.fx = fixed_mul(fx,a.fx); return c;}
        Fixed operator /(Fixed a) {Fixed c; c.fx = fixed_div(fx,a.fx); return c;}

        int operator <(Fixed a)  {return fx < a.fx;}
        int operator >(Fixed a)  {return fx > a.fx;}
        int operator ==(Fixed a) {return fx == a.fx;}
        int operator !=(Fixed a) {return fx != a.fx;}

        Fixed& operator +=(Fixed a) {fx += a.fx; return *this;}
        Fixed& operator -=(Fixed a) {fx -= a.fx; return *this;}
        Fixed& operator *=(Fixed a) {fx  = fixed_mul(fx,a.fx); return *this;}
        Fixed& operator /=(Fixed a) {fx  = fixed_div(fx,a.fx); return *this;}
};

#endif // __FIXED_H

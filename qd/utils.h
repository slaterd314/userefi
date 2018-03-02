#ifndef UTILS_H_INCLUDED__
#define UTILS_H_INCLUDED__
#pragma once
#include "qd/dd_real.h"
#include "qd/qd_real.h"
#include "mcadHelper.h"
#include <complex>


using DD = LPCOMPLEXARRAY;		// double-double
using CDD = LPCCOMPLEXARRAY;	// const double-double


dd_real makeDD(LPCCOMPLEXARRAY a);

qd_real makeQD(LPCCOMPLEXARRAY a);

std::complex<dd_real> cmakeDD(LPCCOMPLEXARRAY a);

inline
bool isDDQDc(LPCCOMPLEXARRAY a)
{
	return (a->cols == 1 && (a->rows == 2 || a->rows == 4))
		||
		(a->rows == 1 && (a->cols == 2 || a->cols == 4));
}

template<class T>
inline
BOOL copy(LPCOMPLEXARRAY result, const T &s)
{
	auto success = MathcadArrayAllocate(result, sizeof(s.x) / sizeof(s.x[0]), 1, TRUE, FALSE);
	if (success)
	{
		for (auto i = 0; i < sizeof(s.x) / sizeof(s.x[0]); ++i)
		{
			result->hReal[0][i] = s.x[i];
		}
	}
	return success;
}

template<class T>
inline
BOOL copy(LPCOMPLEXARRAY result, const std::complex<T> &s)
{
	auto success = MathcadArrayAllocate(result, sizeof(s.real().x) / sizeof(s.real().x[0]), 1, TRUE, TRUE);
	if (success)
	{
		for (auto i = 0; i < sizeof(s.x) / sizeof(s.x[0]); ++i)
		{
			result->hReal[0][i] = s.real().x[i];
			result->hImag[0][i] = s.imag().x[i];
		}
	}
	return success;
}

inline
bool isZero(LPCCOMPLEXSCALAR a)
{
	return (a->real == 0.0);
}

inline
bool isZero(CDD a)
{
	return a->hReal[0][0] == 0.0;
}

inline
BOOL
allocDD(LPCOMPLEXARRAY arr)
{
	return MathcadArrayAllocate(arr, 2, 1, TRUE, FALSE);
}

inline
BOOL
allocDD(LPCOMPLEXARRAY arr, double d1, double d2 = 0.0)
{
	return RealVector(arr, d1, d2);
}

inline
BOOL
allocDD(LPCOMPLEXARRAY arr, double *d)
{
	return RealVector(arr, d, 2);
}

inline
BOOL
allocQD(LPCOMPLEXARRAY arr)
{
	return MathcadArrayAllocate(arr, 4, 1, TRUE, FALSE);
}

inline
BOOL
allocQD(LPCOMPLEXARRAY arr, double d1, double d2 = 0.0, double d3 = 0.0, double d4 = 0.0)
{
	return RealVector(arr, d1, d2, d3, d4);
}

inline
BOOL
allocQD(LPCOMPLEXARRAY arr, double *d)
{
	return RealVector(arr, d, 4);
}

inline
bool
isReal(LPCCOMPLEXARRAY a)
{
	return (a->hImag == nullptr && a->hReal != nullptr);
}

inline 
bool
isInt(LPCCOMPLEXSCALAR a)
{
	return ((a->imag == 0.0) && (std::floor(a->real) == a->real));
}

template<int N>
inline
bool isVector(LPCCOMPLEXARRAY a)
{
	return  (a->rows == N && a->cols == 1);
}

inline
bool
isDD(LPCCOMPLEXARRAY a)
{
	return (isVector<2>(a) && isReal(a));
}

inline
bool
isQD(LPCCOMPLEXARRAY a)
{
	return (isVector<4>(a) && isReal(a));
}

inline
bool
isD(LPCCOMPLEXSCALAR a)
{
	return (a->imag == 0.0);
}


#define ISDD(a,param) if(!isDD(a)) return MAKELRESULT(NOT_A_DD_VALUE,param)
#define ISQD(a,param) if(!isQD(a)) return MAKELRESULT(NOT_A_QD_VALUE,param)
#define ISDDQD(a,param) if(!(isDD(a) || isQD(a)) ) return MAKELRESULT(NOT_A_DD_OR_QD_VALUE,param)
#define ISD(a,param) if(!isD(a)) return MAKELRESULT(MUST_BE_REAL,param)
#define ALLOCDD(a) if(!allocDD(a))return MAKELRESULT(OUT_OF_MEMORY, 0)
#define ALLOCQD(a) if(!allocQD(a))return MAKELRESULT(OUT_OF_MEMORY, 0)
#define DIVZ(a, param) if(isZero(a)) return MAKELRESULT(DIVIDE_BY_ZERO, param)
#define ISINT(a, param) if(!isInt(a)) return MAKELRESULT(MUST_BE_INT,param)



#endif // UTILS_H_INCLUDED__
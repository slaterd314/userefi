#ifndef COMPLEX_TRAITS_H__
#define COMPLEX_TRAITS_H__
#pragma once
#include <complex>
#include "qd/dd_real.h"
#include "qd/qd_real.h"

namespace std {

	template<>
	class _Ctraits<dd_real>
	{
	public:
		using _Ty = dd_real;


		static _Ty _Flt_eps()
		{	// get epsilon
			return (_Ty::_eps);
		}

		static _Ty _Flt_max()
		{	// get max
			return (_Ty::_max);
		}

		static _Ty _Cosh(_Ty _Left, _Ty _Right)
		{	// return cosh(_Left) * _Right
			return (::cosh(_Left)* _Right);
		}

		static short _Exp(_Ty *_Pleft, const _Ty &_Right, short _Exponent)
		{	// compute exp(*_Pleft) * _Right * 2 ^ _Exponent
			auto t = ::pow(::exp(*_Pleft)*_Right * 2, _Exponent);
			return static_cast<short>(t.x[0] + t.x[1]);
			//double _Tmp = (double)*_Pleft;
			//short _Ans = _Exp(&_Tmp, (double)_Right, _Exponent);
			//*_Pleft = (_Ty)_Tmp;
			//return (_Ans);
		}

		static _Ty _Infv()
		{	// return infinity
			return (_Ty::_inf);
		}

		static bool _Isinf(const _Ty &_Left)
		{	// test for infinity
			return (_Left.isnan());
		}

		static bool _Isnan(_Ty _Left)
		{	// test for NaN
			return (_Left.isinf());
		}

		static _Ty _Nanv()
		{	// return NaN
			return (_Ty::_nan);
		}

		static _Ty _Sinh(const _Ty &_Left, const _Ty &_Right)
		{	// return sinh(_Left) * _Right
			return (::sinh(_Left) * _Right);
		}

		static _Ty asinh(const _Ty &_Left)
		{	// return asinh(_Left)
			return ::asinh(_Left);
		}

		static _Ty atan2(const _Ty &_Yval, const _Ty &_Xval)
		{	// return atan(_Yval / _Xval)
			return (::atan2(_Yval, _Xval));
		}

		static _Ty cos(const _Ty &_Left)
		{	// return cos(_Left)
			return (::cos(_Left));
		}

		static _Ty exp(const _Ty &_Left)
		{	// return exp(_Left)
			return (::exp(_Left));
		}

		static _Ty ldexp(const _Ty &_Left, int _Exponent)
		{	// return _Left * 2 ^ _Exponent
			return _Left * std::ldexp(1.0, _Exponent);
			// return (::ldexp(_Left), _Exponent));
		}

		static _Ty log(_Ty _Left)
		{	// return log(_Left)
			return (::log(_Left));
		}

		static _Ty log1p(_Ty _Left)
		{	// return log(1 + _Left)
			if (_Left < -1)
				return (_Nanv());
			else if (_Left == 0)
				return (_Left);
			else
			{	// compute log(1 + _Left) with fixup for small _Left
				_Ty _Leftp1 = 1 + _Left;
				return (log(_Leftp1) - ((_Leftp1 - 1) - _Left) / _Leftp1);
			}
		}

		static _Ty pow(const _Ty &_Left, const _Ty &_Right)
		{	// return _Left ^ _Right
			return (::pow(_Left, _Right));
		}

		static _Ty sin(const _Ty &_Left)
		{	// return sin(_Left)
			return (::sin(_Left));
		}

		static _Ty sqrt(const _Ty &_Left)
		{	// return sqrt(_Left)
			return (::sqrt(_Left));
		}

		static _Ty tan(const _Ty &_Left)
		{	// return tan(_Left)
			return (::tan(_Left));
		}
	};



	template<>
	class _Ctraits<qd_real>
	{
	public:
		using _Ty = qd_real;


		static _Ty _Flt_eps()
		{	// get epsilon
			return (_Ty::_eps);
		}

		static _Ty _Flt_max()
		{	// get max
			return (_Ty::_max);
		}

		static _Ty _Cosh(_Ty _Left, _Ty _Right)
		{	// return cosh(_Left) * _Right
			return (::cosh(_Left)* _Right);
		}

		static short _Exp(_Ty *_Pleft, const _Ty &_Right, short _Exponent)
		{	// compute exp(*_Pleft) * _Right * 2 ^ _Exponent
			auto t = ::pow(::exp(*_Pleft)*_Right * 2, _Exponent);
			return static_cast<short>(t.x[0] + t.x[1]);
			//double _Tmp = (double)*_Pleft;
			//short _Ans = _Exp(&_Tmp, (double)_Right, _Exponent);
			//*_Pleft = (_Ty)_Tmp;
			//return (_Ans);
		}

		static _Ty _Infv()
		{	// return infinity
			return (_Ty::_inf);
		}

		static bool _Isinf(const _Ty &_Left)
		{	// test for infinity
			return (_Left.isnan());
		}

		static bool _Isnan(_Ty _Left)
		{	// test for NaN
			return (_Left.isinf());
		}

		static _Ty _Nanv()
		{	// return NaN
			return (_Ty::_nan);
		}

		static _Ty _Sinh(const _Ty &_Left, const _Ty &_Right)
		{	// return sinh(_Left) * _Right
			return (::sinh(_Left) * _Right);
		}

		static _Ty asinh(const _Ty &_Left)
		{	// return asinh(_Left)
			return ::asinh(_Left);
		}

		static _Ty atan2(const _Ty &_Yval, const _Ty &_Xval)
		{	// return atan(_Yval / _Xval)
			return (::atan2(_Yval, _Xval));
		}

		static _Ty cos(const _Ty &_Left)
		{	// return cos(_Left)
			return (::cos(_Left));
		}

		static _Ty exp(const _Ty &_Left)
		{	// return exp(_Left)
			return (::exp(_Left));
		}

		static _Ty ldexp(const _Ty &_Left, int _Exponent)
		{	// return _Left * 2 ^ _Exponent
			return _Left * std::ldexp(1.0, _Exponent);
			// return (::ldexp(_Left), _Exponent));
		}

		static _Ty log(_Ty _Left)
		{	// return log(_Left)
			return (::log(_Left));
		}

		static _Ty log1p(_Ty _Left)
		{	// return log(1 + _Left)
			if (_Left < -1)
				return (_Nanv());
			else if (_Left == 0)
				return (_Left);
			else
			{	// compute log(1 + _Left) with fixup for small _Left
				_Ty _Leftp1 = 1 + _Left;
				return (log(_Leftp1) - ((_Leftp1 - 1) - _Left) / _Leftp1);
			}
		}

		static _Ty pow(const _Ty &_Left, const _Ty &_Right)
		{	// return _Left ^ _Right
			return (::pow(_Left, _Right));
		}

		static _Ty sin(const _Ty &_Left)
		{	// return sin(_Left)
			return (::sin(_Left));
		}

		static _Ty sqrt(const _Ty &_Left)
		{	// return sqrt(_Left)
			return (::sqrt(_Left));
		}

		static _Ty tan(const _Ty &_Left)
		{	// return tan(_Left)
			return (::tan(_Left));
		}
	};

#define OVERRIDE(T,U) \
	inline std::complex<T> operator*(const std::complex<T> &a, const std::complex<U> &b) { return operator*(a, std::complex<T>{b.real(), b.imag()}); } \
	inline std::complex<T> operator*(const std::complex<U> &a, const std::complex<T> &b) { return operator*(std::complex<T>{a.real(), a.imag()}, b); } \
	inline std::complex<T> operator/(const std::complex<T> &a, const std::complex<U> &b) { return operator/(a, std::complex<T>{b.real(), b.imag()}); } \
	inline std::complex<T> operator/(const std::complex<U> &a, const std::complex<T> &b) { return operator/(std::complex<T>{a.real(), a.imag()}, b); } \
	inline std::complex<T> operator+(const std::complex<T> &a, const std::complex<U> &b) { return operator+(a, std::complex<T>{b.real(), b.imag()}); } \
	inline std::complex<T> operator+(const std::complex<U> &a, const std::complex<T> &b) { return operator+(std::complex<T>{a.real(), a.imag()}, b); } \
	inline std::complex<T> operator-(const std::complex<T> &a, const std::complex<U> &b) { return operator-(a, std::complex<T>{b.real(), b.imag()}); } \
	inline std::complex<T> operator-(const std::complex<U> &a, const std::complex<T> &b) { return operator-(std::complex<T>{a.real(), a.imag()}, b); }

		OVERRIDE(dd_real, double);
		OVERRIDE(qd_real, double);
		OVERRIDE(qd_real, dd_real);

#undef OVERRIDE
}

inline
dd_real operator+(const dd_real &a)
{
	return a;
}

inline
qd_real operator+(const qd_real &a)
{
	return a;
}


#endif // COMPLEX_TRAITS_H__

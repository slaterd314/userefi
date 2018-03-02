// qd.cpp : Defines the exported functions for the DLL application.
//
#include "header.h"
#include "mcadHelper.h"
#include "qd/c_dd.h"
#include "qd/dd_real.h"
#include <complex>
#include <functional>
#include <ComplexTraits.h>
#include "utils.h"
#include <vector>

using namespace mcadHelper;

namespace {
	auto NOT_A_DD_VALUE = registerErrorMessage("must be a 2 element real vector");
	auto NOT_A_DD_OR_QD_VALUE = registerErrorMessage("must be a 2 or 4 element real vector");

	auto OUT_OF_MEMORY = registerErrorMessage("out of memory");
	auto MUST_BE_REAL = registerErrorMessage("must be real");
	auto INVALID_ARGUMENT = registerErrorMessage("invalid argument");
	auto DIVIDE_BY_ZERO = registerErrorMessage("divide by 0");
	auto MUST_BE_INT = registerErrorMessage("must be a integer");
	auto MUST_BE_POS_INT = registerErrorMessage("must be a positive integer");
	auto UNKNOWN_CONST = registerErrorMessage("unrecognized constant");
	auto NOT_A_DOUBLE_DOUBLE_LIST = registerErrorMessage("must be a 2xn real matrix.");
}

MCFN(dd_read, "s", "read a string representation of a double-double into a double-double value",
	 LPCOMPLEXARRAY result, LPCMCSTRING s)
{
	double dd[2]{ 0.0, 0.0 };
	c_dd_read(s->str, dd);
	if (!std::isfinite(dd[0]))
		return MAKELRESULT(INVALID_ARGUMENT, 1);

	if (!allocDD(result, dd))
		return MAKELRESULT(OUT_OF_MEMORY, 0);

	return 0;
}
END_FN;

MCFN(dd_write, "a", "converts a double-double or quad-double to a string ",
	 LPMCSTRING result, LPCCOMPLEXARRAY a)
{
	// ISDD(a, 1);
	ISDD(a, 1);

	const auto allocSize = 41;
	const auto precision = 32;

	result->str = MathcadAllocate(allocSize);

	if (!result->str)
	{
		return MAKELRESULT(OUT_OF_MEMORY, 0);
	}

	result->str[allocSize - 1] = '\0';
	c_dd_swrite(a->hReal[0], precision, result->str, allocSize);

	//if (isDd)
	//	c_dd_swrite(a->hReal[0], 32, result->str, 41);
	//else
	//	c_qd_swrite(a->hReal[0], 64, result->str, 73);

	return 0;
}
END_FN;

//MCFN(dd_cwrite, "a", "converts a complex double-double to a string ",
//	 LPMCSTRING result, LPCCOMPLEXARRAY a)
//{
//	// ISDD(a, 1);
//
//	// result->str = MathcadAllocate(41 + 3 + 41);
//
//	char re[41];
//	char im[41];
//
//	double d[2] = { a->hImag[0][0], a->hImag[0][1] };
//
//	auto isNeg = d[0] < 0.0;
//
//	if (isNeg)
//	{
//		d[0] = -a->hImag[0][0];
//		d[1] = -a->hImag[0][1];
//	}
//
//
//	// c_dd_swrite(a->hReal[0], 32, re, 41);
//	dd_real(a->hReal[0]).write(re, 41, 32, false, true);
//	dd_real(d).write(im, 41, 32, false, true);
//	// c_dd_swrite(d, 32, im, 41);
//
//	std::string str{ re };
//
//	if (isNeg)
//		str += " - ";
//	else
//		str += " + ";
//
//	str += im;
//	str += "i";
//
//	result->str = MathcadAllocate(str.size() + 1);
//	result->str[str.size()] = '\0';
//	if (!result->str)
//	{
//		return MAKELRESULT(OUT_OF_MEMORY, 0);
//	}
//	strcpy_s(result->str, str.size(), str.c_str());
//
//	return 0;
//}
//END_FN;

MCFN(dd_format, "a,precision", "converts a double-double string with precision",
	 LPMCSTRING result, LPCCOMPLEXARRAY a, LPCCOMPLEXSCALAR precision)
{
	ISDD(a, 1);
	ISD(precision, 2);

	auto n = std::floor(precision->real);

	if (n != precision->real)
	{
		return MAKELRESULT(MUST_BE_INT, 2);
	}


	auto aa = makeDD(a);

	auto s = aa.to_string(static_cast<int>(n), 0, std::ios_base::scientific, false, true);
	const auto sz = s.size();
	result->str = MathcadAllocate(static_cast<unsigned int>(sz + 1));
	if (!result->str)
	{
		return MAKELRESULT(OUT_OF_MEMORY, 0);
	}
	result->str[sz] = '\0';
	if (sz > 0)
	{
		strncpy_s(result->str, sz+1, s.c_str(), sz);
	}



	//auto n = std::floor(precision->real);
	//if (n != precision->real)
	//{
	//	return MAKELRESULT(MUST_BE_INT, 2);
	//}

	//auto nn = static_cast<unsigned>(n);

	//result->str = MathcadAllocate(nn + 8);

	//if (!result->str)
	//{
	//	return MAKELRESULT(OUT_OF_MEMORY, 0);
	//}

	//result->str[nn + 8] = '\0';

	//c_dd_swrite(a->hReal[0], nn, result->str, nn + 8);

	return 0;

}
END_FN;

///////////////////////////////Addition
MCFN(dd_add, "a,b", "adds double-double \"a\" to double-double or quad-double \"b\"", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a, LPCCOMPLEXARRAY b)
{
	ISDD(a, 1);
	ISDD(b, 2);
	const auto success = copy(result, dd_real{ a->hReal[0] } + dd_real{ b->hReal[0] });
	//const auto success = (a->rows == 2
	//					  ? b->rows == 2
	//					  ? copy(result, dd_real{ a->hReal[0] } +dd_real{ b->hReal[0] })
	//					  : copy(result, dd_real{ a->hReal[0] } +qd_real{ b->hReal[0] })
	//					  : b->rows == 2
	//					  ? copy(result, qd_real{ a->hReal[0] } +dd_real{ b->hReal[0] })
	//					  : copy(result, qd_real{ a->hReal[0] } +qd_real{ b->hReal[0] })
	//					  );

	return (success) ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
}
END_FN;

//MCFN(dd_cadd, "a,b", "adds double-double a to double-double b", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a_, LPCCOMPLEXARRAY b_)
//{
//
//	std::complex<dd_real> a{ dd_real{ a_->hReal[0][0], a_->hReal[0][1] },  dd_real{ a_->hImag[0][0], a_->hImag[0][1] } };
//	std::complex<dd_real> b{ dd_real{ b_->hReal[0][0], b_->hReal[0][1] },  dd_real{ b_->hImag[0][0], b_->hImag[0][1] } };
//
//	std::complex<dd_real> c = a + b;
//
//	MathcadArrayAllocate(result, 2, 1, true, true);
//	result->hReal[0][0] = c.real().x[0];
//	result->hReal[0][1] = c.real().x[1];
//	result->hImag[0][0] = c.imag().x[0];
//	result->hImag[0][1] = c.imag().x[1];
//
//	std::complex<double> foo{ 3.5, 1.3 };
//	auto bar = a * foo;
//	auto foobar = foo * a;
//
//
//
//	// c_dd_add(a->hReal[0], b->hReal[0], result->hReal[0]);
//	return 0;
//}
//END_FN;

MCFN(dd_add_dd_d, "a,b", "adds double-double a to a double b",
	 DD result, CDD a, LPCCOMPLEXSCALAR b)
{
	ISDD(a, 1);
	ISD(b, 2);

	ALLOCDD(result);
	c_dd_add_dd_d(a->hReal[0], b->real, result->hReal[0]);
	return 0L;
}
END_FN;

MCFN(dd_add_d_dd, "a,b", "adds double a to a double-double b",
	 DD result, LPCCOMPLEXSCALAR a, CDD b)
{
	ISD(a, 1);
	ISDD(b, 2);

	ALLOCDD(result);
	c_dd_add_d_dd(a->real, b->hReal[0], result->hReal[0]);
	return 0L;
}
END_FN;

MCFN(dd_add_d_d, "a,b", "adds double a to a double b returning a double-double result",
	 DD result, LPCCOMPLEXSCALAR a, LPCCOMPLEXSCALAR b)
{
	ISD(a, 1);
	ISD(b, 2);
	double tmp[2] = { b->real, 0.0 };

	ALLOCDD(result);
	c_dd_add_d_dd(a->real, tmp, result->hReal[0]);
	return 0L;
}
END_FN;


///////////////////////////////Subtraction

MCFN(dd_sub, "a,b", "subtracts double-double \"a\" from double-double or quad-double \"b\"", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a, LPCCOMPLEXARRAY b)
{
	ISDD(a, 1);
	ISDD(b, 2);
	const auto success = copy(result, dd_real{ a->hReal[0] } - dd_real{ b->hReal[0] });
	//const auto success = (a->rows == 2
	//					  ? b->rows == 2
	//					  ? copy(result, dd_real{ a->hReal[0] } -dd_real{ b->hReal[0] })
	//					  : copy(result, dd_real{ a->hReal[0] } -qd_real{ b->hReal[0] })
	//					  : b->rows == 2
	//					  ? copy(result, qd_real{ a->hReal[0] } -dd_real{ b->hReal[0] })
	//					  : copy(result, qd_real{ a->hReal[0] } -qd_real{ b->hReal[0] })
	//					  );

	return (success) ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
}
END_FN;

MCFN(dd_sub_dd_d, "a,b", "subtracts double-double a from double b",
	 DD result, CDD a, LPCCOMPLEXSCALAR b)
{
	ISDD(a, 1);
	ISD(b, 2);

	ALLOCDD(result);
	c_dd_sub_dd_d(a->hReal[0], b->real, result->hReal[0]);
	return 0L;
}
END_FN;

MCFN(dd_sub_d_dd, "a,b", "subtracts double a from double-double b",
	 DD result, LPCCOMPLEXSCALAR a, CDD b)
{
	ISD(a, 1);
	ISDD(b, 2);

	ALLOCDD(result);
	c_dd_sub_d_dd(a->real, b->hReal[0], result->hReal[0]);
	return 0L;
}
END_FN;

MCFN(dd_sub_d_d, "a,b", "subtracts double a from double b returning a double-double result",
	 DD result, LPCCOMPLEXSCALAR a, LPCCOMPLEXSCALAR b)
{
	ISD(a, 1);
	ISD(b, 2);
	double tmp[2] = { a->real, 0.0 };

	ALLOCDD(result);
	c_dd_sub_dd_d(tmp, b->real, result->hReal[0]);
	return 0L;
}
END_FN;

///////////////////////////////Multiplication

MCFN(dd_mul, "a,b", "multiplies double-double \"a\" by double-double or quad-double \"b\"", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a, LPCCOMPLEXARRAY b)
{
	ISDD(a, 1);
	ISDD(b, 2);
	const auto success = copy(result, dd_real{ a->hReal[0] } * dd_real{ b->hReal[0] });
	//const auto success = (a->rows == 2
	//					  ? b->rows == 2
	//					  ? copy(result, dd_real{ a->hReal[0] } *dd_real{ b->hReal[0] })
	//					  : copy(result, dd_real{ a->hReal[0] } *qd_real{ b->hReal[0] })
	//					  : b->rows == 2
	//					  ? copy(result, qd_real{ a->hReal[0] } *dd_real{ b->hReal[0] })
	//					  : copy(result, qd_real{ a->hReal[0] } *qd_real{ b->hReal[0] })
	//					  );

	return (success) ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
}
END_FN;

MCFN(dd_mul_dd_d, "a,b", "multiplies double-double a by double b",
	 DD result, CDD a, LPCCOMPLEXSCALAR b)
{
	ISDD(a, 1);
	ISD(b, 2);

	ALLOCDD(result);
	c_dd_mul_dd_d(a->hReal[0], b->real, result->hReal[0]);
	return 0L;
}
END_FN;

MCFN(dd_mul_d_dd, "a,b", "multiplies double a by double-double b",
	 DD result, LPCCOMPLEXSCALAR a, CDD b)
{
	ISD(a, 1);
	ISDD(b, 2);

	ALLOCDD(result);
	c_dd_mul_d_dd(a->real, b->hReal[0], result->hReal[0]);
	return 0L;
}
END_FN;

MCFN(dd_mul_d_d, "a,b", "multiplies double a by double b returning a double-double result",
	 DD result, LPCCOMPLEXSCALAR a, LPCCOMPLEXSCALAR b)
{
	ISD(a, 1);
	ISD(b, 2);
	double tmp[2] = { b->real, 0.0 };

	ALLOCDD(result);
	c_dd_mul_d_dd(a->real, tmp, result->hReal[0]);
	return 0L;
}
END_FN;


///////////////////////////////Division

MCFN(dd_div, "a,b", "divides double-double \"a\" by double-double \"b\"", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a, LPCCOMPLEXARRAY b)
{
	ISDD(a, 1);
	ISDD(b, 2);
	DIVZ(b, 2);
	const auto success = copy(result, dd_real{ a->hReal[0] } / dd_real{ b->hReal[0] });
	//const auto success = (a->rows == 2
	//					  ? b->rows == 2
	//					  ? copy(result, dd_real{ a->hReal[0] } / dd_real{ b->hReal[0] })
	//					  : copy(result, dd_real{ a->hReal[0] } / qd_real{ b->hReal[0] })
	//					  : b->rows == 2
	//					  ? copy(result, qd_real{ a->hReal[0] } / dd_real{ b->hReal[0] })
	//					  : copy(result, qd_real{ a->hReal[0] } / qd_real{ b->hReal[0] })
	//					  );

	return (success) ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);

}
END_FN;

MCFN(dd_rem, "a,b", "returns the remainder of dividing double-double \"a\" by double-double \"b\"", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a, LPCCOMPLEXARRAY b)
{
	ISDD(a, 1);
	ISDD(b, 2);
	DIVZ(b, 2);
	const auto success = copy(result, drem(dd_real{ a->hReal[0] }, dd_real{ b->hReal[0] }));
	//const auto success = (a->rows == 2
	//					  ? b->rows == 2
	//					  ? copy(result, dd_real{ a->hReal[0] } / dd_real{ b->hReal[0] })
	//					  : copy(result, dd_real{ a->hReal[0] } / qd_real{ b->hReal[0] })
	//					  : b->rows == 2
	//					  ? copy(result, qd_real{ a->hReal[0] } / dd_real{ b->hReal[0] })
	//					  : copy(result, qd_real{ a->hReal[0] } / qd_real{ b->hReal[0] })
	//					  );

	return (success) ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);

}
END_FN;

MCFN(dd_div_dd_d, "a,b", "divides double-double a by double b",
	 DD result, CDD a, LPCCOMPLEXSCALAR b)
{
	ISDD(a, 1);
	ISD(b, 2);
	DIVZ(b, 2);
	ALLOCDD(result);
	c_dd_div_dd_d(a->hReal[0], b->real, result->hReal[0]);
	return 0L;
}
END_FN;

MCFN(dd_div_d_dd, "a,b", "divides double a by double-double b",
	 DD result, LPCCOMPLEXSCALAR a, CDD b)
{
	ISD(a, 1);
	ISDD(b, 2);
	DIVZ(b, 2);
	ALLOCDD(result);
	c_dd_div_d_dd(a->real, b->hReal[0], result->hReal[0]);
	return 0L;
}
END_FN;

MCFN(dd_div_d_d, "a,b", "divides double a by double b returning a double-double result",
	 DD result, LPCCOMPLEXSCALAR a, LPCCOMPLEXSCALAR b)
{
	ISD(a, 1);
	ISD(b, 2);
	DIVZ(b, 2);
	ALLOCDD(result);

	double tmp[2] = { a->real, 0.0 };

	c_dd_div_dd_d(tmp, b->real, result->hReal[0]);
	return 0L;
}
END_FN;

MCFN(dd_fmod, "a,b", "divides double-double \"a\" by double-double \"b\" and returns the remainder", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a, LPCCOMPLEXARRAY b)
{
	ISDD(a, 1);
	ISDD(b, 2);
	DIVZ(b, 2);
	const auto success = copy(result, fmod(dd_real{ a->hReal[0] }, dd_real{ b->hReal[0] }));
	//const auto success = (a->rows == 2
	//					  ? b->rows == 2
	//					  ? copy(result, fmod(dd_real{ a->hReal[0] }, dd_real{ b->hReal[0] }))
	//					  : copy(result, fmod(dd_real{ a->hReal[0] }, qd_real{ b->hReal[0] }))
	//					  : b->rows == 2
	//					  ? copy(result, fmod(qd_real{ a->hReal[0] }, dd_real{ b->hReal[0] }))
	//					  : copy(result, fmod(qd_real{ a->hReal[0] }, qd_real{ b->hReal[0] }))
	//					  );

	return (success) ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);

}
END_FN;


MCFN(dd_const, "c", "looks up constants and returns the double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXSCALAR a)
{
	if (fabs(a->real - 3.141592653589793116) < 1e-3)
	{
		copy(result, dd_real::_pi);
	}
	else if (fabs(a->real - 6.28318530717958620) < 1e-3)
	{
		copy(result, dd_real::_2pi);
	}
	else if (fabs(a->real - 2.35619449019234480) < 1e-3)
	{
		copy(result, dd_real::_3pi4);
	}
	else if (fabs(a->real - 1.57079632679489660) < 1e-3)
	{
		copy(result, dd_real::_pi2);
	}
	else if (fabs(a->real - 0.78539816339744828) < 1e-3)
	{
		copy(result, dd_real::_pi4);
	}
	else if (fabs(a->real - 2.71828182845904510) < 1e-3)
	{
		copy(result, dd_real::_e);
	}
	else if (fabs(a->real - 0.69314718055994529) < 1e-3)
	{
		copy(result, dd_real::_log2);
	}
	else if (fabs(a->real - 2.30258509299404590) < 1e-3)
	{
		copy(result, dd_real::_log10);
	}
	else
	{
		return MAKELRESULT(UNKNOWN_CONST, 0);
	}
	return 0L;
}
END_FN;

MCFN(dd_sin, "a", "computes the sin of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, sin(makeDD(a)));
	return 0L;

	//ISDDQD(a, 1);
	//if (isDD(a))
	//{
	//	copy(result, sin(makeDD(a)));
	//}
	//else
	//{
	//	copy(result, sin(makeQD(a)));
	//}
	//return 0L;
}
END_FN;

MCFN(dd_cos, "a", "computes the cos of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, cos(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_tan, "a", "computes the tan of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, tan(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_asin, "a", "computes the arcsin of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, asin(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_acos, "a", "computes the arccos of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, acos(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_atan, "a", "computes the arctan of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, atan(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_atan2, "y,x", "computes arctan of double-double value y/x and returns the correct quadrant.", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY y, LPCCOMPLEXARRAY x)
{
	ISDD(y, 1);
	ISDD(x, 2);
	copy(result, atan2(makeDD(y), makeDD(x)));
	return 0L;
}
END_FN;

MCFN(dd_sinh, "a", "computes the sinh of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, sinh(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_cosh, "a", "computes the cosh of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, cosh(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_tanh, "a", "computes the tanh of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, tanh(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_asinh, "a", "computes the arc-sinh of a double-double  value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, asinh(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_acosh, "a", "computes the arc-cosh of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, acosh(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_atanh, "a", "computes the arc-tanh of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, atanh(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_exp, "a", "computes the exponential of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, exp(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_log, "a", "computes the natural log of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, log(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_log10, "a", "computes the base-10 log of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, log10(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_abs, "a", "computes the absolute value of a double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	copy(result, fabs(makeDD(a)));
	return 0L;
}
END_FN;

MCFN(dd_lt, "a,b", "returns a < b of double-double values", LPCOMPLEXSCALAR result, LPCCOMPLEXARRAY a, LPCCOMPLEXARRAY b)
{
	ISDD(a, 1);
	ISDD(b, 2);
	auto lThan = (makeDD(a) < makeDD(b));
	result->real = lThan ? 1.0 : 0.0;
	result->imag = 0.0;

	return 0L;

	//ISDDQD(a, 1);
	//ISDDQD(b, 2);
	//auto lThan = false;
	//if (isDD(a))
	//{
	//	if (isDD(b))
	//	{
	//		lThan = (makeDD(a) < makeDD(b));
	//	}
	//	else
	//	{
	//		lThan = (makeDD(a) < makeQD(b));
	//	}
	//}
	//else if (isDD(b))
	//{
	//	lThan = (makeQD(a) < makeDD(b));
	//}
	//else
	//{
	//	lThan = (makeQD(a) < makeQD(b));
	//}

	//result->real = lThan ? 1.0 : 0.0;
	//result->imag = 0.0;

	//return 0L;
}
END_FN;

MCFN(dd_lte, "a,b", "returns a <= b of double-double values", LPCOMPLEXSCALAR result, LPCCOMPLEXARRAY a, LPCCOMPLEXARRAY b)
{
	ISDD(a, 1);
	ISDD(b, 2);
	const auto lThan = (makeDD(a) <= makeDD(b));
	result->real = lThan ? 1.0 : 0.0;
	result->imag = 0.0;

	return 0L;

	//ISDDQD(a, 1);
	//ISDDQD(b, 2);
	//auto lThan = false;
	//if (isDD(a))
	//{
	//	if (isDD(b))
	//	{
	//		lThan = (makeDD(a) <= makeDD(b));
	//	}
	//	else
	//	{
	//		lThan = (makeDD(a) <= makeQD(b));
	//	}
	//}
	//else if (isDD(b))
	//{
	//	lThan = (makeQD(a) <= makeDD(b));
	//}
	//else
	//{
	//	lThan = (makeQD(a) <= makeQD(b));
	//}

	//result->real = lThan ? 1.0 : 0.0;
	//result->imag = 0.0;

	//return 0L;
}
END_FN;


MCFN(dd_gt, "a,b", "returns a > b of double-double values", LPCOMPLEXSCALAR result, LPCCOMPLEXARRAY a, LPCCOMPLEXARRAY b)
{
	ISDD(a, 1);
	ISDD(b, 2);
	const auto lThan = (makeDD(a) > makeDD(b));
	result->real = lThan ? 1.0 : 0.0;
	result->imag = 0.0;

	return 0L;

	//ISDDQD(a, 1);
	//ISDDQD(b, 2);
	//auto lThan = false;
	//if (isDD(a))
	//{
	//	if (isDD(b))
	//	{
	//		lThan = (makeDD(a) > makeDD(b));
	//	}
	//	else
	//	{
	//		lThan = (makeDD(a) > makeQD(b));
	//	}
	//}
	//else if (isDD(b))
	//{
	//	lThan = (makeQD(a) > makeDD(b));
	//}
	//else
	//{
	//	lThan = (makeQD(a) > makeQD(b));
	//}

	//result->real = lThan ? 1.0 : 0.0;
	//result->imag = 0.0;

	//return 0L;
}
END_FN;

MCFN(dd_gte, "a,b", "returns a >= b of double-double values", LPCOMPLEXSCALAR result, LPCCOMPLEXARRAY a, LPCCOMPLEXARRAY b)
{
	ISDD(a, 1);
	ISDD(b, 2);
	const auto lThan = (makeDD(a) >= makeDD(b));
	result->real = lThan ? 1.0 : 0.0;
	result->imag = 0.0;

	return 0L;

	//ISDDQD(a, 1);
	//ISDDQD(b, 2);
	//auto lThan = false;
	//if (isDD(a))
	//{
	//	if (isDD(b))
	//	{
	//		lThan = (makeDD(a) >= makeDD(b));
	//	}
	//	else
	//	{
	//		lThan = (makeDD(a) >= makeQD(b));
	//	}
	//}
	//else if (isDD(b))
	//{
	//	lThan = (makeQD(a) >= makeDD(b));
	//}
	//else
	//{
	//	lThan = (makeQD(a) >= makeQD(b));
	//}

	//result->real = lThan ? 1.0 : 0.0;
	//result->imag = 0.0;

	//return 0L;
}
END_FN;

MCFN(dd_eq, "a,b", "returns a == b of double-double values", LPCOMPLEXSCALAR result, LPCCOMPLEXARRAY a, LPCCOMPLEXARRAY b)
{
	ISDD(a, 1);
	ISDD(b, 2);
	const auto lThan = (makeDD(a) == makeDD(b));
	result->real = lThan ? 1.0 : 0.0;
	result->imag = 0.0;

	return 0L;

	//ISDDQD(a, 1);
	//ISDDQD(b, 2);
	//auto lThan = false;
	//if (isDD(a))
	//{
	//	if (isDD(b))
	//	{
	//		lThan = (makeDD(a) == makeDD(b));
	//	}
	//	else
	//	{
	//		lThan = (makeDD(a) == makeQD(b));
	//	}
	//}
	//else if (isDD(b))
	//{
	//	lThan = (makeQD(a) == makeDD(b));
	//}
	//else
	//{
	//	lThan = (makeQD(a) == makeQD(b));
	//}

	//result->real = lThan ? 1.0 : 0.0;
	//result->imag = 0.0;

	//return 0L;
}
END_FN;

MCFN(dd_neq, "a,b", "returns a != b of double-double values", LPCOMPLEXSCALAR result, LPCCOMPLEXARRAY a, LPCCOMPLEXARRAY b)
{
	ISDD(a, 1);
	ISDD(b, 2);
	const auto lThan = (makeDD(a) != makeDD(b));
	result->real = lThan ? 1.0 : 0.0;
	result->imag = 0.0;

	return 0L;

	//ISDDQD(a, 1);
	//ISDDQD(b, 2);
	//auto lThan = false;
	//if (isDD(a))
	//{
	//	if (isDD(b))
	//	{
	//		lThan = (makeDD(a) != makeDD(b));
	//	}
	//	else
	//	{
	//		lThan = (makeDD(a) != makeQD(b));
	//	}
	//}
	//else if (isDD(b))
	//{
	//	lThan = (makeQD(a) != makeDD(b));
	//}
	//else
	//{
	//	lThan = (makeQD(a) != makeQD(b));
	//}

	//result->real = lThan ? 1.0 : 0.0;
	//result->imag = 0.0;

	//return 0L;
}
END_FN;

MCFN(ddrand, "unused", "returns a random double-double value", LPCOMPLEXARRAY result, LPCCOMPLEXSCALAR)
{
	copy(result, ddrand());
	return 0L;
}
END_FN;

MCFN(dd_nint, "a", "rounds double-double value to nearest integer", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a,1);
	const auto success = copy(result, nint(makeDD(a)));
	return success ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
}
END_FN;

MCFN(dd_aint, "a", "truncates double-double value towards 0", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a,1);
	const auto success = copy(result, aint(makeDD(a)));
	return success ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
}
END_FN;


MCFN(dd_floor, "a", "computes the floor of double-double value \"a\".", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	const auto success = copy(result, floor(makeDD(a)));
	return success ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
}
END_FN;

MCFN(dd_ceil, "a", "computes the floor of double-double value \"a\".", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	const auto success = copy(result, ceil(makeDD(a)));
	return success ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
}
END_FN;

MCFN(dd_ldexp, "x,exp", "Multiplies a double-double point value x by the number 2 raised to the exp power \"a\".", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a, LPCCOMPLEXSCALAR exp)
{
	ISDD(a, 1);
	ISINT(exp, 2);
	const auto success = copy(result, ldexp( makeDD(a), static_cast<int>(exp->real) ) );
	return success ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
}
END_FN;

MCFN(dd_pow, "x,y", "Raises double-double value x to double-double power y", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY x, LPCCOMPLEXARRAY y)
{
	ISDD(x, 1);
	ISDD(y, 2);

	const auto xx = makeDD(x);
	const auto yy = makeDD(y);
	auto success = false;
	if (xx.is_zero())
	{
		if (yy.is_zero())
		{
			success = copy(result, dd_real{ 1.0 });
		}
		else
		{
			success = copy(result, dd_real{ 0.0 });
		}
	}
	else
	{
		success = copy(result, pow(xx, yy));
	}

	return success ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
}
END_FN;

MCFN(dd_npwr, "x,exp", "Raises double-double value x to integer power y", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a, LPCCOMPLEXSCALAR exp)
{
	ISDD(a, 1);
	ISINT(exp, 2);
	const auto success = copy(result, npwr(makeDD(a), static_cast<int>(exp->real)));
	return success ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
}
END_FN;

MCFN(dd_nroot, "x,n", "Computesthe n'th root of double-double value x", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a, LPCCOMPLEXSCALAR n)
{
	ISDD(a, 1);

	if (!isInt(n) || n->real < 1.0)
		return MAKELRESULT(MUST_BE_POS_INT, 2);

	const auto success = copy(result, nroot(makeDD(a), static_cast<int>(n->real)));
	return success ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
}
END_FN;

MCFN(dd_sqrt, "a", "computes the square root of double-double value \"a\".", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	const auto success = copy(result, sqrt(makeDD(a)));
	return success ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
}
END_FN;

MCFN(dd_sqr, "a", "efficient square of double-double value \"a\".", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	const auto success = copy(result, sqr(makeDD(a)));
	return success ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
}
END_FN;

MCFN(dd_isnan, "a", "tests if double-double value \"a\" is a NaN.", LPCOMPLEXSCALAR result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	result->imag = 0.0;
	result->real = makeDD(a).isnan() ? 1.0 : 0.0;
	return 0L;
}
END_FN;

MCFN(dd_isinf, "a", "tests if double-double value \"a\" is an infinity.", LPCOMPLEXSCALAR result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	result->imag = 0.0;
	result->real = makeDD(a).isinf() ? 1.0 : 0.0;
	return 0L;
}
END_FN;

MCFN(dd_isfinite, "a", "tests if double-double value \"a\" is finite.", LPCOMPLEXSCALAR result, LPCCOMPLEXARRAY a)
{
	ISDD(a, 1);
	result->imag = 0.0;
	result->real = makeDD(a).isfinite() ? 1.0 : 0.0;
	return 0L;
}
END_FN;

MCFN(dd_polyeval, "c,x", "Evaluates the given n-th degree polynomial at \"x\". The double-double coreeifients are storeed in a 2 row by n column matrix", LPCOMPLEXARRAY result, LPCCOMPLEXARRAY c, LPCCOMPLEXARRAY x)
{
	if (c->hImag != nullptr)
		return MAKELRESULT(MUST_BE_REAL, 1);
	if (c->rows != 2)
		return MAKELRESULT(NOT_A_DOUBLE_DOUBLE_LIST, 1);
	
	ISDD(x, 2);

	try
	{
		std::vector<dd_real> vec{};
		vec.reserve(c->rows);
		for (auto i = 0U; i < c->cols; ++i)
		{
			vec.emplace_back(c->hReal[i]);
		}

		const auto success = copy(result, polyeval(vec.data(), static_cast<int>(vec.size() - 1), makeDD(x)));

		return success ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
	}
	catch (const std::bad_alloc &)
	{
		return MAKELRESULT(OUT_OF_MEMORY, 0);
	}
}
END_FN;

MCFN(dd_polyroot, "c,x0,max_iter,eps", "Given an n-th degree double-double polynomial, finds a root close to the given guess \
x0. Note that this uses simple Newton iteration scheme, and does not work for multiple roots. Will run for maximum of iter iterations. \
eps is the error thereshold. Pass eps=0.0 to use build-in eps.", 
LPCOMPLEXARRAY result, LPCCOMPLEXARRAY c, LPCCOMPLEXARRAY x, LPCCOMPLEXSCALAR iter, LPCCOMPLEXSCALAR eps)
{
	if (c->hImag != nullptr)
		return MAKELRESULT(MUST_BE_REAL, 1);
	if (c->rows != 2)
		return MAKELRESULT(NOT_A_DOUBLE_DOUBLE_LIST, 1);

	ISINT(iter, 3);
	ISD(eps, 4);
	ISDD(x, 2);

	try
	{
		std::vector<dd_real> vec{};
		vec.reserve(c->rows);
		for (auto i = 0U; i < c->cols; ++i)
		{
			vec.emplace_back(c->hReal[i]);
		}

		const auto success = copy(result, polyroot(vec.data(), static_cast<int>(vec.size() - 1), makeDD(x), static_cast<int>(iter->real), eps->real));

		return success ? 0L : MAKELRESULT(OUT_OF_MEMORY, 0);
	}
	catch (const std::bad_alloc &)
	{
		return MAKELRESULT(OUT_OF_MEMORY, 0);
	}
}
END_FN;

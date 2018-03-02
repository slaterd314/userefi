#include "utils.h"

dd_real makeDD(LPCCOMPLEXARRAY a)
{
	if (a->cols == 1 && a->rows == 2)
	{
		return dd_real{ a->hReal[0] };
	}
	else if (a->cols == 2 && a->rows == 1)
	{
		return dd_real{ a->hReal[0][0], a->hImag[1][0] };
	}
	return dd_real{};
}

qd_real makeQD(LPCCOMPLEXARRAY a)
{
	if (a->cols == 1 && a->rows == 4)
	{
		return qd_real{ a->hReal[0] };
	}
	else if (a->cols == 4 && a->rows == 1)
	{
		return qd_real{ a->hReal[0][0], a->hImag[1][0], a->hReal[2][0], a->hImag[3][0] };
	}
	return qd_real{};
}

std::complex<dd_real> cmakeDD(LPCCOMPLEXARRAY a)
{
	if (a->cols == 1 && a->rows == 2)
	{
		if (a->hReal && a->hImag)
		{
			return std::complex<dd_real>{dd_real{ a->hReal[0] }, dd_real{ a->hImag[0] }};
		}
		else if (a->hReal)
		{
			return std::complex<dd_real>{dd_real{ a->hReal[0] }, dd_real{}};
		}
		else
		{
			return std::complex<dd_real>{dd_real{}, dd_real{ a->hImag[0] }};
		}
	}
	else if (a->cols == 2 && a->rows == 1)
	{

		if (a->hReal && a->hImag)
		{
			return std::complex<dd_real>{dd_real{ a->hReal[0][0], a->hReal[1][0] }, dd_real{ a->hImag[0][0], a->hImag[1][0] }};
		}
		else if (a->hReal)
		{
			return std::complex<dd_real>{dd_real{ a->hReal[0][0], a->hReal[1][0] }, dd_real{}};
		}
		else
		{
			return std::complex<dd_real>{dd_real{}, dd_real{ a->hImag[0][0], a->hImag[1][0] }};
		}
	}
	return std::complex<dd_real>{};
}

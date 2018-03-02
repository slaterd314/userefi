#include "mcadHelper.h"
using namespace mcadHelper;

extern "C" {

#define NRANSI 1

#include "nr.h"
#include "nrutil.h"

}


namespace {
	auto MUST_BE_SQUARE = registerErrorMessage("must be a square matrix.");
	auto MUST_BE_REAL = registerErrorMessage("must be real");
}



MCFN(LUDecomp, "a", "LU Decomposition",
	LPCOMPLEXARRAY result, LPCCOMPLEXARRAY m)
{
	const auto rows = m->rows;
	const auto cols = m->cols;
	if (rows != cols)
	{
		return MAKELRESULT(MUST_BE_SQUARE, 1);
	}

	if (m->hImag != nullptr)
	{
		return MAKELRESULT(MUST_BE_REAL, 1);
	}

	auto **a = matrix(1, rows, 1, cols);
	for (auto i = 1u; i <= rows; ++i)
	{
		for (auto j = 1u; j <= cols; ++j)
		{
			a[i][j] = (float)(m->hReal[j - 1][i - 1]);
		}
	}
	auto *indx = ivector(1, rows);
	float d = 1.0;
	ludcmp(a, rows, indx, &d);

	MathcadArrayAllocate(result, rows, cols + 2, TRUE, FALSE);
	for (auto i = 0u; i < rows; ++i)
	{
		for (auto j = 0u; j < cols; ++j)
		{
			result->hReal[j][i] = a[i+1][j+1];
		}
	}
	for (auto i = 0u; i < rows; ++i)
	{
		result->hReal[cols][i] = indx[i+1]-1;
	}
	result->hReal[cols + 1][0] = d;
	for (auto i = 1u; i < rows; ++i)
	{
		result->hReal[cols + 1][i] = 0.0;
	}
	
	free_matrix(a, 1, rows, 1, cols);
	free_ivector(indx, 1, rows);

	return 0L;
}
END_FN;
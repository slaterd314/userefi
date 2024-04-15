// qd.cpp : Defines the exported functions for the DLL application.
//
#include "mcadHelper.h"
#include "armadillo"

using namespace mcadHelper;
namespace {
}

static void armadillo_solve(LPCOMPLEXARRAY result, LPCOMPLEXARRAY A, LPCOMPLEXARRAY x);

MCFN(armadillo_solve, "arma_solve", "Solve a linear system using Armadillo", LPCOMPLEXARRAY result, LPCOMPLEXARRAY A, LPCOMPLEXARRAY x)
{
});


static void armadillo_solve(LPCOMPLEXARRAY result, LPCOMPLEXARRAY A, LPCOMPLEXARRAY x)
{


	arma::mat A_ = arma::mat(A->nR, A->nC);
	arma::vec x_ = arma::vec(x->nR);
	arma::vec result_ = arma::vec(result->nR);

	for (int i = 0; i < A->nR; i++)
	{
		for (int j = 0; j < A->nC; j++)
		{
			A_(i, j) = A->real[i * A->nC + j];
		}
	}

	for (int i = 0; i < x->nR; i++)
	{
		x_(i) = x->real[i];
	}

	result_ = arma::solve(A_, x_);

	for (int i = 0; i < result->nR; i++)
	{
		result->real[i] = result_(i);
	}
}
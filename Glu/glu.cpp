// qd.cpp : Defines the exported functions for the DLL application.
//
#include <gl/GLU.h>
#include "mcadHelper.h"
#include <complex>
#include <functional>
#include <vector>
#include <iomanip>
#include <list>
#include "GluData.h"

using namespace mcadHelper;
namespace {
	auto NOT_A_QD_VALUE = registerErrorMessage("must be a 4 element real vector");
	auto NOT_A_DD_OR_QD_VALUE = registerErrorMessage("must be a 2 or 4 element real vector");

	auto OUT_OF_MEMORY = registerErrorMessage("out of memory");
	auto MUST_BE_REAL = registerErrorMessage("must be real");
	auto INVALID_ARGUMENT = registerErrorMessage("invalid argument");
	auto DIVIDE_BY_ZERO = registerErrorMessage("divide by 0");
	auto MUST_BE_INT = registerErrorMessage("must be a integer");
	auto MUST_BE_POS_INT = registerErrorMessage("must be a positive integer");
	auto UNKNOWN_CONST = registerErrorMessage("unrecognized constant");
	auto NOT_A_QUAD_DOUBLE_LIST = registerErrorMessage("must be a 4xn real matrix.");
}

static void combine(GLdouble coords[3], VertexData* data[4], GLfloat w[4], VertexData** dataOut, void *ctx);
static void edgeFlag(GLboolean flag);
static void beginData(GLenum type, void* ctx);
static void endData(void* ctx);
static void vertexData(void* vertex, void* ctx);


MCFN(tesselatePoly, "poly", "Tesselate a non-convex polygon into triangles.",
	LPCOMPLEXARRAY result, LPCOMPLEXARRAY polygon)
{
	auto* newTess = gluNewTess();
	if (newTess)
	{
		auto len = polygon.rows;

	}
}


static void combine(GLdouble coords[3], VertexData* data[4], GLfloat w[4], VertexData** dataOut, void* ctx)
{
	VertexData* newData = nullptr;
	auto dataCount = size_t{ 3 };
	switch (data[0]->dataType)
	{
		case kPos:
			newData = new VertexData{ data[0]->dataType };
			dataCount = 3;
			break;
		case kPosNml:
			newData = new VertexNormalData{};
			dataCount = 6;
			break;
		case kPosClr:
			newData = new VertexColorData{};
			dataCount = 6;
			break;
		case kPosNmlClr:
			newData = new VertexNormalColorData{ };
			dataCount = 9;
			break;
	}
	if (newData)
	{
		auto list = reinterpret_cast<std::list< VertexData*> *>(ctx);
		if (list)
		{
			list->emplace_back(newData);
		}

		auto* pDstData = newData->data();

		for (auto i = 0u; i < dataCount; ++i)
		{
			pDstData[i] = w[0] * data[0]->data()[i] + w[1] * data[1]->data()[i] + w[2] * data[2]->data()[i] + w[3] * data[3]->data()[i];
		}
		*dataOut = newData;
	}
}
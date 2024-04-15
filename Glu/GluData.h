#ifndef GLUDATA_H_INCLUDED
#define GLUDATA_H_INCLUDED
#pragma once


enum Datatype
{
	kPos,
	kPosNml,
	kPosClr,
	kPosNmlClr
};

struct Position
{
	double x{ 0.0 };
	double y{ 0.0 };
	double z{ 0.0 };
};

struct NormalData
{
	double nx{ 0.0 };
	double ny{ 0.0 };
	double nz{ 0.0 };
};

struct ColorData
{
	double r{ 0.0 };
	double g{ 0.0 };
	double b{ 0.0 };
};

struct VertexData
{
	explicit VertexData(Datatype type = kPos)
		: dataType{ type }
		, pos{}
	{}
	double* data() {
		return &(pos.x);
	}
	Datatype dataType{ kPos };
	Position pos{};
};

struct VertexColorData : public VertexData
{
	VertexColorData() : VertexData{ kPosClr } {}
	ColorData  clr{};
};

struct VertexNormalData : public VertexData
{
	VertexNormalData(Datatype type = kPosNml) : VertexData{ type } {}
	NormalData  nml{};
};

struct VertexNormalColorData : public VertexNormalData
{
	VertexNormalColorData() : VertexNormalData{ kPosNmlClr } {}
	ColorData	clr;
};

#endif // GLUDATA_H_INCLUDED

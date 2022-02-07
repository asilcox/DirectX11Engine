#pragma once
#include <DirectXMath.h>

struct WorldViewProj
{
	DirectX::XMFLOAT4X4 wMatrix;
	DirectX::XMFLOAT4X4 vMatrix;
	DirectX::XMFLOAT4X4 pMatrix;
};

struct Vertex
{
	struct
	{
		float x;
		float y;
		float z;
	} pos;

	struct
	{
		float r;
		float g;
		float b;
	} color;
};


struct Transform
{
	struct
	{
		float x;
		float y;
		float z;
	} pos;

	struct
	{
		float x;
		float y;
		float z;
	} rot;

	struct
	{
		float x;
		float y;
		float z;
	} scale;
};
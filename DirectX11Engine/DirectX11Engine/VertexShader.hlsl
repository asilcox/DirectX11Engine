cbuffer cBuf
{
	matrix wMatrix;
	matrix vMatrix;
	matrix pMatrix;
};

struct OutputVertex
{
	float3 color : COLOR;
	float4 pos : SV_POSITION;
};

OutputVertex main(float3 pos : POSITION, float3 color : COLOR)
{
	OutputVertex output;
	output.pos = mul(wMatrix, float4(pos, 1.0f));
	output.pos = mul(vMatrix, output.pos);
	output.pos = mul(pMatrix, output.pos);
	output.color = color;
	return output;
}
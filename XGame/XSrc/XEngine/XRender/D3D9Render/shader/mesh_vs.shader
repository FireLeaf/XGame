
matrix world;
matrix view_proj_matrix;

struct VS_INPUT
{
	float4 pos : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 pos : POSITION;
}

VS_OUTPUT VS_Main(VS_INPUT ip)
{
	VS_OUTPUT op;
	matrix wvp = world * view_proj_matrix;
	op.pos = mul(ip.pos, wvp);
	return op;
}
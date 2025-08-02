struct VSInput
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;

    float4 color;
}
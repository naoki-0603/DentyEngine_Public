#include "Mesh/Material.hlsli"

struct VSInput
{
    float4 position : POSITION;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 worldPosition : TEXCOORD;
};

cbuffer MODEL_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
};
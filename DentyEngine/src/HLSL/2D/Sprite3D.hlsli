struct VSInput
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

struct PSOutput
{
    float4 color : SV_TARGET0;
    uint entityID : SV_TARGET1;
};

cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    uint id;
    float3 padding;

    float4 color;

    row_major float4x4 world;
}
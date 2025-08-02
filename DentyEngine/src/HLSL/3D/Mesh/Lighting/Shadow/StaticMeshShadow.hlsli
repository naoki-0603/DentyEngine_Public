struct VSInput
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 binormal : BINORMAL;
    float2 texcoord : TEXCOORD;
};

cbuffer MESH_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 shadowWorld;

    uint cascadeAreaNo;
    float3 padding;
};
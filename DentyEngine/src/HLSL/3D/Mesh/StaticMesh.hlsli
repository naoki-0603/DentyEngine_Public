#include "../../../HLSL/3D/Mesh/Material.hlsli"

struct VSInput
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 binormal : BINORMAL;
    float2 texcoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 worldNormal : NORMAL;
    float4 worldTangent : TANGENT;
    float4 worldBinormal : BINORMAL;

    // If use vertex color, store to this variable. 
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
    uint entityID : TEXCOORD1;
    float4 positionInLVP : TEXCOORD2;
    //float4 positionsInLVP[3] : TEXCOORD2;
};

struct PSOutput
{
    float4 color : SV_TARGET0;
    uint entityID : SV_TARGET1;
};

cbuffer MESH_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    
    uint entityID;
    float3 padding;

    Material material;
};

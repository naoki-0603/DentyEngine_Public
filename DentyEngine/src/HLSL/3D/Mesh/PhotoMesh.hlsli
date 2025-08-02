struct SkinnedVSInput
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 tangent : TANGENT;
    float4 binormal : BINORMAL;
    float4 boneWeights : WEIGHTS;
    uint4 boneIndices : BONES;
};

struct StaticVSInput
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 tangent : TANGENT;
    float4 binormal : BINORMAL;
};

struct VSOutput
{
    float4 position : SV_Position;
    float4 worldPosition : POSITION;
    float4 worldNormal : NORMAL;
    float4 worldTangent : TANGENT;
    float4 worldBinormal : BINORMAL;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

struct PSOutput
{
    float4 color : SV_TARGET0;
};

struct TextureUsed
{
    int albedoMap;
    int specularMap;
    int normalMap;
    int ambientMap;
};

cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 boneTransforms[256];
    
    float4 albedoColor;
    float4 specularColor;
    float4 ambientColor;
    float4 emissionColor;
    float metallic;
    float roughness;
    float2 padding;

    uint entityID;

    float3 padding2;

    TextureUsed textureUsed;
};
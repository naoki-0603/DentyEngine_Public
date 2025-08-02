struct VSInput
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 binormal : BINORMAL;
    float4 boneWeights : WEIGHTS;
    uint4 boneIndices : BONES;
    float2 texcoord : TEXCOORD;
};

cbuffer MESH_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 shadowWorld;
    row_major float4x4 shadowBoneTransforms[256];

    uint cascadeAreaNo;
    float3 padding;
}
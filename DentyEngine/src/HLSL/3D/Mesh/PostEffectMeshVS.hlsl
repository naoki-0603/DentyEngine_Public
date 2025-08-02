#include "PostEffectMesh.hlsli"
#include "../../Constants.hlsli"

VSOutput main(VSInput input)
{
    VSOutput output;

    float4 blendedPosition = { 0, 0, 0, 1 };
    float4 blendedNormal = { 0, 0, 0, 0 };
    float4 blendedTangent = { 0, 0, 0, 0 };

    // Blending position.
    for (int boneIndex = 0; boneIndex < 4; ++boneIndex)
    {
        blendedPosition += input.boneWeights[boneIndex] *
            mul(input.position, boneTransforms[input.boneIndices[boneIndex]]);
        
        blendedNormal += input.boneWeights[boneIndex] *
            mul(input.normal, boneTransforms[input.boneIndices[boneIndex]]);
        
        blendedTangent += input.boneWeights[boneIndex] *
            mul(input.tangent, boneTransforms[input.boneIndices[boneIndex]]);
    }

    input.position = float4(blendedPosition.xyz, 1.0f);
    input.normal = float4(blendedNormal.xyz, 0.0f);
    input.tangent = float4(blendedTangent.xyz, 0.0f);

    // clip space.
    output.position = mul(input.position, mul(world, postEffectCameraViewProjection));

    // Normalize.
    output.worldPosition = mul(input.position, world);

    // Calculate local.
    float3 localNormal = normalize(input.normal.xyz);
    float3 localTangent = normalize(input.tangent.xyz);
    float3 localBinormal = normalize(cross(localTangent.xyz, localNormal.xyz));

    // Calculate world space.
    output.worldNormal = mul(localNormal, world);
    output.worldTangent = mul(localTangent, world);
    output.worldBinormal = mul(localBinormal, world);

    // Texcoords
    output.texcoord = input.texcoord;
    output.color = albedoColor;
    
    return output;
}

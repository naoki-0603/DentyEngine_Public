#include "PhotoMesh.hlsli"

#include "../../Constants.hlsli"

VSOutput main( SkinnedVSInput input )
{
    VSOutput output;
    input.normal.w = 0.0f;
    
    // 1.0f -> right-handed, -1.0f -> left-handed
    float tangentSigma = input.tangent.w;
    float binormalSigma = input.binormal.w;

    input.tangent.w = 0.0f;
    input.binormal.w = 0.0f;
    
    float4 blendedPosition = { 0, 0, 0, 1 };
    float4 blendedNormal = { 0, 0, 0, 0 };
    float4 blendedTangent = { 0, 0, 0, 0 };
    
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
    
    output.position = mul(input.position, mul(world, viewProjection));
    output.worldPosition = mul(input.position, world);

    output.worldNormal = normalize(mul(input.normal, world));
    output.worldNormal.w = 0;

    output.worldBinormal = normalize(mul(input.binormal, world));
    output.worldBinormal.w = binormalSigma;

    output.worldTangent = normalize(mul(input.tangent, world));

    // 1.0f -> right-handed, -1.0f -> left-handed
    output.worldTangent.w = tangentSigma;
    
    output.texcoord = input.texcoord;
    output.color = albedoColor;

    return output;
}
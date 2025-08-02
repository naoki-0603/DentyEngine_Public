#include "SkinnedMesh.hlsli"

#include "../../Constants.hlsli"

VSOutput main(VSInput input)
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
        uint index = input.boneIndices[boneIndex];

        blendedPosition += input.boneWeights[boneIndex] *
            mul(input.position, boneTransforms[index]);
        
        blendedNormal += input.boneWeights[boneIndex] *
            mul(input.normal, boneTransforms[index]);
        
        blendedTangent += input.boneWeights[boneIndex] *
            mul(input.tangent, boneTransforms[index]);
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
    output.color = float4(1, 1, 1, 1);
    
    output.entityID = entityID;
    
    output.positionInLVP = mul(output.worldPosition, shadowCameraViewProjection);
    
    //// Shadow map near.
    //output.positionsInLVP[0] = mul(output.worldPosition, shadowCameraViewProjections[0]);
    
    //// Shadow map middle.
    //output.positionsInLVP[1] = mul(output.worldPosition, shadowCameraViewProjections[1]);
    
    //// Shadow map far.
    //output.positionsInLVP[2] = mul(output.worldPosition, shadowCameraViewProjections[2]);
    
    return output;
}
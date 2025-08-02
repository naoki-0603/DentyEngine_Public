#include "Mesh.hlsli"
#include "../Light.hlsli"
#include "../../Constants.hlsli"

ShadowVSOutput main(VSInput input)
{
    ShadowVSOutput output;

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

    // clip space.
    output.position = mul(input.position, mul(world, shadowCameraViewProjection));

    return output;
}

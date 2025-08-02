#include "Shadow.hlsli"

#include "SkinnedMeshShadow.hlsli"

#include "../../../../Constants.hlsli"

ShadowVSOutput main( VSInput input )
{
    ShadowVSOutput output;
    
    float4 blendedPosition = { 0, 0, 0, 1 };

	for (int boneIndex = 0; boneIndex < 4; ++boneIndex)
    {
        blendedPosition += input.boneWeights[boneIndex] *
            mul(input.position, shadowBoneTransforms[input.boneIndices[boneIndex]]);
    }

    output.position = mul(blendedPosition, mul(shadowWorld, shadowCameraViewProjection));
    output.worldPosition = mul(blendedPosition, shadowWorld);

    return output;
}
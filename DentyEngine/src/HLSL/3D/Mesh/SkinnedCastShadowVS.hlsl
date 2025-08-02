//#include "SkinnedMesh.hlsli"
//#include "Constants.hlsli"
//#include "Light.hlsli"

//float4 main( VSInput input ) : SV_POSITION
//{
//    float4 blendedPosition = { 0, 0, 0, 1 };
    
//    if (hasBone)
//    {
//        for (int boneIndex = 0; boneIndex < 4; ++boneIndex)
//        {
//            blendedPosition += input.boneWeights[boneIndex]
//            * mul(input.position, boneTransforms[input.boneIndices[boneIndex]]);
//        }
//    }
//    else
//    {
//        blendedPosition = input.position;
//        blendedPosition.w = 1.0f;
//    }
    
//    return float4(mul(blendedPosition, mul(world, directionalLight.lightViewProjection)));

//}

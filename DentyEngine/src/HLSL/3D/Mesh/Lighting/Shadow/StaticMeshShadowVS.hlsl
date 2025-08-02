#include "StaticMeshShadow.hlsli"
#include "Shadow.hlsli"
#include "../../../../Constants.hlsli"

ShadowVSOutput main( VSInput input )
{
    ShadowVSOutput shadowVsOutput = (ShadowVSOutput) 0;
    shadowVsOutput.position = mul(input.position, mul(shadowWorld, shadowCameraViewProjection));
    shadowVsOutput.worldPosition = mul(input.position, shadowWorld);

    return shadowVsOutput;
}
#include "StaticMesh.hlsli"

#include "../../Constants.hlsli"

VSOutput main(VSInput input)
{
    VSOutput output;
    
    output.position = mul(input.position, mul(world, collisionVisualizerCameraViewProjection));
    output.worldPosition = mul(input.position, world);
    
    input.normal.w = 0.0f;
    input.tangent.w = 0.0f;
    input.binormal.w = 0.0f;
    
    output.worldNormal = normalize(mul(input.normal, world));
    output.worldTangent = normalize(mul(input.tangent, world));
    output.worldBinormal = normalize(mul(input.binormal, world));
    
    output.color = float4(1, 1, 1, 1);
    output.texcoord = input.texcoord;
    output.positionInLVP = float4(0, 0, 0, 1);
    
    output.entityID = entityID;
    
    return output;
}

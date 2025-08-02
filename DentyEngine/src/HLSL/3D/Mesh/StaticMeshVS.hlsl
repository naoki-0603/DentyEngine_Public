#include "StaticMesh.hlsli"

#include "../../Constants.hlsli"

VSOutput main(VSInput input)
{
    VSOutput output;
    
    output.position = mul(input.position, mul(world, viewProjection));
    output.worldPosition = mul(input.position, world);
    
    input.normal.w = 0.0f;
    input.tangent.w = 0.0f;
    input.binormal.w = 0.0f;
    
    output.worldNormal = normalize(mul(input.normal, world));
    output.worldTangent = normalize(mul(input.tangent, world));
    output.worldBinormal = normalize(mul(input.binormal, world));
    
    output.color = float4(1, 1, 1, 1);
    output.texcoord = input.texcoord;
    
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

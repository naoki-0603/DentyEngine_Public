#include "PhotoMesh.hlsli"

#include "../../Constants.hlsli"

VSOutput main( StaticVSInput input )
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
    
    output.color = albedoColor;
    output.texcoord = input.texcoord;

    return output;
}
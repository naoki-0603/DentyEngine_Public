#include "StaticMesh.hlsli"

#include "Textures.hlsli"

#include "../../Constants.hlsli"

SamplerState samplerStates[3] : register(s0);
SamplerComparisonState comparisonSamplerState : register(s4);

Texture2D textureMaps[8] : register(t0);
Texture2D shadowMap : register(t20);

PSOutput main(VSOutput pin)
{
    PSOutput output;

    // Albedo
    const float4 baseColor = TextureFunctions::ComputeOrGetBaseColor(
		textureMaps[MATERIAL_ALBEDO_TEXTURE_INDEX], samplerStates[MATERIAL_SAMPLER_STATE_POINT],
		material.baseColor, pin.texcoord, material.baseRate, material.texturesUsed.albedo
    );

    output.color = baseColor;
    output.entityID = pin.entityID;

    return output;
}

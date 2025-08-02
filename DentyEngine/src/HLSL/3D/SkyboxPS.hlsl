#include "Skybox.hlsli"

#include "../Constants.hlsli"

TextureCube cubeMap : register(t0);
SamplerState samplers[3] : register(s0);

float4 main(PSInput pin) : SV_TARGET
{
    float3 texcoord = pin.worldPosition.xyz;
    
    return float4(cubeMap.SampleLevel(
        samplers[MATERIAL_SAMPLER_STATE_POINT], texcoord, 0.0f).rgb,
    1.0f);
}
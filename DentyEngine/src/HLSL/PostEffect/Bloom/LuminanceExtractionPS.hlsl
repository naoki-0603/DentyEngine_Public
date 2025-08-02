#include "../FullScreenQuad.hlsli"

Texture2D sceneTexture : register(t0);
SamplerState pointSamplerState : register(s0);

float4 main(VSOutput output) : SV_TARGET
{
    const float3 eachColorBrightness = float3(0.2125f, 0.7154f, 0.0721f);

    // Sampling color.
    const float4 color = sceneTexture.Sample(pointSamplerState, output.texcoord);

    // Calculate brightness.
    const float brightness = dot(color.rgb, eachColorBrightness);

    // If brightness less than 1.0f, clip it.
    clip(brightness - 1.0f);

    return color;
}
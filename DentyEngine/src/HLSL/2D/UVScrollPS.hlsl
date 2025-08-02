#include "UVScroll.hlsli"

Texture2D colorMap : register(t0);
SamplerState samplerState : register(s1);

float4 main(VSOutput pin) : SV_TARGET
{
    return float4(colorMap.Sample(samplerState, pin.texcoord).rgb, 1.0f) * pin.color;
}
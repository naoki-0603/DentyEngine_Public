#include "FullScreenQuad.hlsli"

Texture2D buffer : register(t0);
SamplerState pointSamplerState : register(s0);

float4 main(VSOutput vout) : SV_TARGET
{
    return buffer.Sample(pointSamplerState, vout.texcoord);
}
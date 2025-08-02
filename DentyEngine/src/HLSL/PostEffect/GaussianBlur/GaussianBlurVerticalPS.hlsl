#include "GaussianBlur.hlsli"
#include "../FullScreenQuad.hlsli"

Texture2D baseTexture : register(t0);
SamplerState anisotropicSampler : register(s0);

float4 main(VSOutput vout) : SV_TARGET
{
    float4 color = float4(0, 0, 0, 1);

    // Get base texture information.
    float2 texSize;
    float mipMapLevel;

    baseTexture.GetDimensions(0, texSize.x, texSize.y, mipMapLevel);

    // Base texcoord.
    float2 baseTexcoord = vout.texcoord;

    // Calculate sampling texcoords.
    SamplingTexcoords samplingTexcoords;

    // Up side

    // Base + 1
    samplingTexcoords.texcoord0.xy = float2(0.0f, 1.0f / texSize.y);

    // Base + 3
    samplingTexcoords.texcoord1.xy = float2(0.0f, 3.0f / texSize.y);

    // Base + 5
    samplingTexcoords.texcoord2.xy = float2(0.0f, 5.0f / texSize.y);

    // Base + 7
    samplingTexcoords.texcoord3.xy = float2(0.0f, 7.0f / texSize.y);

    // Base + 9
    samplingTexcoords.texcoord4.xy = float2(0.0f, 9.0f / texSize.y);

    // Base + 11
    samplingTexcoords.texcoord5.xy = float2(0.0f, 11.0f / texSize.y);

    // Base + 13
    samplingTexcoords.texcoord6.xy = float2(0.0f, 13.0f / texSize.y);

    // Base + 15
    samplingTexcoords.texcoord7.xy = float2(0.0f, 15.0f / texSize.y);

    // Down side.
    samplingTexcoords.texcoord0.zw = samplingTexcoords.texcoord0.xy * -1;
    samplingTexcoords.texcoord1.zw = samplingTexcoords.texcoord1.xy * -1;
    samplingTexcoords.texcoord2.zw = samplingTexcoords.texcoord2.xy * -1;
    samplingTexcoords.texcoord3.zw = samplingTexcoords.texcoord3.xy * -1;
    samplingTexcoords.texcoord4.zw = samplingTexcoords.texcoord4.xy * -1;
    samplingTexcoords.texcoord5.zw = samplingTexcoords.texcoord5.xy * -1;
    samplingTexcoords.texcoord6.zw = samplingTexcoords.texcoord6.xy * -1;
    samplingTexcoords.texcoord7.zw = samplingTexcoords.texcoord7.xy * -1;

    // Calculate actual texcoords.
    // Base + calculated texcoords.
    samplingTexcoords.texcoord0 += float4(baseTexcoord, baseTexcoord);
    samplingTexcoords.texcoord1 += float4(baseTexcoord, baseTexcoord);
    samplingTexcoords.texcoord2 += float4(baseTexcoord, baseTexcoord);
    samplingTexcoords.texcoord3 += float4(baseTexcoord, baseTexcoord);
    samplingTexcoords.texcoord4 += float4(baseTexcoord, baseTexcoord);
    samplingTexcoords.texcoord5 += float4(baseTexcoord, baseTexcoord);
    samplingTexcoords.texcoord6 += float4(baseTexcoord, baseTexcoord);
    samplingTexcoords.texcoord7 += float4(baseTexcoord, baseTexcoord);

    // Calculate color.
    // Up direction.
    color += weights[0].x * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord0.xy);
    color += weights[0].y * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord1.xy);
    color += weights[0].z * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord2.xy);
    color += weights[0].w * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord3.xy);
    color += weights[1].x * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord4.xy);
    color += weights[1].y * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord5.xy);
    color += weights[1].z * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord6.xy);
    color += weights[1].w * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord7.xy);

    // Down direction.
    color += weights[0].x * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord0.zw);
    color += weights[0].y * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord1.zw);
    color += weights[0].z * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord2.zw);
    color += weights[0].w * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord3.zw);
    color += weights[1].x * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord4.zw);
    color += weights[1].y * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord5.zw);
    color += weights[1].z * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord6.zw);
    color += weights[1].w * baseTexture.Sample(anisotropicSampler, samplingTexcoords.texcoord7.zw);

    return float4(color.rgb, 1.0f);
}
#include "ExpandedImage.hlsli"

Texture2D colorMap : register(t0);
SamplerState samplerState : register(s0);

float4 main(VSOutput pin) : SV_TARGET
{
    float4 convertedColor = float4(0, 0, 0, 0);
    float4 color = colorMap.Sample(samplerState, pin.texcoord);

    if (redElementEnabled)
    {
        convertedColor.r = color.r;
    }

    if (greenElementEnabled)
    {
        convertedColor.g = color.g;
    }

    if (blueElementEnabled)
    {
        convertedColor.b = color.b;
    }

    if (alphaElementEnabled)
    {
        convertedColor.a = color.a;
    }

    return convertedColor;
}
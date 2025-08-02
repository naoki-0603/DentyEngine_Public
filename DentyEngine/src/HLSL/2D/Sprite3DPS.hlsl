#include "Sprite3D.hlsli"

Texture2D colorMap : register(t0);
SamplerState samplerState : register(s0);

PSOutput main(VSOutput pin)
{
    PSOutput output = (PSOutput)0;

    const float4 textureColor = colorMap.Sample(samplerState, pin.texcoord);
    const float alpha = textureColor.a;

    output.color = float4(color.rgb, alpha);
    output.entityID = id;
    
    return output;
}
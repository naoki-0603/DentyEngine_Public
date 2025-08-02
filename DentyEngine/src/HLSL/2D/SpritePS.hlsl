#include "Sprite.hlsli"

#include "../Constants.hlsli"

Texture2D colorMap : register(t0);
SamplerState samplerState : register(s0);

PSOutput main(VSOutput pin)
{
    PSOutput output;
    
    float4 color = colorMap.Sample(samplerState, pin.texcoord);
    
    float alpha = color.a;
    
    // Inverse gamma process
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);

    output.color = float4(color.rgb, alpha);
    output.entityID = entityID;
    
    return output;
}
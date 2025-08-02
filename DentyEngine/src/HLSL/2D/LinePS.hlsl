#include "Line.hlsli"

float4 main(VSOutput pin) : SV_TARGET0
{
    return float4(pin.color.rgb, 1.0f);
}
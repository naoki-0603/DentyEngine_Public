#include "Grid.hlsli"

float4 main(VSOutput output) : SV_TARGET
{
    clip(output.color.a - 0.01f);

    return output.color;
}
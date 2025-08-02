#include "Shadow.hlsli"

// Variance shadow map.
float4 main(ShadowVSOutput output) : SV_TARGET
{
    const float depth = output.position.z;

    return float4(depth, depth * depth, 0.0f, 1.0f);
}
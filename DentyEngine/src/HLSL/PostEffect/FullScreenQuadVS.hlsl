#include "FullScreenQuad.hlsli"

VSOutput main(uint id : SV_VERTEXID)
{
    VSOutput output;

    const float2 positions[4] = { { -1, +1 }, { +1, +1 }, { -1, -1 }, { +1, -1 } };
    const float2 texcoords[4] = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };

    output.position = float4(positions[id], 0.0f, 1.0f);
    output.texcoord = texcoords[id];

    return output;
}
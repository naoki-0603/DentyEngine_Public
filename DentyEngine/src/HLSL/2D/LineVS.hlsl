#include "Line.hlsli"

#include "../Constants.hlsli"

VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    output.position = mul(input.position, collisionVisualizerCameraViewProjection);
    output.color = input.color;

    return output;
}
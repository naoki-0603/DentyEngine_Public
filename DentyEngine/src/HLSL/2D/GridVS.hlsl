#include "Grid.hlsli"

#include "../Constants.hlsli"

VSOutput main(float4 position : POSITION)
{
    VSOutput output;
    output.position = mul(position, viewProjection);
    output.color = float4(0, 0, 0, 0);

    const float cameraPositionY = abs(cameraPosition.y);
    if (cameraPositionY < VISIBLE_GRID_HEIGHT_MAX)
    {
        output.color.rgb = 0.5f;
        output.color.a = saturate(0.5f + cameraPositionY / VISIBLE_GRID_HEIGHT_MAX);
    }

    return output;
}
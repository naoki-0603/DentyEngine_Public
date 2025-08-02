#include "ExpandedImage.hlsli"

VSOutput main( VSInput input )
{
    VSOutput output = (VSOutput) 0;

    const float4 position = float4(input.position.x, input.position.y, 0.0f, 1.0f);
    output.position = mul(position, projectionMatrix);
	output.color = input.color;
    output.texcoord = input.texcoord;

    return output;
}
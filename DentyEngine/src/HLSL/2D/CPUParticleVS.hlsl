#include "CPUParticle.hlsli"
#include "../Constants.hlsli"

VSOutput main(VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(input.position, mul(world, viewProjection));
    output.color = color;
	output.texcoord = input.texcoord;

	return output;
}
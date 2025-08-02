#include "Sprite.hlsli"

#include "../Constants.hlsli"

VSOutput main(VSInput input)
{
    VSOutput output;
    
    output.position = input.position;
    output.color = input.color;
    output.texcoord = input.texcoord;

    return output;
}

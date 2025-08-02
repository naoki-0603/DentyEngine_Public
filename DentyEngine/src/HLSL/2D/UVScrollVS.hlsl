#include "UVScroll.hlsli"

VSOutput main(VSInput vin)
{
    VSOutput vout;
    vout.position = vin.position;
    vout.color = vin.color;
    vout.texcoord = vin.texcoord + uvScrollValue.z;
    
    return vout;
}
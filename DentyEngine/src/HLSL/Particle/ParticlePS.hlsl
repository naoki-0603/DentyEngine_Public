#include "Particle.hlsli"

[earlydepthstencil]
float4 main(GSOut pin) : SV_TARGET
{
    return pin.color;
}
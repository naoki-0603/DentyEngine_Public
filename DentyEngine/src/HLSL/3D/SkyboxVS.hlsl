#include "Skybox.hlsli"

#include "../Constants.hlsli"

PSInput main(VSInput vin)
{
    PSInput pin;
    pin.position = mul(vin.position, mul(world, viewProjection));
    pin.worldPosition = mul(vin.position, world);
    
    return pin;
}
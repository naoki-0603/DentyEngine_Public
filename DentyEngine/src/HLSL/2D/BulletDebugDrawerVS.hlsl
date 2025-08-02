#include "BulletDebugDrawer.hlsli"

VSOutput main( float4 position : POSITION, float4 color : COLOR)
{
    VSOutput vout;

    // Bullet is a right-handed type
    vout.position = mul(position, viewProjection);    

    vout.color = color;

	return vout;
}
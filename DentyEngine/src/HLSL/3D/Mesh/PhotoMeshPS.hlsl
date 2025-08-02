#include "PhotoMesh.hlsli"

#include "../../Constants.hlsli"
#include "../../ConstantValues.hlsli"

#include "../Light.hlsli"
#include "../Lighting.hlsli"

SamplerState samplerStates[3] : register(s0);
Texture2D textureMaps[2] : register(t0);

PSOutput main(VSOutput pin)
{
    PSOutput output;

	// Base color.
    float4 baseColor = float4(albedoColor.rgb, 1.0f);

    // Has textures.
	{
	     // has albedo texture.
        if (textureUsed.albedoMap)
        {
            baseColor = float4(textureMaps[0].Sample(samplerStates[SAMPLER_STATE_POINT], pin.texcoord) * albedoColor.rgb, 1.0f);
        }
    }

    // Normalized
    float3 worldNormal = normalize(pin.worldNormal).xyz;

    // Takes values from 0.0f ~ 1.0f.
    float3 diffuseTotal = float3(0, 0, 0);

	// Calculate directional light
	{
        const float3 lightDirection = normalize(float3(1, -1, 0));

        diffuseTotal += CalcHalfLambertDiffuse(worldNormal, lightDirection, float4(1, 1, 1, 1));
    }

    output.color = float4(baseColor.rgb * diffuseTotal, 1.0f);
    return output;
}
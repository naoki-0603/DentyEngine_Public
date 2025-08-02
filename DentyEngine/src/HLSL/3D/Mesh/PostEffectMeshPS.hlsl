#include "PostEffectMesh.hlsli"

#include "../../Constants.hlsli"
#include "../../ConstantValues.hlsli"

#include "../Light.hlsli"
#include "../Lighting.hlsli"

SamplerState samplerStates[3] : register(s0);

PSOutput main(VSOutput pin)
{
    PSOutput output;

	// Base color.
    float4 baseColor = albedoColor;

    // Normalized
    float3 worldNormal = normalize(pin.worldNormal).xyz;

    // View direction
    const float3 viewDirection = normalize(pin.worldPosition.xyz - postEffectCameraPosition.xyz);

    // Takes values from 0.0f ~ 1.0f.
    float3 diffuseTotal = float3(0, 0, 0);
    float3 specularTotal = float3(0, 0, 0);
    const float3 ambientTotal = float3(.2f, .2f, .2f);

	// Calculate directional light
	{
        const float3 lightDirection = normalize(directionalLight.direction.xyz);

        diffuseTotal += CalcHalfLambertDiffuse(worldNormal, lightDirection, directionalLight.color.rgb);
        specularTotal += CalcBlinnPhongSpecular(worldNormal, lightDirection, viewDirection, directionalLight.color.rgb);
    }

    output.color = float4(baseColor * diffuseTotal + specularTotal + ambientTotal, 1.0f);

    return output;
}
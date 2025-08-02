#include "StaticMesh.hlsli"
#include "Textures.hlsli"

#include "Lighting/Lighting.hlsli"
#include "Lighting/Shadow/Shadow.hlsli"

#include "../../Constants.hlsli"

SamplerState samplerStates[3] : register(s0);

Texture2D textureMaps[8] : register(t0);
Texture2D shadowMap : register(t20);
TextureCube skyboxCubeMap : register(t30);

PSOutput main(VSOutput pin)
{
    PSOutput output;

    // Albedo
    const float4 baseColor = TextureFunctions::ComputeOrGetBaseColor(
		textureMaps[MATERIAL_ALBEDO_TEXTURE_INDEX], samplerStates[MATERIAL_SAMPLER_STATE_POINT],
		material.baseColor, pin.texcoord, material.baseRate, material.texturesUsed.albedo
    );

    // Metalness
    const float metalness = TextureFunctions::ComputeOrGetMetalness(
		textureMaps[MATERIAL_METALNESS_TEXTURE_INDEX], samplerStates[MATERIAL_SAMPLER_STATE_POINT],
		pin.texcoord, material.metallic, material.texturesUsed.metalness
    );

    // Normal
    const float3 worldNormal = TextureFunctions::ComputeOrGetNormal(
		textureMaps[MATERIAL_NORMAL_TEXTURE_INDEX], samplerStates[MATERIAL_SAMPLER_STATE_POINT], 
		pin.worldTangent.xyz, pin.worldBinormal.xyz, pin.worldNormal.xyz,
		pin.texcoord, material.texturesUsed.normal
    );

    // Roughness
    const float roughness = TextureFunctions::ComputeOrGetRoughness(
		textureMaps[MATERIAL_ROUGHNESS_TEXTURE_INDEX], samplerStates[MATERIAL_SAMPLER_STATE_POINT],
		pin.texcoord, material.roughness, material.texturesUsed.roughness
    );

    // Emissive
    const float4 emissive = TextureFunctions::ComputeOrGetEmissive(
		textureMaps[MATERIAL_EMISSIVE_TEXTURE_INDEX], samplerStates[MATERIAL_SAMPLER_STATE_POINT],
		material.emissiveColor, pin.texcoord, material.texturesUsed.emissive
    );

    // AmbientOcclusion
    const float4 ambientOcclusion = TextureFunctions::GetAmbientOcclusion(
		textureMaps[MATERIAL_AMBIENT_OCCLUSION_TEXTURE_INDEX],
		samplerStates[MATERIAL_SAMPLER_STATE_POINT],
		pin.texcoord, material.texturesUsed.ambientOcclusion
    );

    // Reflection
    const float3 diffuseReflectance = lerp(baseColor.rgb, 0.02f, metalness);

    // Fresnel
    const float3 F0 = lerp(DIELECTRIC, baseColor.rgb, metalness);

    // Eye vector.
    const float3 eyeVector = normalize(cameraPosition.xyz - pin.worldPosition.xyz);

    float3 directDiffuse = 0, directSpecular = 0;

    // Directional light.
    {
        DirectionalLightBRDFParameters parameters = (DirectionalLightBRDFParameters)0;
        parameters.direction = normalize(directionalLight.direction.xyz);
        parameters.eyeVector = eyeVector;
        parameters.reflectance = diffuseReflectance;
        parameters.lightColor = directionalLight.color.rgb;
        parameters.f0 = F0;
        parameters.normal = worldNormal;
        parameters.roughness = roughness;

        const BRDFResult result = DirectionalLightFunctions::ComputeBRDF(parameters);
        directDiffuse = result.diffuse;
        directSpecular = result.specular;
    }

    // Point lights.
    //    [unroll]
    //    for (int index = 0; index < pointLightCount; ++index)
    //    {
    //        const PointLightConstant pointLight = pointLights[index];

    //        PointLightFunctions::BRDFParameters parameters = { };
    //        parameters.lightPosition = pointLight.position.xyz;
    //        parameters.worldPosition = pin.worldPosition.xyz;
    //        parameters.lightColor = pointLight.color.rgb;
    //        parameters.reflectance = diffuseReflectance;
    //        parameters.


    //        diffuseTotal += PointLightFunctions::ComputeToonDiffuse(rampTexture, samplerStates[SAMPLER_STATE_POINT],
				//pointLight.position.xyz, pin.worldPosition.xyz, worldNormal, baseColor.rgb,
				//pointLight.color.rgb, diffuseFactor);

    //        specularTotal += PointLightFunctions::ComputeBlinnPhongSpecular(
				//pointLight.position.xyz, worldNormal, pin.worldPosition.xyz, cameraPosition.xyz,
				//pointLight.color.rgb, DEFAULT_SPECULAR_SHININESS
    //        );
    //    }

    // Spot lights.
    //    // https://www.braynzarsoft.net/viewtutorial/q16390-21-spotlights
    //    [unroll]
    //    for (int index = 0; index < spotLightCount; ++index)
    //    {
    //        const SpotLightConstant spotLight = spotLights[index];

    //        diffuseTotal += SpotLightFunctions::ComputeToonDiffuse(
				//rampTexture, samplerStates[SAMPLER_STATE_POINT],
				//spotLight.theta, spotLight.phi, 
				//spotLight.position.xyz, pin.worldPosition.xyz, spotLight.direction.xyz, worldNormal, baseColor.rgb,
				//spotLight.color.rgb, diffuseFactor
    //        );

    //        specularTotal += PointLightFunctions::ComputeBlinnPhongSpecular(
				//spotLight.position.xyz, worldNormal, pin.worldPosition.xyz, cameraPosition.xyz,
				//spotLight.color.rgb, DEFAULT_SPECULAR_SHININESS
    //        );
    //    }

    // Hemisphere light
    float3 irradiance = 0;
    float3 indirectDiffuse = 0;
    {
        if (material.influenceHemisphereLight == 1)
        {
			irradiance = HemisphereLight::ComputeLight(worldNormal, hemisphereLight.skyDirection, hemisphereLight.skyColor, hemisphereLight.groundColor);
        }
        else
        {
            irradiance = 1;
        }

        indirectDiffuse = ComputeDiffuseBRDF(diffuseReflectance) * irradiance;
    }

    float3 indirectSpecular = 0;
    {
        float3 texcoord = reflect(eyeVector, worldNormal);

        uint width, height, mipMapLevel;
        skyboxCubeMap.GetDimensions(0, width, height, mipMapLevel);
        float mipLevel = lerp(0, mipMapLevel - 1, roughness);

        float3 raddiance = skyboxCubeMap.SampleLevel(samplerStates[MATERIAL_SAMPLER_STATE_ANISOTROPIC], texcoord, mipLevel);

        indirectSpecular = EnvBRDFApprox(F0, roughness, dot(worldNormal, eyeVector)) * raddiance;
    }

    directDiffuse = saturate(directDiffuse);
    directSpecular = saturate(directSpecular);

    // Emissive + diffuse + specular 
    float4 resultColor = float4((emissive.rgb * material.emissiveIntensity) + directDiffuse + directSpecular + ((indirectDiffuse + indirectSpecular) * ambientOcclusion.rgb), 1.0f);

    // Variance shadow process.
	resultColor = VarianceShadow::ShadowProcess(resultColor, pin.positionInLVP, shadowMap, samplerStates[MATERIAL_SAMPLER_STATE_ANISOTROPIC]);

    output.color = resultColor;
    output.entityID = pin.entityID;

    return output;
}

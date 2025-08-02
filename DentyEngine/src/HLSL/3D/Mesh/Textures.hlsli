struct TextureFunctions
{
	static float4 ComputeOrGetBaseColor(Texture2D albedoMap, SamplerState samplerState, float4 baseColor, float2 texcoord, float baseRate, int albedoMapUsed)
	{
        if (albedoMapUsed != 1)
            return baseColor;

        return albedoMap.Sample(samplerState, texcoord) * (baseColor * baseRate);
    }

	static float ComputeOrGetMetalness(Texture2D metalnessMap, SamplerState samplerState, float2 texcoord, float metalness, int metalnessMapUsed)
	{
		if (metalnessMapUsed != 1)
            return metalness;

        return metalnessMap.Sample(samplerState, texcoord);
    }

	// If normal map exists, compute world normal. otherwise return world normal.
	static float3 ComputeOrGetNormal(
		Texture2D normalMap, SamplerState samplerState,
		float3 worldTangent, float3 worldBinormal, 
		float3 worldNormal, float2 texcoord, int normalMapUsed
    )
	{
        if (normalMapUsed != 1)
            return worldNormal;

        // Tangent space
        float3 normalInTangentSpace = normalMap.Sample(samplerState, texcoord);

		// Convert to -1.0f ~ +1.0f
        normalInTangentSpace = (normalInTangentSpace * 2.0f) - 1.0f;

		// Convert to world space.
        const float3x3 tangentSpaceToWorld = float3x3(worldTangent, worldBinormal, worldNormal);

		return normalize(mul(normalInTangentSpace, tangentSpaceToWorld));
    }

    static float ComputeOrGetRoughness(
		Texture2D roughnessMap, SamplerState samplerState,
		float2 texcoord, float roughness, int roughnessMapUsed
	)
	{
		if (roughnessMapUsed !=  1)
            return roughness;

        return roughnessMap.Sample(samplerState, texcoord);
    }

    static float4 ComputeOrGetEmissive(
		Texture2D emissiveMap, SamplerState samplerState,
		float4 emissive, float2 texcoord, int emissiveMapUsed
	)
	{
		if (emissiveMapUsed != 1)
            return emissive;

        return emissiveMap.Sample(samplerState, texcoord);
    }

    static float4 GetAmbientOcclusion(
		Texture2D ambientOcclusion, SamplerState samplerState,
		float2 texcoord, int ambientOcclusionMapUsed
	)
	{
		if (ambientOcclusionMapUsed != 1)
            return float4(1, 1, 1, 1);

        return ambientOcclusion.Sample(samplerState, texcoord);
    }
};
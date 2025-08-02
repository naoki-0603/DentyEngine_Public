//
// Constants
//
static const float3 SHADOW_COLOR = float3(0.5f, 0.5f, 0.5f);
static const float3 CASCADE_SHADOW_DEBUG_COLOR[3] =
{
    float3(1, 0, 0), float3(0, 1, 0), float3(0, 0, 1)
};

static const uint NUM_SHADOW_MAP = 3u;


//
// Structures
//

struct ShadowVSOutput
{
    float4 position : SV_Position;
    float4 worldPosition : POSITION;
};

//
// Variance shadow
//
struct VarianceShadow
{
	//
    // Functions
    //
    static bool IsInShadow(float2 shadowTexcoord)
    {
        return (shadowTexcoord.x >= 0.0f && shadowTexcoord.x <= 1.0f &&
			shadowTexcoord.y >= 0.0f && shadowTexcoord.y <= 1.0f);
    }

    static bool IsShadow(float depth, float shadowMapDepth)
    {
        return (depth >= shadowMapDepth && depth <= 1.0f);
    }

    static float CalculateDepth(float4 positionInLVP)
    {
        return positionInLVP.z / positionInLVP.w;
    }

    static float2 CalculateShadowTexcoord(float4 positionInLVP)
    {
        float2 texcoord = positionInLVP.xy / positionInLVP.w;

		// Convert to -1.0f ~ 1.0f to 0.0f ~ 1.0f
        texcoord *= float2(+0.5f, -0.5f);
        texcoord += 0.5f;

        return texcoord;
    }

    static float4 ShadowProcess(float4 color, float4 positionInLVP, Texture2D shadowMap, SamplerState wrapSampler)
    {
        const float2 shadowTexcoord = CalculateShadowTexcoord(positionInLVP);
        const float2 shadowMapDepth = shadowMap.Sample(wrapSampler, shadowTexcoord).xy;

        const float depth = CalculateDepth(positionInLVP);

        if (IsShadow(depth, shadowMapDepth.r))
        {
            const float depthSq = shadowMapDepth.r * shadowMapDepth.r;

            const float variance = min(max(shadowMapDepth.g - depthSq, 0.00001f), 1.0f);

            const float md = depth - shadowMapDepth.r;

            const float litFactor = variance / (variance + md * md);

            const float3 shadowColor = color.rgb * 0.5f;

            color.rgb = lerp(shadowColor, color.rgb, litFactor);
        }

        return color;
    }
};

//
// Cascade shadow
// 
struct CascadeShadow
{
    //
	// Functions
	//

    static bool IsInShadowMap(float2 shadowTexcoord)
    {
        return (shadowTexcoord.x >= 0.0f && shadowTexcoord.x <= 1.0f &&
			shadowTexcoord.y >= 0.0f && shadowTexcoord.y <= 1.0f);
    }

    static bool IsShadow(float depth, float shadowMapDepth)
    {
        return (depth >= shadowMapDepth);
    }

    static float CalculateDepth(float4 positionInLVP)
    {
        return positionInLVP.z / positionInLVP.w;
    }

    static float2 CalculateShadowTexcoord(float4 positionInLVP)
    {
        float2 texcoord = positionInLVP.xy / positionInLVP.w;

		// Convert to -1.0f ~ 1.0f to 0.0f ~ 1.0f
        texcoord *= float2(+0.5f, -0.5f);
        texcoord += 0.5f;

        return texcoord;
    }

    static float4 ShadowProcess(float4 color, float4 positionsInLVP[3], Texture2D shadowMaps[3], SamplerState wrapSampler)
    {
        for (int cascadeIndex = 0u; cascadeIndex < NUM_SHADOW_MAP; ++cascadeIndex)
        {
            const float depth = CalculateDepth(positionsInLVP[cascadeIndex]);
            if (depth >= 0.0f && depth <= 1.0f)
            {
                const float2 shadowMapTexcoord = CalculateShadowTexcoord(positionsInLVP[cascadeIndex]);
                if (IsInShadowMap(shadowMapTexcoord))
                {
                    const float2 shadowMapDepth = shadowMaps[cascadeIndex].Sample(wrapSampler, shadowMapTexcoord);
                    if (IsShadow(depth, shadowMapDepth.r))
                    {
                        return float4(color.rgb * SHADOW_COLOR, color.a);
                    }
                }
            }
        }

        return color;
    }

    static float4 ShadowProcessDebug(float4 color, float4 positionsInLVP[3], Texture2D shadowMaps[3], SamplerState wrapSampler)
    {
        for (int cascadeIndex = 0u; cascadeIndex < NUM_SHADOW_MAP; ++cascadeIndex)
        {
            const float depth = CalculateDepth(positionsInLVP[cascadeIndex]);
            if (depth >= 0.0f && depth <= 1.0f)
            {
                const float2 shadowMapTexcoord = CalculateShadowTexcoord(positionsInLVP[cascadeIndex]);
                if (IsInShadowMap(shadowMapTexcoord))
                {
                    const float2 shadowMapDepth = shadowMaps[cascadeIndex].Sample(wrapSampler, shadowMapTexcoord);
                    if (IsShadow(depth, shadowMapDepth.r))
                    {
                        return float4(color.rgb * CASCADE_SHADOW_DEBUG_COLOR[cascadeIndex], color.a);
                    }
                }
            }
        }

        return color;
    }
};

//// Z component include depth.
//float3 CalcShadowTexcoord(float4 worldPosition, row_major float4x4 lightViewProjection, float3 normal, float3 lightDirection, float shadowBias)
//{
//    float4 positionInLvp = mul(worldPosition, lightViewProjection);
//    float4 positionInNdc = positionInLvp / positionInLvp.w;

//    float2 lightViewUv = float2(positionInNdc.x * +0.5 + 0.5, positionInNdc.y * -0.5 + 0.5);

//    // Copy from 3dxgp shadow
//    const float shadowDepthBias = max(0.01 * (1.0 - dot(normal, lightDirection)), 0.001);

//    float depth = saturate(positionInNdc.z - shadowDepthBias);

//    return float3(lightViewUv.x, lightViewUv.y, depth);
//}

//float3 CalcShadowColor(Texture2D shadowMap, SamplerState samplerState, float3 shadowTexcoord)
//{
//    if (shadowTexcoord.x <= 0.0f || shadowTexcoord.x >= 1.0f || 
//        shadowTexcoord.y <= 0.0f || shadowTexcoord.y >= 1.0f)
//    {
//        return float3(1.0f, 1.0f, 1.0f);
//    }

//    float4 depthValues = shadowMap.Sample(samplerState, shadowTexcoord.xy);

//    const float depthSquared = depthValues.y;
//    const float depthTwoTimes = depthValues.x * depthValues.x;
//    const float variance = depthSquared - depthTwoTimes;
//    const float md = depthValues.x - shadowTexcoord.z;
//    const float md2 = md * md;
//    float p = variance / (variance + md2);
//    p = max(p, shadowTexcoord.z <= depthValues.x);

//    return clamp(p, 0.5f, 1.0f);
//}
#include "../../Utility.hlsli"

//
// Constants.
//

// Minimum reflection.
static const float DIELECTRIC = 0.04f;

//
// Functions.
//

float3 ComputeFresnel(float3 F0, float vDotH)
{
    return F0 + (1.0f - F0) * pow(1.0f - vDotH, 5.0f);
}

// GGX
float ComputeNormalDistributionFunction(float nDotH, float roughness)
{
    const float a2 = roughness * roughness;
    const float nDotH2 = nDotH * nDotH;
    const float numerator = a2;
	const float denominator = PI * pow(nDotH2 * (a2 - 1.0f) + 1, 2.0f);

    return numerator / denominator;
}

float ComputeGeometryFunction(float dot, float roughness)
{
    const float k = roughness;
    const float numerator = dot;
    const float denominator = dot * (1.0f - k) + k;

    return numerator / denominator;
}

float ComputeGeometryFunction(float nDotL, float nDotV, float roughness)
{
    const float r = roughness * 0.5f;

    const float shadowing = ComputeGeometryFunction(nDotL, r);
    const float masking = ComputeGeometryFunction(nDotV, r);

    return shadowing * masking;
}

float3 ComputeDiffuseBRDF(float3 diffuseReflectance)
{
    return diffuseReflectance / PI;
}

float3 ComputeSpecularBRDF(float nDotV, float nDotL, float nDotH, float vDotH, float3 fresnelF0, float roughness)
{
	// Distribution
    const float D = ComputeNormalDistributionFunction(nDotH, roughness);

    // Geometry
    const float G = ComputeGeometryFunction(nDotL, nDotV, roughness);

    // Fresnel
    const float3 F = ComputeFresnel(fresnelF0, vDotH);

    const float3 numerator = D * G * F;
    const float denominator = 4.0f * nDotL * nDotV;

    return numerator / denominator;
}

void ComputeDirectBRDF(
	float3 diffuseReflectance, float3 f0, float3 normal, float3 eyeVector,
	float3 lightVector, float3 lightColor, float roughness,
    out float3 outDiffuse, out float3 outSpecular
)
{
    const float3 N = normal;
    const float3 L = -lightVector;
    const float3 V = eyeVector;
    const float3 H = normalize(L + V);

    const float NDotV = max(0.0001f, dot(N, V));
    const float NDotL = max(0.0001f, dot(N, L));
    const float NDotH = max(0.0001f, dot(N, H));
    const float VDotH = max(0.0001f, dot(V, H));

    const float3 irradiance = lightColor * NDotL;

    outDiffuse = ComputeDiffuseBRDF(diffuseReflectance) * irradiance;
    outSpecular = ComputeSpecularBRDF(NDotV, NDotL, NDotH, VDotH, f0, roughness);
}

//
// IBL
//

// https://www.unrealengine.com/en-US/blog/physically-based-shading-on-mobile
float3 EnvBRDFApprox(float3 F0, float roughness, float NDotV)
{
    const float4 c0 = { -1.0f, -0.0275f, -0.572f, 0.022f };
    const float4 c1 = { 1.0f, 0.0425f, 1.04f, -0.04f };

	float4 r = roughness * c0 + c1;

	float a004 = min(r.x * r.x, exp2(-9.28f * NDotV)) * r.x + r.y;

	float2 AB = float2(-1.04, 1.04f) * a004 + r.zw;

	return F0 * AB.x + AB.y;
}
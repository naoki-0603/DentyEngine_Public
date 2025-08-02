#include "../../../HLSL/3D/Mesh/Lighting/PBR.hlsli"

//
// References 
//

// https://www.3dgep.com/texturing-lighting-directx-11/#Lighting
// https://araramistudio.jimdo.com/2017/09/08/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0-directx-11%E3%81%A7%E7%82%B9%E5%85%89%E6%BA%90%E3%83%A9%E3%82%A4%E3%83%86%E3%82%A3%E3%83%B3%E3%82%B0/
// https://sakataharumi.hatenablog.jp/

//
// Structures.
//
struct DirectionalLightBRDFParameters
{
    float3 direction;
    float3 eyeVector;
    float3 lightColor;
    float3 reflectance;
    float3 f0;
    float3 normal;
    float roughness;
};

struct BRDFResult
{
    float3 diffuse;
    float3 specular;
};

//
// Directional Light
//
struct DirectionalLightFunctions
{
    

    //
    // Compute diffuse functions
    //
    static float3 ComputeLambertDiffuse(
		float3 worldDirection, float3 worldNormal, float3 surfaceColor, float3 lightColor, float3 kd
    )
    {
        const float3 normalizedWorldNormal = normalize(worldNormal);
        const float3 normalizedWorldDirection = normalize(worldDirection) * -1.0f;

        const float3 diffuse = max(0, dot(normalizedWorldNormal, normalizedWorldDirection)) * lightColor * surfaceColor * kd;

    	return diffuse;
    }

    static float3 ComputeHalfLambertDiffuse(
		float3 worldDirection, float3 worldNormal, float3 surfaceColor, float3 lightColor, float3 kd
    )
    {
        const float3 normalizedWorldNormal = normalize(worldNormal);
        const float3 normalizedWorldDirection = normalize(worldDirection) * -1.0f;

        const float fixedDot = dot(normalizedWorldNormal, normalizedWorldDirection) * 0.5f + 0.5f;
        const float3 diffuse = fixedDot * lightColor * surfaceColor * kd;

        return diffuse;
    }

    static float3 ComputeToonDiffuse(
		Texture2D rampMap, SamplerState samplerState,
		float3 worldDirection, float3 worldNormal, float3 surfaceColor, float3 lightColor, float3 kd
    )
    {
        const float3 normalizedWorldNormal = normalize(worldNormal);
        const float3 normalizedWorldDirection = normalize(worldDirection) * -1.0f;

        const float texcoordX = dot(normalizedWorldNormal, normalizedWorldDirection) * 0.5f + 0.5f;

        return rampMap.Sample(samplerState, float2(texcoordX, 0)) * lightColor * surfaceColor * kd;
    }

    static BRDFResult ComputeBRDF(DirectionalLightBRDFParameters parameters)
    {
        float3 diffuse = 0, specular = 0;

        ComputeDirectBRDF(
			parameters.reflectance, parameters.f0, parameters.normal,
			parameters.eyeVector, parameters.direction,
			parameters.lightColor, parameters.roughness,
			diffuse, specular
        );

        BRDFResult result;
        result.diffuse = diffuse;
        result.specular = specular;

        return result;
    }

    //
    // Compute specular functions
    //
    static float3 ComputePhongSpecular(
		float3 worldDirection, float3 worldNormal, float3 worldPosition,
		float3 viewPosition, float3 lightColor, float shininess
    )
    {
        const float3 normalizedWorldNormal = normalize(worldNormal);
        const float3 normalizedWorldDirection = normalize(worldDirection) * -1.0f;
        const float3 normalizedToView = normalize(worldPosition - viewPosition);
        const float3 normalizedReflection = normalize(reflect(normalizedWorldDirection, normalizedWorldNormal));
        const float specularIntensity = pow(max(0, dot(normalizedReflection, normalizedToView)), shininess);

        return lightColor * specularIntensity;
    }

    static float3 ComputeBlinnPhongSpecular(
		float3 worldDirection, float3 worldNormal, float3 worldPosition,
		float3 viewPosition, float3 lightColor, float shininess
	)
    {
        const float3 normalizedWorldNormal = normalize(worldNormal);
        const float3 normalizedWorldDirection = normalize(worldDirection) * -1.0f;
        const float3 normalizedToView = normalize(worldPosition - viewPosition);
        const float3 normalizedHalfVector = normalize(normalizedToView + normalizedWorldDirection);
        const float specularIntensity = pow(max(0, dot(normalizedWorldNormal, normalizedHalfVector)), shininess);

        return lightColor * specularIntensity;
    }
};

//
// Point light
//
struct PointLightFunctions
{
    //
    // Compute diffuse functions.
    //
    static float3 ComputeLambertDiffuse(
		float3 lightWorldPosition, float3 worldPosition, float3 worldNormal, float3 surfaceColor, float3 lightColor, float3 kd
    )
    {
        const float3 worldDirection = worldPosition - lightWorldPosition;
        const float lengthLightToVertexInWorld = abs(length(worldDirection));
        const float3 normalizedWorldNormal = normalize(worldNormal);
        const float3 normalizedWorldDirection = normalize(worldDirection) * -1.0f;

        const float attenuation = saturate(1.0f / (0.0f + 1.0f / lengthLightToVertexInWorld + 0.0f / (lengthLightToVertexInWorld * lengthLightToVertexInWorld)));
        const float3 diffuse = max(0, dot(normalizedWorldNormal, normalizedWorldDirection)) * lightColor * surfaceColor * kd;

        return diffuse * attenuation;
    }

    static float3 ComputeHalfLambertDiffuse(
		float3 lightWorldPosition, float3 worldPosition, float3 worldNormal, float3 surfaceColor, float3 lightColor, float3 kd
    )
    {
        const float3 worldDirection = worldPosition - lightWorldPosition;
        const float lengthLightToVertexInWorld = abs(length(worldDirection));
        const float3 normalizedWorldNormal = normalize(worldNormal);
        const float3 normalizedWorldDirection = normalize(worldDirection) * -1.0f;

        const float attenuation = saturate(1.0f / (0.0f + 1.0f / lengthLightToVertexInWorld + 0.0f / (lengthLightToVertexInWorld * lengthLightToVertexInWorld)));

    	const float fixedDot = dot(normalizedWorldNormal, normalizedWorldDirection) * 0.5f + 0.5f;
        const float3 diffuse = fixedDot * lightColor * surfaceColor * kd;

        return diffuse * attenuation;
    }

    static float3 ComputeToonDiffuse(
		Texture2D rampMap, SamplerState samplerState,
		float3 lightWorldPosition, float3 worldPosition, float3 worldNormal, float3 surfaceColor, float3 lightColor, float3 kd
    )
    {
        const float3 worldDirection = worldPosition - lightWorldPosition;
        const float lengthLightToVertexInWorld = abs(length(worldDirection));
        const float3 normalizedWorldNormal = normalize(worldNormal);
        const float3 normalizedWorldDirection = normalize(worldDirection) * -1.0f;

        const float attenuation = saturate(1.0f / (0.0f + 1.0f / lengthLightToVertexInWorld + 0.0f / (lengthLightToVertexInWorld * lengthLightToVertexInWorld)));

        const float texcoordX = dot(normalizedWorldNormal, normalizedWorldDirection) * 0.5f + 0.5f;
        const float3 diffuse = rampMap.Sample(samplerState, float2(texcoordX, 0.0f)) * lightColor * surfaceColor * kd;

        return diffuse * attenuation;
    }

    //
    // Compute specular functions
    //
    static float3 ComputePhongSpecular(
		float3 lightWorldPosition, float3 worldNormal, float3 worldPosition,
		float3 viewPosition, float3 lightColor, float shininess
    )
    {
        const float3 worldDirection = worldPosition - lightWorldPosition;
        const float3 normalizedWorldNormal = normalize(worldNormal);
        const float3 normalizedWorldDirection = normalize(worldDirection) * -1.0f;
        const float3 normalizedToView = normalize(worldPosition - viewPosition);
        const float3 normalizedReflection = normalize(reflect(normalizedWorldDirection, normalizedWorldNormal));
        const float specularIntensity = pow(max(0, dot(normalizedReflection, normalizedToView)), shininess);

        return lightColor * specularIntensity;
    }

    static float3 ComputeBlinnPhongSpecular(
		float3 lightWorldPosition, float3 worldNormal, float3 worldPosition,
		float3 viewPosition, float3 lightColor, float shininess
    )
    {
        const float3 worldDirection = worldPosition - lightWorldPosition;
        const float3 normalizedWorldNormal = normalize(worldNormal);
        const float3 normalizedWorldDirection = normalize(worldDirection) * -1.0f;
        const float3 normalizedToView = normalize(worldPosition - viewPosition);
        const float3 normalizedHalfVector = normalize(normalizedToView + normalizedWorldDirection);
        const float specularIntensity = pow(max(0, dot(normalizedWorldNormal, normalizedHalfVector)), shininess);

        return lightColor * specularIntensity;
    }
};

//
// SpotLight
//
struct SpotLightFunctions
{
    //
    // Compute diffuse functions
    //
    static float3 ComputeLambertDiffuse(
		const float theta, const float phi,
		float3 lightWorldPosition, float3 worldPosition, float3 lightDirection, float3 worldNormal, float3 surfaceColor, float3 lightColor, float3 kd
    )
    {
        const float3 normalizedLightDirection = normalize(lightDirection);
        const float3 normalizedWorldNormal = normalize(worldNormal);

        const float alphaAngle = ComputeAngleBetweenLightDirectionAndToVertexInWorld(lightWorldPosition, worldPosition, lightDirection);
        const float falloff = ComputeFallOff(lightWorldPosition, worldPosition, theta, phi, alphaAngle);

        const float3 diffuse = max(0, dot(normalizedWorldNormal, normalizedLightDirection)) * lightColor * surfaceColor * kd;

        return diffuse * falloff;
    }

    static float3 ComputeHalfLambertDiffuse(
		const float theta, const float phi,
		float3 lightWorldPosition, float3 worldPosition, float3 lightDirection, float3 worldNormal, float3 surfaceColor, float3 lightColor, float3 kd
    )
    {
        const float3 normalizedLightDirection = normalize(lightDirection);
        const float3 normalizedWorldNormal = normalize(worldNormal);

        const float alphaAngle = ComputeAngleBetweenLightDirectionAndToVertexInWorld(lightWorldPosition, worldPosition, lightDirection);
        const float falloff = ComputeFallOff(lightWorldPosition, worldPosition, theta, phi, alphaAngle);

        const float fixedDot = dot(normalizedWorldNormal, normalizedLightDirection) * 0.5f + 0.5f;
        const float3 diffuse = fixedDot * lightColor * surfaceColor * kd;

        return diffuse * falloff;
    }

    static float3 ComputeToonDiffuse(
		Texture2D rampMap, SamplerState samplerState, 
		const float theta, const float phi,
		float3 lightWorldPosition, float3 worldPosition, float3 lightDirection, float3 worldNormal, float3 surfaceColor, float3 lightColor, float3 kd
    )
    {
        const float3 normalizedLightDirection = normalize(lightDirection);
        const float3 normalizedWorldNormal = normalize(worldNormal);

        const float alphaAngle = ComputeAngleBetweenLightDirectionAndToVertexInWorld(lightWorldPosition, worldPosition, lightDirection);
        const float falloff = ComputeFallOff(lightWorldPosition, worldPosition, theta, phi, alphaAngle);

        const float texcoordX = dot(normalizedWorldNormal, normalizedLightDirection) * 0.5f + 0.5f;
        const float3 diffuse = rampMap.Sample(samplerState, float2(texcoordX, 0.0f)) * lightColor * surfaceColor * kd;

        return diffuse * falloff;
    }

    //
    // Specular functions.
    //
    static float3 ComputePhongSpecular(
		//const float theta, const float phi,
		float3 lightWorldPosition, float3 worldNormal, float3 worldPosition,
		float3 viewPosition, float3 lightColor, float shininess
    )
    {
        const float3 worldDirection = worldPosition - lightWorldPosition;
        const float3 normalizedWorldNormal = normalize(worldNormal);
        const float3 normalizedWorldDirection = normalize(worldDirection) * -1.0f;
        const float3 normalizedToView = normalize(worldPosition - viewPosition);
        const float3 normalizedReflection = normalize(reflect(normalizedWorldDirection, normalizedWorldNormal));
        const float specularIntensity = pow(max(0, dot(normalizedReflection, normalizedToView)), shininess);

        return lightColor * specularIntensity;
        //const float alphaAngle = ComputeAngleBetweenLightDirectionAndToVertexInWorld(lightWorldPosition, worldPosition, lightDirection);
        //const float falloff = ComputeFallOff(lightWorldPosition, worldPosition, theta, phi, alphaAngle);
    }

    static float3 ComputeBlinnPhongSpecular(
		//const float theta, const float phi,
		float3 lightWorldPosition, float3 worldNormal, float3 worldPosition,
		float3 viewPosition, float3 lightColor, float shininess
    )
    {
        const float3 worldDirection = worldPosition - lightWorldPosition;
        const float3 normalizedWorldNormal = normalize(worldNormal);
        const float3 normalizedWorldDirection = normalize(worldDirection) * -1.0f;
        const float3 normalizedToView = normalize(worldPosition - viewPosition);
        const float3 normalizedHalfVector = normalize(normalizedToView + normalizedWorldDirection);
        const float specularIntensity = pow(max(0, dot(normalizedWorldNormal, normalizedHalfVector)), shininess);

        return lightColor * specularIntensity;
    }

    //
    // Utils functions.
    //
    static float ComputeAngleBetweenLightDirectionAndToVertexInWorld(
		float3 lightWorldPosition, float3 worldPosition, float3 lightDirection
    )
    {
        const float3 normalizedLightDirection = normalize(lightDirection);
        const float3 normalizedToVertexInWorld = normalize(worldPosition - lightWorldPosition);

        return dot(normalizedLightDirection, normalizedToVertexInWorld);
    }

    static float ComputeFallOff(float3 lightWorldPosition, float3 worldPosition, const float theta, const float phi, const float alphaAngle)
    {
        // Vertex should be in between phi and theta.
        if (alphaAngle >= phi && alphaAngle <= theta)
        {
            return ComputeAttenuationBetweenInnerAndOuterCorn(theta, phi, alphaAngle);
        }
        // Vertex should be in inner corn.
        else if (alphaAngle >= theta)
        {
            return ComputeAttenuationInnerCorn(lightWorldPosition, worldPosition);
        }

        // Doesn't reach light.
        return 0.0f;
    }

    static float ComputeAttenuationInnerCorn(float3 lightWorldPosition, float3 worldPosition)
    {
        const float3 lengthLightToVertexInWorld = abs(length(worldPosition - lightWorldPosition));
        const float attenuation = saturate(1.0f / (0.0f + 1.0f / lengthLightToVertexInWorld + 0.0f / (lengthLightToVertexInWorld * lengthLightToVertexInWorld)));

        return attenuation;
    }

    static float ComputeAttenuationBetweenInnerAndOuterCorn(const float theta, const float phi, const float alpha)
    {
        //return (alpha - (phi / 2.0f) / (theta / 2.0f) - (phi / 2.0f)) * falloff;
        return (alpha - (phi / 2.0f) / (theta / 2.0f) - (phi / 2.0f));
    }
};

struct HemisphereLight
{
	static float3 ComputeLight(float3 normal, float3 skyDirection, float4 skyColor, float4 groundColor)
	{
        const float3 N = normalize(normal);
        const float3 D = normalize(skyDirection);

        const float factor = dot(D, N) * 0.5f + 0.5f;
        const float3 result = lerp(groundColor.rgb, skyColor.rgb, factor);

        return result;
    }
};

//
// Lambert/Diffuse
//
float3 ComputeLambertDiffuse(
    const float3 worldPosition, const float3 worldNormal, 
    const float3 lightWorldPosition, const float3 lightColor,
    const float3 kd
)
{
    const float3 L = normalize(lightWorldPosition - worldPosition);
    const float3 N = normalize(worldNormal);
    const float3 diffuse = max(0, dot(N, L)) * lightColor * kd;

    return diffuse;
}

float3 ComputeLambertDiffuse(
    const float3 worldNormal,
    const float3 lightDirection, const float3 lightColor, 
    const float3 kd
)
{
    const float3 L = normalize(-lightDirection);
    const float3 N = normalize(worldNormal);
    const float3 diffuse = saturate(dot(N, L)) * lightColor * kd;

    return diffuse;
}

//
// Half lambert
//
float3 ComputeHalfLambertDiffuse(
    const float3 worldPosition, const float3 worldNormal,
    const float3 lightWorldPosition, const float3 lightColor,
    const float3 kd
)
{
    const float3 L = normalize(lightWorldPosition - worldPosition);
    const float3 N = normalize(worldNormal);

    const float fixedDot = saturate((dot(N, L) + 1.0f) * 0.5f);
    const float3 diffuse = fixedDot * lightColor * kd;

    return diffuse;
}

float3 ComputeHalfLambertDiffuse(
    const float3 worldNormal,
    const float3 lightDirection, const float3 lightColor,
    const float3 kd
)
{
    const float3 L = normalize(-lightDirection);
    const float3 N = normalize(worldNormal);

    float fixedDot = dot(N, L);
    fixedDot = (fixedDot * 0.5f) + 0.5f;
    const float3 diffuse = max(0, fixedDot) * lightColor * kd;

    return diffuse;
}

//
// Phong specular
//
float3 ComputePhongSpecular(
    const float3 worldPosition, const float3 worldNormal,
    const float3 worldCameraPosition,
    const float3 lightWorldPosition, const float3 lightColor,
    const float3 ks, const uint shininess = 128
)
{
    const float3 L = normalize(lightWorldPosition - worldPosition);
    const float3 N = normalize(worldNormal);
    const float3 V = normalize(worldCameraPosition - worldPosition);
    const float3 R = normalize(reflect(L, N));
    const float3 specular = pow(dot(R, V), shininess) * lightColor * ks;

    return specular;
}

float3 ComputePhongSpecular(
    const float3 worldNormal,
    const float3 viewDirection,
    const float3 lightDirection, const float3 lightColor,
    const float3 ks, const uint shininess = 128
)
{
    const float3 L = normalize(-lightDirection);
    const float3 N = normalize(worldNormal);
    const float3 V = normalize(-viewDirection);
    const float3 R = normalize(reflect(L, N));
    const float3 specular = pow(dot(R, V), shininess) * lightColor * ks;

    return specular;
}

//
// Blinn-phong specular
//
float3 ComputeBlinnPhongSpecular(
    const float3 worldPosition, const float3 worldNormal,
    const float3 worldCameraPosition,
    const float3 lightWorldPosition, const float3 lightColor,
    const float3 ks, const uint shininess = 128
)
{
    const float3 L = normalize(lightWorldPosition - worldPosition);
    const float3 N = normalize(worldNormal);
    const float3 V = normalize(worldCameraPosition - worldPosition);
    const float3 H = normalize(L + V);
    const float3 specular = pow(dot(N, H), shininess) * lightColor * ks;

    return specular;
}

float3 ComputeBlinnPhongSpecular(
    const float3 worldNormal,
    const float3 viewDirection,
    const float3 lightDirection, const float3 lightColor,
    const float3 ks, const uint shininess = 128
)
{
    const float3 L = normalize(-lightDirection);
    const float3 N = normalize(worldNormal);
    const float3 V = normalize(-viewDirection);
    const float3 H = normalize(L + V);
    const float3 specular = pow(dot(N, H), shininess) * lightColor * ks;

    return specular;
}

//
// Utils functions.
//


// distance -> Distance from shading point to light point.
float ComputeAttenuation(
    const float distance,
    const float constantAttenuation = 0.2f,
    const float linearAttenuation = 0.2f,
    const float quadraticAttenuation = 0.2f
)
{
    return saturate(1.0f / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * (distance * distance)));
}

float ComputeSpotCone(
    const float spotAngle, // Radian
    const float3 lightDirection,
    const float3 lightDirectionToShadedPoint
)
{
    const float3 L = normalize(lightDirection);
    const float3 LS = normalize(-lightDirectionToShadedPoint);

    const float minCos = cos(spotAngle);
    const float maxCos = (minCos + 1.0f) * 0.5f;
    const float cosTheta = dot(L, LS);

    return smoothstep(minCos, maxCos, cosTheta);
}


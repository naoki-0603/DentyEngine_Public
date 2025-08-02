//
// Structures.
//
struct TextureUsed
{
    int albedo;
    int metalness;
    int normal;
    int roughness;

    int emissive;
    int ambientOcclusion;
    int optionalSlot1;
    int optionalSlot2;
};

struct Material
{
    // 32bytes
    float4 baseColor;
    float4 emissiveColor;

    // 16bytes
    float baseRate;
    float roughness;
    float metallic;
    float specular;

    // 16bytes
    float emissiveIntensity;
    int renderingMode;
    int influenceHemisphereLight;
    float padding;

    // 32bytes
    TextureUsed texturesUsed;
};

// Albedo = 0,
// Metalness = 1,
// Normal = 2,
// Roughness = 3,
// Emissive = 4,
// AmbientOcclusion = 5,
// OptionalSlot1 = 6,
// OptionalSlot2 = 7,

//
// Constants.
//
#define MATERIAL_SAMPLER_STATE_POINT (0)
#define MATERIAL_SAMPLER_STATE_LINEAR (1)
#define MATERIAL_SAMPLER_STATE_ANISOTROPIC (2)

#define MATERIAL_ALBEDO_TEXTURE_INDEX (0)
#define MATERIAL_METALNESS_TEXTURE_INDEX (1)
#define MATERIAL_NORMAL_TEXTURE_INDEX (2)
#define MATERIAL_ROUGHNESS_TEXTURE_INDEX (3)
#define MATERIAL_EMISSIVE_TEXTURE_INDEX (4)
#define MATERIAL_AMBIENT_OCCLUSION_TEXTURE_INDEX (5)
#define MATERIAL_OPTIONAL1_TEXTURE_INDEX (6)
#define MATERIAL_OPTIONAL2_TEXTURE_INDEX (7)
#define SHADOW_MAP_NEAR (20)
#define SHADOW_MAP_MIDDLE (21)
#define SHADOW_MAP_FAR (22)

#define MAX_BONES (256)

#define VISIBLE_GRID_HEIGHT_MAX (10.0f)

#define DEFAULT_SPECULAR_SHININESS (128.0f)

static const int MAX_POINT_LIGHT = 10;
static const int MAX_SPOT_LIGHT = 10;

struct DirectionalLightConstant
{
    float4 position;
    float4 direction;
    float4 color;

    row_major float4x4 lightViewProjections[3];
};

struct HemisphereLightConstants
{
    float3 skyDirection;
    float padding;
    float4 skyColor;
    float4 groundColor;
};

struct PointLightConstant
{
    float4 position;
    float4 color;

    float intensity;
    float range;
    float2 padding;

    row_major float4x4 lightViewProjection;
};

struct SpotLightConstant
{
    float4 position;
    float4 direction;

    float4 color;

    float phi;
    float theta;
    float intensity;
    float padding;

    row_major float4x4 lightViewProjection;
};

cbuffer SHADOW_CONSTANT_BUFFER : register(b7)
{
    float4 shadowCameraPosition;
    row_major float4x4 shadowCameraViewProjection;

	float4 playerShadowCameraPosition;
    row_major float4x4 playerShadowCameraViewProjection;
};

cbuffer COLLISION_VISUALIZER_CAMERA_CONSTANT_BUFFER : register(b8)
{
    row_major float4x4 collisionVisualizerCameraViewProjection;
    row_major float4x4 collisionVisualizerCameraView;
    row_major float4x4 collisionVisualizerCameraProjection;

    float4 collisionVisualizerCameraPosition;
};

cbuffer PHOTO_CAMERA_CONSTANT_BUFFER : register(b9)
{
    row_major float4x4 photoCameraViewProjection;
    row_major float4x4 photoCameraView;
    row_major float4x4 photoCameraProjection;

    float4 photoCameraPosition;
};

cbuffer CAMERA_CONSTANT_BUFFER : register(b10)
{
    row_major float4x4 viewProjection;
    row_major float4x4 view;
    row_major float4x4 projection;
    row_major float4x4 inverseView;
    row_major float4x4 inverseProjection;

    float4 cameraPosition;
};

cbuffer LIGHT_CONSTANT_BUFFER : register(b11)
{
    DirectionalLightConstant directionalLight;
    HemisphereLightConstants hemisphereLight;
    PointLightConstant pointLights[MAX_POINT_LIGHT];
    SpotLightConstant spotLights[MAX_SPOT_LIGHT];

    int pointLightCount;
    int spotLightCount;

    // Avoid redefine padding2.
    float2 padding_2;
};

cbuffer TIME_CONSTANT_BUFFER : register(b12)
{
    float time;
    float deltaTime;
};
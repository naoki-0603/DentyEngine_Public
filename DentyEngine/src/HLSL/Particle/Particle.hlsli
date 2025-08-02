struct VSOut
{
    uint vertexId : VertexId;
};

struct GSOut
{
    float4 position : SV_Position;
    float4 color : Color;
    float2 texcoord : Texcoord;
};

struct ParticleData
{
    float3 position;
    float3 velocity;
    float3 eulerAngles;

    float4 color;

    float scale;
    float lifeTime;

    bool isActive;
};

cbuffer PARTICLE_CONSTANT_BUFFER : register(b0)
{
    float4 startColor;
    float4 finishColor;

    float startSize;
    float finishSize;

    float startSpeed;
    float finishSpeed;

    float startRotation;
    float finishRotation;

    float duration;

    uint emitParticleCount;
    uint currentParticleCount;
    uint particleCount;
    uint maxParticle;

    float padding;

	row_major float4x4 world;

    float4 initialPosition;
};
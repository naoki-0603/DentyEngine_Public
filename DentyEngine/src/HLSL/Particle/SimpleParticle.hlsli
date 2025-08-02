#include "Particle.hlsli"

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

struct SimpleParticleData : ParticleData
{
};

cbuffer SIMPLE_PARTICLE_CONSTANTS : register(b0)
{
    float3 currentEyePosition;
    float padding;
    float3 previousEyePosition;
    float padding2;

    float4 color;

    float radius;
    float height;

    float particleSize;

    uint particleCount;

    row_major float4x4 world;
};
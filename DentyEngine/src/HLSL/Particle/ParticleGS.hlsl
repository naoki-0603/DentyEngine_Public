#include "Particle.hlsli"
#include "Constants.hlsli"

StructuredBuffer<ParticleData> particleBuffer : register(t10);

[maxvertexcount(4)]
void main(
	// point -> input[1], line -> input[2], triangle -> input[3]
	point VSOut input[1] : SV_POSITION,
	inout TriangleStream<GSOut> output
)
{
    const float3 billboard[] =
    {
        float3(-1.0f, -1.0f, 0.0f),
        float3(+1.0f, -1.0f, 0.0f),
        float3(-1.0f, +1.0f, 0.0f),
        float3(+1.0f, +1.0f, 0.0f)
    };

    const float2 texcoords[] =
    {
        float2(0.0f, 1.0f),
        float2(1.0f, 1.0f),
        float2(0.0f, 0.0f),
        float2(1.0f, 0.0f)
    };

    // TODO 要改良,
    // Drawコールをパーティクルの最大数呼んでいるため、負荷が高い
    ParticleData particleData = particleBuffer[input[0].vertexId];

    if (particleData.isActive)
    {
        float4 viewSpacePosition = mul(float4(particleData.position, 1.0f), view);
        float3 viewSpaceVelocity = mul(float4(particleData.velocity, 0.0f), view).xyz;

        [unroll]
        for (uint vertexIndex = 0; vertexIndex < 4; ++vertexIndex)
        {
            GSOut element;
    
            const float3 cornerPosition = mul(billboard[vertexIndex] * particleData.scale, view).xyz;
    
            element.position = mul(float4(viewSpacePosition.xyz + cornerPosition, 1.0f), projection);
            element.color = particleData.color;
            element.texcoord = texcoords[vertexIndex];
    
            output.Append(element);
        }

        // Generate a vertex. If the topology requirement is not met, it is discarded.
        output.RestartStrip();
    }
}
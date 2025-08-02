#include "Particle.hlsli"
#include "Constants.hlsli"
#include "Noises.hlsli"

RWStructuredBuffer<ParticleData> particles : register(u0);
AppendStructuredBuffer<uint> deadList : register(u1);
RWStructuredBuffer<uint> particleCounter : register(u2);

[numthreads(PARTICLE_THREAD_NUM, 1, 1)]
void main( uint3 dTId : SV_DispatchThreadID )
{
    const uint threadIdX = dTId.x;

        ParticleData particle = particles[threadIdX];

        if (particle.isActive)
        {
            particle.position += particle.velocity * deltaTime;
            particle.lifeTime -= deltaTime;
    
            if (particle.lifeTime <= 0.0f)
            {
                particle = (ParticleData) 0;

               particle.color = float4(1, 1, 1, 0);
                particle.isActive = false;
    
                deadList.Append(threadIdX);
    
                particleCounter.DecrementCounter();
            }
    
            particles[threadIdX] = particle;
        }
}
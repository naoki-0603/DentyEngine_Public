//#include "Particle.hlsli"

//#include "Noises.hlsli"

//#include "Constants.hlsli"

//RWStructuredBuffer<ParticleData> particles : register(u0);
//ConsumeStructuredBuffer<uint> particlePool : register(u1);
//RWStructuredBuffer<uint> particleCounter : register(u2);

//// TODO ƒ}ƒNƒ‚É‘Î‰‚³‚¹‚é
//[numthreads(1024, 1, 1)]
//void main(uint3 dTId : SV_DISPATCHTHREADID)
//{
//    const uint dispatchThreadXId = dTId.x;

//    if (dispatchThreadXId < particleCount)
//    {
//        const uint id = particlePool.Consume();
   
//        ParticleData particle = particles[id];
//        particle.isActive = true;
//        particle.position = initialPosition;
//        particle.lifeTime = duration;
//        particle.velocity.x = Random(id) * 5.0f;
//        particle.velocity.y = Random(id + 256) * 5.0f;
//        particle.velocity.z = Random(id + 512) * 5.0f;
//        particle.scale = 0.1f;
//        particle.color = float4(1, 1, 1, 1);
   
//        particles[id] = particle;
   
//        particleCounter.IncrementCounter();
//    }
//}
#include "Particle.hlsli"

#include "Noises.hlsli"

RWStructuredBuffer<ParticleData> particles : register(u0);
AppendStructuredBuffer<uint> deadList : register(u1);

[numthreads(PARTICLE_THREAD_NUM, 1, 1)]
void main( uint3 dTId : SV_DispatchThreadID )
{
    const uint id = dTId.x;

    // 指定したパーティクル以上のスレッド数が デッドリストに追加されてしまうと、不具合を起こす
    // なので現在の初期化するスレッドが パーティクルの最大数よりも小さい場合のみ処理するように変更
    if (id < maxParticle)
    {
        ParticleData particle = particles[id];

        particle.isActive = false;
        deadList.Append(id);

        particles[id] = particle;
    }
}
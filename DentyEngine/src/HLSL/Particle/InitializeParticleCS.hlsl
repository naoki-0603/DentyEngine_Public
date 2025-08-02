#include "Particle.hlsli"

#include "Noises.hlsli"

RWStructuredBuffer<ParticleData> particles : register(u0);
AppendStructuredBuffer<uint> deadList : register(u1);

[numthreads(PARTICLE_THREAD_NUM, 1, 1)]
void main( uint3 dTId : SV_DispatchThreadID )
{
    const uint id = dTId.x;

    // �w�肵���p�[�e�B�N���ȏ�̃X���b�h���� �f�b�h���X�g�ɒǉ�����Ă��܂��ƁA�s����N����
    // �Ȃ̂Ō��݂̏���������X���b�h�� �p�[�e�B�N���̍ő吔�����������ꍇ�̂ݏ�������悤�ɕύX
    if (id < maxParticle)
    {
        ParticleData particle = particles[id];

        particle.isActive = false;
        deadList.Append(id);

        particles[id] = particle;
    }
}
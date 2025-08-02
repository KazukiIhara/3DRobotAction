#include "ParticleEffect.hlsli"

// �p�[�e�B�N��
RWStructuredBuffer<Particle> gParticle : register(u0);

// ������
[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint particleIndex = DTid.x;
    if (particleIndex < kMaxParticles)
    {
        gParticle[particleIndex] = (Particle) 0;
    }
    
}
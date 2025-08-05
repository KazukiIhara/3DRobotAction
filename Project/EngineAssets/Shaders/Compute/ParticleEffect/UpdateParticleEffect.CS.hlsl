#include "ParticleEffect.hlsli"

// �p�[�e�B�N��
RWStructuredBuffer<Particle> gParticle : register(u0);
ConstantBuffer<ParticleInfo> gInfo : register(b0);

// ������
[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint particleIndex = DTid.x;
    if (particleIndex < kMaxParticles)
    {
        Particle p = gParticle[particleIndex];
        if (p.age <= p.life)
        {
            gParticle[particleIndex].pos += p.velo * gInfo.deltaTime;
            gParticle[particleIndex].age += gInfo.deltaTime;
        }
    }
}
#include "ParticleEffect.hlsli"

// �p�[�e�B�N��
RWStructuredBuffer<Particle> gParticle : register(u0);
StructuredBuffer<ParticleEmitData> gEmit : register(t0);
ConstantBuffer<ParticleCount> gCounter : register(b0);

// ������
[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint particleIndex = DTid.x;
    if (particleIndex < kMaxParticles)
    {
        gParticle[particleIndex] = (Particle) 0;
    }
    
}
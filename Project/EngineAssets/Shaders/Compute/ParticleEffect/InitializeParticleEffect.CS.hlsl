#include "ParticleEffect.hlsli"

// パーティクル
RWStructuredBuffer<ParticleData> gParticleData : register(u0);

// 初期化
[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint particleIndex = DTid.x;
    if (particleIndex < kMaxParticles)
    {
        gParticleData[particleIndex] = (ParticleData) 0;
    }
    
}
#include "ParticleEffect.hlsli"

// パーティクル
RWStructuredBuffer<Particle> gParticle : register(u0);
StructuredBuffer<ParticleEmitData> gEmit : register(t0);
ConstantBuffer<ParticleInfo> gInfo : register(b0);

// 初期化
[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (gInfo.emitCount != 0)
    {
        for (uint i = 0; i < gInfo.emitCount; ++i)
        {
            gParticle[i].pos = gEmit[i].pos;
            gParticle[i].life = gEmit[i].life;
            gParticle[i].velo = gEmit[i].velo;
            gParticle[i].texIdx = gEmit[i].texIndex;
            gParticle[i].color = gEmit[i].color;
            gParticle[i].size = gEmit[i].size;
            gParticle[i].age = 0.0f;
        }
    }
  
}
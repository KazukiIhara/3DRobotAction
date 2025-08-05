#include "ParticleEffect.hlsli"

// パーティクル
RWStructuredBuffer<Particle> gParticle : register(u0);
StructuredBuffer<ParticleEmitData> gEmit : register(t0);
ConstantBuffer<ParticleInfo> gInfo : register(b0);
RWStructuredBuffer<int> gFreeListIndex : register(u1);
RWStructuredBuffer<uint> gFreeList : register(u2);

// 初期化
[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (gInfo.emitCount != 0)
    {
        for (uint i = 0; i < gInfo.emitCount; ++i)
        {                      
            int freeListIndex;
            InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
            if (0 <= freeListIndex && freeListIndex < kMaxParticles)
            {
                uint particleIdx = gFreeList[freeListIndex];
                gParticle[particleIdx].pos = gEmit[i].pos;
                gParticle[particleIdx].life = gEmit[i].life;
                gParticle[particleIdx].velo = gEmit[i].velo;
                gParticle[particleIdx].texIdx = gEmit[i].texIndex;
                gParticle[particleIdx].color = gEmit[i].color;
                gParticle[particleIdx].size = gEmit[i].size;
                gParticle[particleIdx].age = 0.0f;                 
            }
            else
            {
                InterlockedAdd(gFreeListIndex[0], 1);
                break;
            }
       
        }
    }
  
}
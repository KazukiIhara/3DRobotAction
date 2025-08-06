#include "ParticleEffect.hlsli"

// パーティクル
RWStructuredBuffer<Particle> gParticle : register(u0);
ConstantBuffer<ParticleInfo> gInfo : register(b0);
RWStructuredBuffer<int> gFreeListIndex : register(u1);
RWStructuredBuffer<uint> gFreeList : register(u2);

// 初期化
[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint particleIndex = DTid.x;
    if (particleIndex < kMaxParticles)
    {
        Particle p = gParticle[particleIndex];
        if (0.0f < p.timer)
        {
            float newTimer = p.timer - gInfo.deltaTime;
            float remainRatio = saturate(newTimer / p.life);
            
            gParticle[particleIndex].pos += p.velo * gInfo.deltaTime;
            gParticle[particleIndex].color.a = remainRatio;
            gParticle[particleIndex].timer = newTimer;
            
            if (newTimer <= 0.0f)
            {
                int freeListIdx;
                InterlockedAdd(gFreeListIndex[0], 1, freeListIdx);
                if ((freeListIdx + 1) < kMaxParticles)
                {
                    gFreeList[freeListIdx + 1] = particleIndex;
                }
            
            }
        }
        
        
    }
}
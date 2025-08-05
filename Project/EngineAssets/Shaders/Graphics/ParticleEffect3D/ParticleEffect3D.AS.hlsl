
#include "../../Compute/ParticleEffect/ParticleEffect.hlsli"

StructuredBuffer<Particle> gInstanceData : register(t0);

[shader("amplification")]
[numthreads(1, 1, 1)]
void main(uint3 tid : SV_DispatchThreadID)
{
    uint instanceID = tid.y;
    Particle p = gInstanceData[instanceID];
   
    ASPayload payload;
    payload.instanceID = instanceID;
    
    uint alive = (p.age <= p.life);

    DispatchMesh(alive, 1, 1, payload);
    
}
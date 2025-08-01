
#include "../../Compute/ParticleEffect/ParticleEffect.hlsli"

[numthreads(1, 1, 1)]
void main(uint3 tid : SV_DispatchThreadID)
{
    ASPayload payload;
    payload.instanceID =  tid.y;

    DispatchMesh(1, 1, 1, payload);
}
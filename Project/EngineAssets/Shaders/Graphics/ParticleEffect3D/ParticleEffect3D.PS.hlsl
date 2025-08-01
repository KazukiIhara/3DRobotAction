#include "../../Compute/ParticleEffect/ParticleEffect.hlsli"

StructuredBuffer<Particle> gInstanceData : register(t0);
Texture2D gTextures[] : register(t1000);
SamplerState gSampler : register(s0);

float4 main(MeshOutput input) : SV_Target0
{
    float4 output;
    
    uint instanceID = input.instanceIndex;
    Particle particle = gInstanceData[instanceID];
    
    float2 uv = input.uv;
    float4 texColor = gTextures[particle.texIdx].Sample(gSampler, uv);
    
    output = texColor * particle.color;
    
    return output;
}

static const uint kMaxParticles = 1024;

struct ParticleEmitData
{
    float3 pos;
    float life;
    
    float3 velo;
    uint texIndex;
    
    float4 color;
    
    float2 size;
    float2 pad0;
    
};

struct Particle
{
    float3 pos;
    float life;
    
    float3 velo;
    uint texIdx;
    
    float4 color;
    
    float2 size;
    float age;
    float pad0;
    
};

struct ParticleData
{
    float3 scale;
    float pad0;
    
    float3 rotate;
    float pad1;
    
    float3 translate;
    float pad2;
    
    float3 velocity;
    float pad3;
    
    uint texIndex;
    float3 pad4;
    
    uint isBillboard;
    float3 pad5;
    
    float lifeTime;
    float3 pad6;
    
    float currentTime;
    float3 pad7;
};

struct ParticleDrawData
{
    float4x4 worldMat;
    
    float4 color;
    
    uint texIndex;
    float3 pad0;
};
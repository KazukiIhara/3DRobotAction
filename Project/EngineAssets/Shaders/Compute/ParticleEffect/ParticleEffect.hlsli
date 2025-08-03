
static const uint kMaxParticles = 1024;

static const float4 kPlane[4] =
{
    float4(-1.0f, 1.0f, 0.0f, 1.0f),
    float4(1.0f, 1.0f, 0.0f, 1.0f),
    float4(-1.0f, -1.0f, 0.0f, 1.0f),
    float4(1.0f, -1.0f, 0.0f, 1.0f),
};

static const float2 kPlaneUVs[4] =
{
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f),
    float2(0.0f, 1.0f),
    float2(1.0f, 1.0f)
};

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

struct EmitCount
{
    uint emitCount;
    float3 pad0;
};

// Mesh�V�F�[�_�[����s�N�Z���V�F�[�_�[�ɓn���\����
struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    uint instanceIndex : TEXCOORD1;
};

// �J����
struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
    float _padding;
};

struct CameraVector
{
    float3 right;
    float pad0;
    
    float3 up;
    float pad1;
    
    float3 forward;
    float pad2;
};

// AS�Ŏg���\����
struct ASPayload
{
    uint instanceID;
};
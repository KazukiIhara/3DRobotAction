
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

// Meshシェーダーからピクセルシェーダーに渡す構造体
struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    uint instanceIndex : TEXCOORD1;
};

// カメラ
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

// ASで使う構造体
struct ASPayload
{
    uint instanceID;
};
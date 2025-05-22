struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
};

struct Material
{
    float4 color;
    int enbleLighting;
    int enableSpeculaerRef;
    float4x4 uvTransform;
    float shininess;
};

struct ModelMaterial
{
    float4 color;
    float4x4 uvTransform;
};

struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct ParticleForGPU
{
    float4x4 World;
    float4 color;
};
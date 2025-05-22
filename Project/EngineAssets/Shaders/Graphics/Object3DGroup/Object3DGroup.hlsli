struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 worldPosition : POSITION0;
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct TransformationMatrix
{
    float4x4 World;
    float4x4 WorldInverseTranspose;
};

struct Material
{
    float4 color;
    int enableLighting;
    int enableSpeculaerRef;
    float4x4 uvTransform;
    float shininess;
};

struct ModelMaterial
{
    float4 color;
    float4x4 uvTransform;
    int enableNormalMap;
};

struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
};

struct PunctualLight
{
    // 全ライト共通
    uint type;
    float3 color;
    float intensity;
    
    // SpotPoint共通
    float3 position;
    float radius;
    float decay;

    // SpotLight　DirectionalLight用
    float3 direction;
  
    // SpotLight用
    float cosAngle;
    float cosFalloffStart;
};

struct LightCount
{
    // ライトの総数
    uint num;
};

struct Object3DForGPU
{
    float4x4 World;
    float4x4 WorldInverseTranspose;
};
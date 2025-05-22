
// Meshシェーダーからピクセルシェーダーに渡す構造体
struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    uint instanceIndex : TEXCOORD1;
};

struct SpriteData
{
    float4x4 worldMatrix;
    float2 vertexPos[4];
    float2 texcoord[4];
    
    uint textureIndex;
    float4 baseColor;
    float4x4 uvMatrix;
    float3 padding;
};

struct Camera
{
    float4x4 viewProjection;
};
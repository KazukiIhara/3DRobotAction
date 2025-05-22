
struct VertexShaderInput
{
    float4 position : POSITION0;
};

struct VertexShaderOutput
{
    float4 position : SV_Position;
    float3 texcoord : TEXCOORD0;
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct SkyBoxData
{
    float4x4 worldMatrix;
};

struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
};
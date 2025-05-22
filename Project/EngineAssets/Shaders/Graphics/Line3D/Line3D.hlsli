struct VertexShaderInput
{
    uint instanceId : SV_InstanceID; // インスタンシング用
    uint vertexId : SV_VertexID; // 頂点ID
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION; // スクリーン座標
    float4 color : COLOR0; // ラインの色
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0; // 出力色
};

// 3Dラインデータ
struct LineData3D
{
    float3 start;
    float3 end;
    float4 color;
};

struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
};
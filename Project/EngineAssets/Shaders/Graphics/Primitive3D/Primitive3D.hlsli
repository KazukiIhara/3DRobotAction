
//-----------------------------------
// 定数
//-----------------------------------

static const float2 QuadUVs[4] =
{
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f),
    float2(0.0f, 1.0f),
    float2(1.0f, 1.0f)
};

// Meshシェーダーからピクセルシェーダーに渡す構造体
struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    uint instanceIndex : TEXCOORD1;
    float3 normal : NORMAL0;
};

// αありのMeshシェーダーからピクセルシェーダーに渡す構造体
struct MeshOutputWithAlpha
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    uint instanceIndex : TEXCOORD1;
};

// シャドウ用軽量Vertex出力
struct ShadowMeshOutput
{
    float4 position : SV_Position;
};

// マテリアルデータ
struct PrimitiveMaterialData3D
{
    uint textureIndex;
    float3 _padding0;
    float4 baseColor;
    float4x4 uvMatrix;
};

// カメラ
struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
    float _padding;
};

// ルート定数
struct RootConstants
{
    uint baseInstanceIndex;
    uint _pad0, _pad1, _pad2;
};

// ASで使う構造体
struct ASPayload
{
    uint instanceID;
};
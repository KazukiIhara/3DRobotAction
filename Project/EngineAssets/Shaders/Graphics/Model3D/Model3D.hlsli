
#define AS_GROUP_SIZE 32

// 頂点出力
struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
};

// αありの頂点出力
struct MeshOutputWithAlpha
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

// シャドウ用軽量Vertex出力
struct ShadowMeshOutput
{
    float4 position : SV_Position;
};

// 頂点データ
struct VertexData3D
{
    float4 position;
    float2 uv;
    float3 normal;
    float3 tangent;
};

// インスタンスデータ
struct ModelDataForGPU
{
    float4x4 world;
    float4x4 worldInverseTranspose;
    float4 color;
    uint isMakeShadow;
    float3 _pad;
};

// αありのインスタンスデータ
struct ModelDataWithAlphaForGPU
{
    float4x4 world;
};

// マテリアルデータ
struct MaterialData3D
{
    uint texIdx;
    float3 _pad;
    float4 baseColor;
    float4x4 uvMatrix;
};

// カメラデータ
struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
    float _pad;
};

// ライトのカメラ
struct DirectionalLightCamera
{
    float4x4 viewProjection;
};

// ────────── Meshlet関連構造体 ──────────

struct MeshInfo
{
    uint IndexSize;
    uint MeshletCount;
};

struct Meshlet
{
    uint VertCount;
    uint VertOffset;
    uint PrimCount;
    uint PrimOffset;
};

struct Payload          // AS → MS で渡す最小ペイロード
{
    uint meshletIndices[32];
    uint instanceID;
    uint _pad[2];
};

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

struct CameraFrustum
{
    float4 planes[6];
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

struct CullData
{
    float4 BoundingSphere;
    uint NormalCone;
    float ApexOffset;
};

bool IsConeDegenerate(CullData c)
{
    return (c.NormalCone >> 24) == 0xff;
}

float4 UnpackCone(uint packed)
{
    float4 v;
    v.x = float((packed >> 0) & 0xFF);
    v.y = float((packed >> 8) & 0xFF);
    v.z = float((packed >> 16) & 0xFF);
    v.w = float((packed >> 24) & 0xFF);

    v = v / 255.0;
    v.xyz = v.xyz * 2.0 - 1.0;

    return v;
}

bool IsVisible(CullData c, float4x4 world, float3 viewPos,float4 planes[6])
{
    // Do a cull test of the bounding sphere against the view frustum planes.
    float4 center = mul(float4(c.BoundingSphere.xyz, 1), world);
    float radius = c.BoundingSphere.w;

    for (int i = 0; i < 6; ++i)
    {
        if (dot(center, planes[i]) < -radius)
        {
            return false;
        }
    }

    // Do normal cone culling
    if (IsConeDegenerate(c))
        return true; // Cone is degenerate - spread is wider than a hemisphere.

    // Unpack the normal cone from its 8-bit uint compression
    float4 normalCone = UnpackCone(c.NormalCone);

    // Transform axis to world space
    float3 axis = normalize(mul(float4(normalCone.xyz, 0), world)).xyz;

    // Offset the normal cone axis from the meshlet center-point - make sure to account for world scaling
    float3 apex = center.xyz - axis * c.ApexOffset;
    float3 view = normalize(viewPos - apex);

    // The normal cone w-component stores -cos(angle + 90 deg)
    // This is the min dot product along the inverted axis from which all the meshlet's triangles are backface
    if (dot(view, -axis) > normalCone.w)
    {
        return false;
    }

    // All tests passed - it will merit pixels
    return true;
}
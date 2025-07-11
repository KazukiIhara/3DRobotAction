
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
    uint hasDiffuse;
    float3 _pad1;
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

bool IsVisible(CullData c, float4x4 world, float3 viewPos, float4 planes[6])
{
    // バウンディングスフィアを視錐台の 6 平面とテストしてカリングする
    float4 center = mul(float4(c.BoundingSphere.xyz, 1), world);
    float radius = c.BoundingSphere.w;

    for (int i = 0; i < 6; ++i)
    {
        if (dot(center, planes[i]) < -radius)
        {
            return false; // 半径分オフセットしても平面の外側にある場合は不可視
        }
    }

    // 法線コーンが退化（開きすぎ）している場合の早期退出
    if (IsConeDegenerate(c))
        return true; // コーンが半球以上に広がっているため、常に可視とみなす

    // 8 bit に圧縮された法線コーンを展開
    float4 normalCone = UnpackCone(c.NormalCone);

    // コーン軸をワールド空間に変換
    float3 axis = normalize(mul(float4(normalCone.xyz, 0), world)).xyz;

    // メシュレット中心から軸方向にオフセット（ワールドスケールを考慮）
    float3 apex = center.xyz - axis * c.ApexOffset;
    float3 view = normalize(viewPos - apex);

    // normalCone.w には「角度＋90°」の負の余弦が格納されている  
    // これは、反転軸方向に対する最小ドット積（＝すべての三角形が背面向きになる境界）を示す
    if (dot(view, -axis) > normalCone.w)
    {
        return false; // 視線方向がこの閾値を超える場合は背面のみなので描画不要
    }

    // すべてのテストを通過したのでピクセルが生成される
    return true;
}

bool IsVisibleBackface(CullData c, float4x4 world, float3 viewPos)
{
    // 8bit 圧縮から展開
    float4 normalCone = UnpackCone(c.NormalCone);

    // コーン軸（法線平均方向）をワールド空間へ
    float3 axis = normalize(mul(float4(normalCone.xyz, 0), world)).xyz;

    // メシュレット中心をワールド空間で取得
    float3 center = mul(float4(c.BoundingSphere.xyz, 1), world).xyz;

    // 頂点 (apex) = center - axis * ApexOffset  ※スケール込み
    float3 apex = center - axis * c.ApexOffset;

    // apex からカメラへ向かう正規化ベクトル
    float3 view = normalize(viewPos - apex);

    // 背面判定：コーン軸の反対（-axis）とのドット積が閾値 normalCone.w を超えたら背面
    if (dot(view, -axis) > normalCone.w)
    {
        return false; // 視線方向がこの閾値を超える場合は背面のみなので描画不要
    }
    
    // テストを通過したのでピクセルが生成される
    return true;
}
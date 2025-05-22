#include "Cylinder3D.hlsli"
#include "../Primitive3D/Primitive3D.hlsli"

#define MAX_VERTICES   4
#define MAX_TRIANGLES  2
#define PI             3.14159265f

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<CylinderData3D> gInstanceData : register(t0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(
    in payload ASPayload payload,
    uint3 threadID : SV_DispatchThreadID,
    out indices uint3 tris[MAX_TRIANGLES],
    out vertices MeshOutputWithAlpha verts[MAX_VERTICES]
)
{
    uint instanceID = payload.instanceID;
    uint tileID = threadID.x;

    CylinderData3D cylinder = gInstanceData[instanceID];
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];
    
    uint divide = cylinder.cylinderDivide;
    
    bool isActive = (tileID < divide);
    SetMeshOutputCounts(isActive ? MAX_VERTICES : 0, isActive ? MAX_TRIANGLES : 0);
    if (!isActive)
        return;
    
    float radianParDivide = (2.0f * PI) / divide;
    float angle0 = tileID * radianParDivide;
    float angle1 = (tileID + 1) * radianParDivide;

    // ローカル頂点位置（サイド面）
    float3 pLocal[4] =
    {
        float3(cos(angle0) * cylinder.topRadius, cylinder.height, sin(angle0) * cylinder.topRadius),
        float3(cos(angle1) * cylinder.topRadius, cylinder.height, sin(angle1) * cylinder.topRadius),
        float3(cos(angle0) * cylinder.bottomRadius, 0.0f, sin(angle0) * cylinder.bottomRadius),
        float3(cos(angle1) * cylinder.bottomRadius, 0.0f, sin(angle1) * cylinder.bottomRadius)
    };

    // ワールド位置を先に計算して格納
    float4 worldPos4[4];
    [unroll]
    for (uint i = 0; i < 4; ++i)
    {
        worldPos4[i] = mul(float4(pLocal[i], 1.0f), cylinder.worldMatrix);
    }

    // UV
    float u0 = (float) tileID / (float) divide;
    float u1 = (float) (tileID + 1) / (float) divide;
    float2 baseUVs[4] =
    {
        float2(u0, 0.0f),
        float2(u1, 0.0f),
        float2(u0, 1.0f),
        float2(u1, 1.0f)
    };

    [unroll]
    for (uint i = 0; i < 4; ++i)
    {
        float4 clipPos = mul(worldPos4[i], gCamera.viewProjection);
        float4 worldPosition = worldPos4[i];
        float2 uv = mul(float4(baseUVs[i], 0.0f, 1.0f), mat.uvMatrix).xy;

        // 出力設定
        verts[i].position = clipPos;
        verts[i].uv = uv;
        verts[i].instanceIndex = instanceID;
    }

    // インデックス
    tris[0] = uint3(0, 2, 1);
    tris[1] = uint3(2, 3, 1);
}

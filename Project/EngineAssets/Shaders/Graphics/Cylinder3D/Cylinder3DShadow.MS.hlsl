#include "Cylinder3D.hlsli"
#include "../Primitive3D/Primitive3D.hlsli"

#define MAX_VERTICES   6   
#define MAX_TRIANGLES  4  
#define PI             3.14159265f

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<CylinderData3D> gInstanceData : register(t0);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(
    in payload ASPayload payload,
    uint3 threadID : SV_DispatchThreadID,
    out indices uint3 tris[MAX_TRIANGLES],
    out vertices ShadowMeshOutput verts[MAX_VERTICES]
)
{
    uint instanceID = payload.instanceID;
    uint tileID = threadID.x;

    CylinderData3D cylinder = gInstanceData[instanceID];
    
    uint divide = cylinder.cylinderDivide;
    bool hasCap = cylinder.hasCap;
    
    bool isActive = (tileID < divide);
   
    uint vCount = hasCap ? 6 : 4; // 頂点数
    uint tCount = hasCap ? 4 : 2; // 三角形数
    SetMeshOutputCounts(isActive ? vCount : 0, isActive ? tCount : 0);
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

    float3 topCenterOS = float3(0.0f, cylinder.height, 0.0f);
    float3 bottomCenterOS = float3(0.0f, 0.0f, 0.0f);
    
    // ワールド位置を先に計算して格納
    float4 worldPos[6];
    [unroll]
    for (uint i = 0; i < 4; ++i)
    {
        worldPos[i] = mul(float4(pLocal[i], 1.0f), cylinder.worldMatrix);
    }
    
    if (hasCap)
    {
        worldPos[4] = mul(float4(topCenterOS, 1.0f), cylinder.worldMatrix); // 上面中心
        worldPos[5] = mul(float4(bottomCenterOS, 1.0f), cylinder.worldMatrix); // 底面中心
    }

    [unroll]
    for (uint i = 0; i < 4; ++i)
    {
        verts[i].position = mul(worldPos[i], gCamera.viewProjection);
    }
    
    if (hasCap)
    {
        // 上面中心 (index 4)
        verts[4].position = mul(worldPos[4], gCamera.viewProjection);

        // 底面中心 (index 5)
        verts[5].position = mul(worldPos[5], gCamera.viewProjection);
    }

    // インデックス
    tris[0] = uint3(0, 2, 1);
    tris[1] = uint3(2, 3, 1);
    
    // キャップ
    if (hasCap)
    {
        tris[2] = uint3(4, 1, 0); // 上面（CWで +Y 法線）
        tris[3] = uint3(5, 2, 3); // 底面（CWで -Y 法線）
    }
}

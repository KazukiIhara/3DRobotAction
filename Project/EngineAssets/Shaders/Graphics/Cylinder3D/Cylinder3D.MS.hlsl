#include "Cylinder3D.hlsli"
#include "../Primitive3D/Primitive3D.hlsli"

#define MAX_VERTICES   6   
#define MAX_TRIANGLES  4  
#define PI             3.14159265f

//--------------- 定数・バッファ ---------------
ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<CylinderData3D> gInstanceData : register(t0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

//--------------- Mesh Shader ---------------
[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(
    in payload ASPayload payload,
    uint3 threadID : SV_DispatchThreadID,
    out indices uint3 tris[MAX_TRIANGLES],
    out vertices MeshOutput verts[MAX_VERTICES]
)
{
    uint instanceID = payload.instanceID;
    uint tileID = threadID.x;

    CylinderData3D cylinder = gInstanceData[instanceID];
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    uint divide = cylinder.cylinderDivide;
    bool hasCap = cylinder.hasCap;

    bool isActive = (tileID < divide);

    uint vCount = hasCap ? 6 : 4; // 頂点数
    uint tCount = hasCap ? 4 : 2; // 三角形数
    SetMeshOutputCounts(isActive ? vCount : 0, isActive ? tCount : 0);
    if (!isActive)
        return;

    float radPerDiv = (2.0f * PI) / divide;
    float angle0 = tileID * radPerDiv;
    float angle1 = (tileID + 1) * radPerDiv;

    float3 pLocal[4] =
    {
        float3(cos(angle0) * cylinder.topRadius, cylinder.height, sin(angle0) * cylinder.topRadius),
        float3(cos(angle1) * cylinder.topRadius, cylinder.height, sin(angle1) * cylinder.topRadius),
        float3(cos(angle0) * cylinder.bottomRadius, 0.0f, sin(angle0) * cylinder.bottomRadius),
        float3(cos(angle1) * cylinder.bottomRadius, 0.0f, sin(angle1) * cylinder.bottomRadius)
    };

    float3 topCenterOS = float3(0.0f, cylinder.height, 0.0f);
    float3 bottomCenterOS = float3(0.0f, 0.0f, 0.0f);

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

    float u0 = (float) tileID / divide;
    float u1 = (float) (tileID + 1) / divide;

    float2 baseUV[4] =
    {
        float2(u0, 0.0f), // top-left
        float2(u1, 0.0f), // top-right
        float2(u0, 1.0f), // bottom-left
        float2(u1, 1.0f) // bottom-right
    };

    [unroll]
    for (uint i = 0; i < 4; ++i)
    {
        float angle = (i % 2 == 0) ? angle0 : angle1;
        float3 normalO = float3(cos(angle), 0.0f, sin(angle));
        float3 normalW = normalize(mul(normalO, (float3x3) cylinder.worldInverseTranspose));

        verts[i].position = mul(worldPos[i], gCamera.viewProjection);
        verts[i].uv = mul(float4(baseUV[i], 0.0f, 1.0f), mat.uvMatrix).xy;
        verts[i].normal = normalW;
        verts[i].instanceIndex = instanceID;
    }

    if (hasCap)
    {
        // 上面中心 (index 4)
        verts[4].position = mul(worldPos[4], gCamera.viewProjection);
        verts[4].uv = float2(0.5f, 0.5f); // 任意
        verts[4].normal = normalize(mul(float3(0, 1, 0), (float3x3) cylinder.worldInverseTranspose));
        verts[4].instanceIndex = instanceID;

        // 底面中心 (index 5)
        verts[5].position = mul(worldPos[5], gCamera.viewProjection);
        verts[5].uv = float2(0.5f, 0.5f); // 任意
        verts[5].normal = normalize(mul(float3(0, -1, 0), (float3x3) cylinder.worldInverseTranspose));
        verts[5].instanceIndex = instanceID;
    }

    // 側面
    tris[0] = uint3(0, 2, 1);
    tris[1] = uint3(2, 3, 1);

    // キャップ
    if (hasCap)
    {
        tris[2] = uint3(4, 1, 0); // 上面（CWで +Y 法線）
        tris[3] = uint3(5, 2, 3); // 底面（CWで -Y 法線）
    }
}

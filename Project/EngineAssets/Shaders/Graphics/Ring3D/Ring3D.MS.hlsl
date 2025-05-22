#include "Ring3D.hlsli"
#include "../Primitive3D/Primitive3D.hlsli"

#define MAX_VERTICES 4
#define MAX_TRIANGLES 2

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<RingData3D> gInstanceData : register(t0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

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

    RingData3D ring = gInstanceData[instanceID];
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    uint divide = ring.ringDivide;
    
    bool isActive = (tileID < divide);
    SetMeshOutputCounts(isActive ? 4 : 0, isActive ? 2 : 0);
    if (!isActive)
        return;

    float angle0 = tileID * ring.radianParDivide;
    float angle1 = (tileID + 1) * ring.radianParDivide;

    float cos0 = cos(angle0);
    float sin0 = sin(angle0);
    float cos1 = cos(angle1);
    float sin1 = sin(angle1);

    // XY平面でのリング頂点
    float3 vertices[4] =
    {
        float3(cos0 * ring.outerRadius, sin0 * ring.outerRadius, 0.0f), // 外側開始
        float3(cos1 * ring.outerRadius, sin1 * ring.outerRadius, 0.0f), // 外側終了
        float3(cos0 * ring.innerRadius, sin0 * ring.innerRadius, 0.0f), // 内側開始
        float3(cos1 * ring.innerRadius, sin1 * ring.innerRadius, 0.0f) // 内側終了
    };
    

    float4 positions[4] =
    {
        mul(float4(vertices[0], 1.0f), ring.worldMatrix),
        mul(float4(vertices[1], 1.0f), ring.worldMatrix),
        mul(float4(vertices[2], 1.0f), ring.worldMatrix),
        mul(float4(vertices[3], 1.0f), ring.worldMatrix)
    };

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
        float4 localPos = float4(vertices[i], 1.0f);
        float4 worldPos = mul(localPos, ring.worldMatrix);
        float4 clipPos = mul(positions[i], gCamera.viewProjection);
        float2 uv = mul(float4(baseUVs[i], 0.0f, 1.0f), mat.uvMatrix).xy;
        float3 normal = normalize(mul(float3(0.0f, 0.0f, -1.0f), (float3x3) ring.worldInverseTranspose));
        
        verts[i].position = clipPos;
        verts[i].uv = uv;
        verts[i].instanceIndex = instanceID;
        verts[i].normal = normal;
    }


    tris[0] = uint3(0, 2, 1);
    tris[1] = uint3(2, 3, 1);
}
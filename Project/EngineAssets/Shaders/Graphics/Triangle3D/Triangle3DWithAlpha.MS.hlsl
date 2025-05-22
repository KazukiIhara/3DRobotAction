#include "Triangle3D.hlsli"
#include "../Primitive3D/Primitive3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<TriangleData3D> gInstanceData : register(t0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(in payload ASPayload payload,
          uint3 tid : SV_DispatchThreadID,
          out indices uint3 tris[1],
          out vertices MeshOutputWithAlpha verts[3])
{
    const uint instanceID = payload.instanceID;

    TriangleData3D data = gInstanceData[instanceID];
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    SetMeshOutputCounts(3, 1);

    float2 uvs[3] =
    {
        float2(0.0f, 1.0f), // 左下
        float2(0.5f, 0.0f), // 上
        float2(1.0f, 1.0f), // 右下
    };

    [unroll]
    for (uint i = 0; i < 3; ++i)
    {
        float4 localPos = float4(data.vertices[i].xyz, 1.0f);
        float4 worldPos = mul(localPos, data.worldMatrix);
        float4 clipPos = mul(worldPos, gCamera.viewProjection);
        float2 uv = mul(float4(uvs[i], 0.0f, 1.0f), mat.uvMatrix).xy;

        // 頂点出力にセット
        verts[i].position = clipPos;
        verts[i].uv = uv;
        verts[i].instanceIndex = instanceID;
    }

    tris[0] = uint3(0, 1, 2);
}

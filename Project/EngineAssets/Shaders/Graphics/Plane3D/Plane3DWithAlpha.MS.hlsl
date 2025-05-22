#include "Plane3D.hlsli"
#include "../Primitive3D/Primitive3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<PlaneData3D> gInstanceData : register(t0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(
    in payload ASPayload payload,
    uint3 dispatchThreadID : SV_DispatchThreadID,
    out indices uint3 tris[2],
    out vertices MeshOutputWithAlpha verts[4]
)
{
    uint instanceID = payload.instanceID;

    PlaneData3D plane = gInstanceData[instanceID];
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    SetMeshOutputCounts(4, 2); // 2 triangles from 4 vertices

    float4 positions[4] =
    {
        plane.offsets[0], // LT
        plane.offsets[1], // RT
        plane.offsets[2], // LB
        plane.offsets[3]  // RB
    };

    for (uint i = 0; i < 4; ++i)
    {
        float4 worldPos = mul(positions[i], plane.worldMatrix);
        float4 clipPos = mul(worldPos, gCamera.viewProjection);
        float2 uv = mul(float4(QuadUVs[i], 0.0f, 1.0f), mat.uvMatrix).xy;

        verts[i].position = clipPos;
        verts[i].uv = uv;
        verts[i].instanceIndex = instanceID;
    }

    // 2�̎O�p�`���\���i�C���f�b�N�X��6�g�p�j
    tris[0] = uint3(0, 1, 2);
    tris[1] = uint3(2, 1, 3);
}

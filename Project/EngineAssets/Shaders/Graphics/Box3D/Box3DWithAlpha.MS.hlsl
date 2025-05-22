#include "Box3D.hlsli"
#include "../Primitive3D/Primitive3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<BoxData3D> gInstanceData : register(t0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

// 8 corner ���_�C���f�b�N�X �� 6 �� �~ 4 corner �ւ̓W�J
static const uint kCornerLUT[6][4] =
{
    /* Front  */
    { 0, 1, 2, 3 }, // -Z
    /* Back   */
    { 5, 4, 7, 6 }, // +Z
    /* Left   */
    { 4, 0, 6, 2 }, // -X
    /* Right  */
    { 1, 5, 3, 7 }, // +X
    /* Top    */
    { 4, 5, 0, 1 }, // +Y
    /* Bottom */
    { 2, 3, 6, 7 } // -Y
};

// �ʃ��[�J���@���i������W�n�� -Z ���O�j
static const float3 kFaceNormal[6] =
{
    float3(0, 0, -1), // Front
    float3(0, 0, 1), // Back
    float3(-1, 0, 0), // Left
    float3(1, 0, 0), // Right
    float3(0, 1, 0), // Top
    float3(0, -1, 0) // Bottom
};

// �ʂ��Ƃ� 2 ���|���S���i4 ���_�j�� CCW �C���f�b�N�X
static const uint3 kTriLUT[12] =
{
    // Front
    uint3(0, 1, 2), uint3(2, 1, 3),
    // Back
    uint3(4, 5, 6), uint3(6, 5, 7),
    // Left
    uint3(8, 9, 10), uint3(10, 9, 11),
    // Right
    uint3(12, 13, 14), uint3(14, 13, 15),
    // Top
    uint3(16, 17, 18), uint3(18, 17, 19),
    // Bottom
    uint3(20, 21, 22), uint3(22, 21, 23)
};

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(
    in payload ASPayload payload,
    uint3 dispatchThreadID : SV_DispatchThreadID,
    out indices uint3 tris[12],
    out vertices MeshOutputWithAlpha verts[24]
)
{
    uint instanceID = payload.instanceID;

    BoxData3D box = gInstanceData[instanceID];
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    // 1) �o�͒��_���O�p�`��
    SetMeshOutputCounts(24, 12);

    // 2) 24 ���_����
    [unroll]
    for (uint face = 0; face < 6; ++face)
    {
        float3 localN = kFaceNormal[face];
        float3 worldN = normalize(
            mul(localN, (float3x3) box.worldInverseTranspose));

        [unroll]
        for (uint corner = 0; corner < 4; ++corner)
        {
            uint outIdx = face * 4 + corner;
            uint inIdx = kCornerLUT[face][corner];

            // ���[�J�� �� ���[���h �� �N���b�v
            float4 localPos = box.offsets[inIdx];
            float4 worldPos = mul(localPos, box.worldMatrix);
            float4 clipPos = mul(worldPos, gCamera.viewProjection);

            // UV �ϊ�
            float2 uv = mul(float4(QuadUVs[corner], 0.0f, 1.0f), mat.uvMatrix).xy;

            verts[outIdx].position = clipPos;
            verts[outIdx].uv = uv;
            verts[outIdx].instanceIndex = instanceID;
        }
    }

    // 3) �C���f�b�N�X
    [unroll]
    for (uint i = 0; i < 12; ++i)
    {
        tris[i] = kTriLUT[i];
    }
}
        
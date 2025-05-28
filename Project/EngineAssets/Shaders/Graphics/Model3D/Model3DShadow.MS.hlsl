#include "Model3D.hlsli"

// �o�C���h����o�b�t�@
ConstantBuffer<DirectionalLightCamera> gCamera : register(b0);
StructuredBuffer<ModelDataForGPU> gInstanceData : register(t0);
StructuredBuffer<VertexData3D> gVertexBuffer : register(t1);
StructuredBuffer<Meshlet> gMeshlets : register(t2);
ByteAddressBuffer gUniqueVertexIndices : register(t3);
ByteAddressBuffer gPrimitiveIndices : register(t4);
ConstantBuffer<MeshInfo> gMeshInfo : register(b2);

// 10bit�p�b�N�v���~�e�B�u�̃A���p�b�N
inline uint3 UnpackPrimitive(uint v)
{
    return uint3(v & 0x3FF, (v >> 10) & 0x3FF, (v >> 20) & 0x3FF);
}

// ���V�����b�g���璸�_�C���f�b�N�X���擾
uint GetVertexIndex(in Meshlet m, uint local)
{
    uint global = m.VertOffset + local;

    if (gMeshInfo.IndexSize == 4)
        return gUniqueVertexIndices.Load(global * 4);

    uint word = global & 1;
    uint bytes = (global >> 1) * 4;
    uint pair = gUniqueVertexIndices.Load(bytes);
    return (pair >> (word * 16)) & 0xFFFF;
}

// ���V�����b�g����v���~�e�B�u���擾
uint3 GetPrimitive(in Meshlet m, uint localPrim)
{
    uint packed = gPrimitiveIndices.Load((m.PrimOffset + localPrim) * 4);
    return UnpackPrimitive(packed);
}

// ���_�f�[�^�����[���h�ϊ����r���[�v���W�F�N�V�����K�p
ShadowMeshOutput GetVertexAttributes(uint vertexIndex, uint instID)
{
    VertexData3D v = gVertexBuffer[vertexIndex];
    ModelDataForGPU instData = gInstanceData[instID];

    ShadowMeshOutput vout;
    vout.position = mul(v.position, mul(instData.world, gCamera.viewProjection));
    return vout;
}

// MS�{��
[outputtopology("triangle")]
[numthreads(128, 1, 1)]
void main(
     uint gtid : SV_GroupThreadID,
     uint gid : SV_GroupID,
     in payload Payload payload,
     out vertices ShadowMeshOutput verts[256],
     out indices uint3 tris[256]
)
{
    const uint instID = payload.instanceID;
    uint meshletIndex = payload.meshletIndices[gid];

    Meshlet m = gMeshlets[meshletIndex];

    bool visible = meshletIndex < gMeshInfo.MeshletCount;
    
    if (visible && !gInstanceData[instID].isMakeShadow)
    {
        visible = false;
    }
    
    if (!visible)
    {
        m.VertCount = 0;
        m.PrimCount = 0;
    }

    SetMeshOutputCounts(m.VertCount, m.PrimCount);
    
    if (!visible)
    {
        return;
    }

    // ���_�o��
    if (gtid.x < m.VertCount)
    {
        uint vertexIndex = GetVertexIndex(m, gtid);
        verts[gtid] = GetVertexAttributes(vertexIndex, instID);
    }

    // �C���f�b�N�X�o��
    if (gtid.x < m.PrimCount)
    {
        tris[gtid] = GetPrimitive(m, gtid);
    }
}

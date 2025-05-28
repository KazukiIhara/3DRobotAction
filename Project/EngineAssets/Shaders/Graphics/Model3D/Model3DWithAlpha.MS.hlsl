#include "Model3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
ConstantBuffer<MeshInfo> gMeshInfo : register(b2);
StructuredBuffer<ModelDataForGPU> gInstanceData : register(t0);
StructuredBuffer<VertexData3D> gVertexBuffer : register(t1);
StructuredBuffer<Meshlet> gMeshlets : register(t2);
ByteAddressBuffer gUniqueVertexIndices : register(t3);
ByteAddressBuffer gPrimitiveIndices : register(t4);

// ───── 10bit プリミティブ unpack ────
inline uint3 UnpackPrimitive(uint v)
{
    return uint3(v & 0x3FF, (v >> 10) & 0x3FF, (v >> 20) & 0x3FF);
}

// ───── Meshlet から頂点/三角形を読む ─────
uint GetVertexIndex(in Meshlet m, uint local)
{
    uint global = m.VertOffset + local;

    if (gMeshInfo.IndexSize == 4)                     // 32‑bit index
        return gUniqueVertexIndices.Load(global * 4);

    /* 16‑bit index -------------------------------------------------------- */
    uint word = global & 1;
    uint bytes = (global >> 1) * 4;
    uint pair = gUniqueVertexIndices.Load(bytes);
    return (pair >> (word * 16)) & 0xFFFF;
}

uint3 GetPrimitive(in Meshlet m, uint localPrim)
{
    uint packed = gPrimitiveIndices.Load((m.PrimOffset + localPrim) * 4);
    return UnpackPrimitive(packed);
}

MeshOutputWithAlpha GetVertexAttributes(uint vertexIndex, uint instID)
{
    VertexData3D v = gVertexBuffer[vertexIndex];
    ModelDataForGPU instData = gInstanceData[instID];
    
    MeshOutputWithAlpha vout;
    vout.position = mul(v.position, mul(instData.world, gCamera.viewProjection));
    vout.uv = v.uv;
    vout.color = instData.color;

    return vout;
}
// ───── MS 本体 ─────
[outputtopology("triangle")]
[numthreads(128, 1, 1)]
void main(
     uint gtid : SV_GroupThreadID,
     uint gid : SV_GroupID,
     in payload Payload payload,
     out vertices MeshOutputWithAlpha verts[256],
     out indices uint3 tris[256]
)
{
    const uint instID = payload.instanceID;
    uint meshletIndex = payload.meshletIndices[gid];

    Meshlet m = gMeshlets[meshletIndex];
    
    if (meshletIndex >= gMeshInfo.MeshletCount)
    {
        m.VertCount = 0;
        m.PrimCount = 0;
    }

    SetMeshOutputCounts(m.VertCount, m.PrimCount);

    //------------------- 頂点 --------------------
    if (gtid.x < m.VertCount)
    {
        uint vertexIndex = GetVertexIndex(m, gtid);
        verts[gtid] = GetVertexAttributes(vertexIndex, instID);
    }

    if (gtid.x < m.PrimCount)
    {
        tris[gtid] = GetPrimitive(m, gtid);
    }
}
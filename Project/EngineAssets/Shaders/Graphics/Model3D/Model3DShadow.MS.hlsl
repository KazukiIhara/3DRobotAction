#include "Model3D.hlsli"

// バインドするバッファ
ConstantBuffer<DirectionalLightCamera> gCamera : register(b0);
StructuredBuffer<ModelDataForGPU> gInstanceData : register(t0);
StructuredBuffer<VertexData3D> gVertexBuffer : register(t1);
StructuredBuffer<Meshlet> gMeshlets : register(t2);
ByteAddressBuffer gUniqueVertexIndices : register(t3);
ByteAddressBuffer gPrimitiveIndices : register(t4);
ConstantBuffer<MeshInfo> gMeshInfo : register(b2);

// 10bitパックプリミティブのアンパック
inline uint3 UnpackPrimitive(uint v)
{
    return uint3(v & 0x3FF, (v >> 10) & 0x3FF, (v >> 20) & 0x3FF);
}

// メシュレットから頂点インデックスを取得
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

// メシュレットからプリミティブを取得
uint3 GetPrimitive(in Meshlet m, uint localPrim)
{
    uint packed = gPrimitiveIndices.Load((m.PrimOffset + localPrim) * 4);
    return UnpackPrimitive(packed);
}

// 頂点データをワールド変換＆ビュープロジェクション適用
ShadowMeshOutput GetVertexAttributes(uint vertexIndex, uint instID)
{
    VertexData3D v = gVertexBuffer[vertexIndex];
    ModelDataForGPU instData = gInstanceData[instID];

    ShadowMeshOutput vout;
    vout.position = mul(v.position, mul(instData.world, gCamera.viewProjection));
    return vout;
}

// MS本体
[outputtopology("triangle")]
[numthreads(128, 1, 1)]
void main(
     uint3 gtid : SV_GroupThreadID,
     uint3 gid : SV_GroupID,
     in payload Payload payload,
     out vertices ShadowMeshOutput verts[256],
     out indices uint3 tris[256]
)
{
    const uint instID = payload.instanceID;
    uint meshletID = payload.meshletIndices[gid.x];

    Meshlet m = gMeshlets[meshletID];

    SetMeshOutputCounts(m.VertCount, m.PrimCount);

    // 頂点出力
    if (gtid.x < m.VertCount)
    {
        uint vertexIndex = GetVertexIndex(m, gtid.x);
        verts[gtid.x] = GetVertexAttributes(vertexIndex, instID);
    }

    // インデックス出力
    if (gtid.x < m.PrimCount)
    {
        tris[gtid.x] = GetPrimitive(m, gtid.x);
    }
}

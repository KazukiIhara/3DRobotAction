#include "Sprite.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<SpriteData> gInstanceData : register(t0);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(
    uint3 dispatchThreadID : SV_DispatchThreadID,
    out indices uint3 tris[2],
    out vertices MeshOutput verts[4]
)
{
    uint instanceID = dispatchThreadID.y;
    
    SpriteData sprite = gInstanceData[instanceID];
    
    SetMeshOutputCounts(4, 2);
    
    float4 positions[4] =
    {
        float4(sprite.vertexPos[0], 0.0f, 1.0f), // LT
        float4(sprite.vertexPos[1], 0.0f, 1.0f), // RT
        float4(sprite.vertexPos[2], 0.0f, 1.0f), // LB
        float4(sprite.vertexPos[3], 0.0f, 1.0f), // RB
    };

    float2 uvs[4] =
    {
        sprite.texcoord[0],
        sprite.texcoord[1],
        sprite.texcoord[2],
        sprite.texcoord[3]
    };

    for (uint i = 0; i < 4; ++i)
    {
        float4 worldPos = mul(positions[i], sprite.worldMatrix);
        float4 clipPos = mul(worldPos, gCamera.viewProjection);
        float2 uv = mul(float4(uvs[i], 0.0f, 1.0f), sprite.uvMatrix).xy;

        verts[i].position = clipPos;
        verts[i].uv = uv;
        verts[i].instanceIndex = instanceID;
    }
    
    // 2つの三角形を構成（インデックスは6個使用）
    tris[0] = uint3(0, 1, 2);
    tris[1] = uint3(2, 1, 3);
}
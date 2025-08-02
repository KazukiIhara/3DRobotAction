#include "../../Compute/ParticleEffect/ParticleEffect.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
ConstantBuffer<CameraVector> gCameraVec : register(b1);
StructuredBuffer<Particle> gInstanceData : register(t0);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(in payload ASPayload payload,
    uint3 dispatchThreadID : SV_DispatchThreadID,
    out indices uint3 tris[2],
    out vertices MeshOutput verts[4]
)
{
    uint instanceID = payload.instanceID;
    Particle particle = gInstanceData[instanceID];
    
    SetMeshOutputCounts(4, 2);
    
    const float3 worldUp = { 0, 1, 0 };
    const float3 camPos = gCamera.worldPosition;
    const float3 objPos = particle.pos;
    
    const float3 forward = normalize(gCameraVec.forward);
    const float3 right = normalize(gCameraVec.right);
    const float3 up = normalize(gCameraVec.up);
    
    float sx = particle.size.x * 0.5f; // 横幅の半分
    float sy = particle.size.y * 0.5f; // 縦幅の半分

    float4x4 wMat;

    // 行 0 : Right 方向（X 軸）
    wMat[0] = float4(right * sx, 0.0f);
    // 行 1 : Up 方向（Y 軸）
    wMat[1] = float4(up * sy, 0.0f);
    // 行 2 : Forward 方向（Z 軸）
    wMat[2] = float4(forward, 0.0f);
    // 行 3 : 平行移動（ワールド位置）
    wMat[3] = float4(objPos, 1.0f);
    
    for (uint i = 0; i < 4; ++i)
    {
        float4 worldPos = mul(kPlane[i], wMat);
        float4 clipPos = mul(worldPos, gCamera.viewProjection);
        
        verts[i].position = clipPos;
        verts[i].uv = kPlaneUVs[i];
        verts[i].instanceIndex = instanceID;
    }
}
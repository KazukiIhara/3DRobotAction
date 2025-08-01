#include "Model3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
ConstantBuffer<MeshInfo> gMeshInfo : register(b2);
ConstantBuffer<CameraFrustum> gFrustum : register(b3);

StructuredBuffer<ModelDataForGPU> gInstance : register(t0);
StructuredBuffer<CullData> gCullData : register(t5);

[shader("amplification")]
[numthreads(AS_GROUP_SIZE, 1, 1)]
void main(uint3 dtid : SV_DispatchThreadID)
{
    uint meshletID = dtid.x;
    uint instanceID = dtid.y;
    
    bool visible = false;
   
    Payload payload;
    payload.instanceID = instanceID;
    
    if (dtid.x < gMeshInfo.MeshletCount)
    {
        // TODO:スキニングモデルはGPUカリングを行わないようにする
        visible = IsVisible(gCullData[meshletID], gInstance[instanceID].world, gCamera.worldPosition, gFrustum.planes);
    }
    
    if (visible)
    {
        uint index = WavePrefixCountBits(visible);
        payload.meshletIndices[index] = meshletID;
    }
    
    uint visibleCount = WaveActiveCountBits(visible);

    DispatchMesh(visibleCount, 1, 1, payload);
}

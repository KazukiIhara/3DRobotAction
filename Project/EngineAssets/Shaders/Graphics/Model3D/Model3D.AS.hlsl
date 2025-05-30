#include "Model3D.hlsli"

ConstantBuffer<MeshInfo> gMeshInfo : register(b2);

[shader("amplification")]
[numthreads(AS_GROUP_SIZE, 1, 1)]
void main(uint3 dtid : SV_DispatchThreadID)
{
    bool visible = false;
   
    Payload payload;
    payload.instanceID = dtid.y;
    
    if (dtid.x < gMeshInfo.MeshletCount)
    {
        visible = true;
    }
    
    if (visible)
    {
        uint index = WavePrefixCountBits(visible);
        payload.meshletIndices[index] = dtid.x;
    }
    
    uint visibleCount = WaveActiveCountBits(visible);

    DispatchMesh(visibleCount, 1, 1, payload);
}

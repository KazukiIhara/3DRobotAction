#include "Sphere3D.hlsli"
#include "../Primitive3D/Primitive3D.hlsli"

ConstantBuffer<RootConstants> gRootConstant : register(b1);
StructuredBuffer<SphereData3D> gInstanceData : register(t0);

[numthreads(1, 1, 1)]
void main(uint3 tid : SV_DispatchThreadID)
{
    uint instanceID = gRootConstant.baseInstanceIndex + tid.y;
    SphereData3D sphere = gInstanceData[instanceID];
    
    uint tileCount = sphere.longitudeSegments * sphere.latitudeSegments;

    // à¿ëSêßå¿
    tileCount = min(tileCount, kMaxTileCount);

    ASPayload payload;
    payload.instanceID = instanceID;

    DispatchMesh(tileCount, 1, 1, payload);
}
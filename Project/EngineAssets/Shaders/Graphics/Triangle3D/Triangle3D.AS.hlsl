#include "Triangle3D.hlsli"
#include "../Primitive3D/Primitive3D.hlsli"

ConstantBuffer<RootConstants> gRootConstant : register(b1);

[numthreads(1, 1, 1)]
void main(uint3 tid : SV_DispatchThreadID)
{
    ASPayload payload;
    // ベース + ローカルスレッド ID でインスタンスを一意に
    payload.instanceID = gRootConstant.baseInstanceIndex + tid.y;
    
    // 1×1×1 の MeshShader グループを発行
    DispatchMesh(1, 1, 1, payload);
}
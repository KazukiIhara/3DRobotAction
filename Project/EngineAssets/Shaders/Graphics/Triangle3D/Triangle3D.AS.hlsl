#include "Triangle3D.hlsli"
#include "../Primitive3D/Primitive3D.hlsli"

ConstantBuffer<RootConstants> gRootConstant : register(b1);

[numthreads(1, 1, 1)]
void main(uint3 tid : SV_DispatchThreadID)
{
    ASPayload payload;
    // �x�[�X + ���[�J���X���b�h ID �ŃC���X�^���X����ӂ�
    payload.instanceID = gRootConstant.baseInstanceIndex + tid.y;
    
    // 1�~1�~1 �� MeshShader �O���[�v�𔭍s
    DispatchMesh(1, 1, 1, payload);
}
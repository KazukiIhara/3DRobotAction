#include "Object3DGroup.hlsli"

StructuredBuffer<Object3DForGPU> gInstanceData : register(t3);
ConstantBuffer<Camera> gCamera : register(b1);

VertexShaderOutput main(VertexShaderInput input, uint instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    output.position = mul(input.position, mul(gInstanceData[instanceId].World, gCamera.viewProjection));
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) gInstanceData[instanceId].WorldInverseTranspose));
    output.tangent = normalize(mul(input.tangent, (float3x3) gInstanceData[instanceId].WorldInverseTranspose));
    output.worldPosition = mul(input.position, gInstanceData[instanceId].World).xyz;
    return output;
}
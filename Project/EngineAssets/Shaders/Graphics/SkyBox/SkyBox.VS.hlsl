#include "SkyBox.hlsli"

ConstantBuffer<SkyBoxData> gSkyBoxData : register(b0);
ConstantBuffer<Camera> gCamera : register(b1);

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, mul(gSkyBoxData.worldMatrix, gCamera.viewProjection)).xyww;
    output.texcoord = input.position.xyz;
    
    return output;
}
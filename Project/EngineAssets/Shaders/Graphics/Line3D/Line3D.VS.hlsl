#include "Line3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<LineData3D> gLines : register(t0);

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;

    // 頂点インデックスに基づいて始点/終点を計算
    LineData3D lineData = gLines[input.instanceId];
    float3 position = (input.vertexId == 0) ? lineData.start : lineData.end;

    // ワールド変換
    output.position = mul(float4(position, 1.0f), gCamera.viewProjection);

    output.color = lineData.color;
    return output;
}
#include "Model3D.hlsli"

// マテリアル定数バッファ
ConstantBuffer<MaterialData3D> gMaterialData : register(b1);

// テクスチャ（Bindless）
Texture2D<float4> gTextures[1024] : register(t1000);

// サンプラー
SamplerState gSampler : register(s0);

float4 main(MeshOutputWithAlpha input) :SV_Target0
{
    float4 output;

    // --- アルベド出力 ---
    float2 uv = mul(float4(input.uv, 0.0f, 1.0f), gMaterialData.uvMatrix).xy;
    float4 texColor = gTextures[gMaterialData.texIdx].Sample(gSampler, uv);
    output = texColor * gMaterialData.baseColor;

    return output;
}

#include "Model3D.hlsli"

// マテリアル定数バッファ
ConstantBuffer<MaterialData3D> gMaterialData : register(b1);

// テクスチャ（Bindless）
Texture2D<float4> gTextures[1024] : register(t1000);

// サンプラー
SamplerState gSampler : register(s0);

// 3出力の構造体
struct GBufferOutput
{
    float4 albedo : SV_Target0;
    float4 normal : SV_Target1;
};

GBufferOutput main(MeshOutput input)
{
    GBufferOutput output;

    // --- アルベド出力 ---
    float2 uv = mul(float4(input.uv, 0.0f, 1.0f), gMaterialData.uvMatrix).xy;
    float4 texColor = gTextures[gMaterialData.texIdx].Sample(gSampler, uv);
    output.albedo = texColor * gMaterialData.baseColor * input.color;

    // --- 法線出力 ---
    float3 normal = normalize(input.normal); // 念のため正規化
    output.normal = float4(normal * 0.5f + 0.5f, 1.0f); // [-1,1] → [0,1]マッピングして格納
    

    return output;
}

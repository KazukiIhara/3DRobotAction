#include "DepthOutline.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<DepthOutlineData> gdata : register(b0);

// 深度テクスチャ
Texture2D<float> gDepthTexture : register(t1);

// ポイント比較用サンプラー
SamplerState gSamplerPoint : register(s1);

float4 main() : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
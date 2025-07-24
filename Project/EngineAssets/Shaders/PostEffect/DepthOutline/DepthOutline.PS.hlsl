#include "DepthOutline.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<DepthOutlineData> gdata : register(b0);
ConstantBuffer<CameraInv> gCameraInv : register(b1);

// 深度テクスチャ
Texture2D<float> gDepthTexture : register(t1);

// ポイント比較用サンプラー
SamplerState gSamplerPoint : register(s1);


static const float2 kIndex[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
};

static const float kPrewittHorizontalKernel[3][3] =
{
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
};

static const float kPrewittVerticalKernel[3][3] =
{
    { -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
    { 0.0f, 0.0f, 0.0f },
    { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f },
};

float Luminance(float3 v)
{
    return dot(v, float3(0.2125f, 0.7154f, 0.0721f));
}

PixelShaderOutput main(VertexShaderOutput input)
{
    int width, height;
    gTexture.GetDimensions(width, height);
    float2 uvStepSize = float2(rcp(width), rcp(height));
    
    float2 differnce = float2(0.0f, 0.0f);
    
    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            float2 texcoord = input.texcoord + kIndex[x][y] * uvStepSize;
            
            float ndcDepth = gDepthTexture.Sample(gSamplerPoint, texcoord);
            float4 viewSpace = mul(float4(0.0f, 0.0f, ndcDepth, 1.0f), gCameraInv.invProj);
            float viewZ = viewSpace.z * rcp(viewSpace.w);
            
            differnce.x += viewZ * kPrewittHorizontalKernel[x][y];
            differnce.y += viewZ * kPrewittVerticalKernel[x][y];
        }
    }
    
    float weight = length(differnce);
    weight = saturate(weight + 0.1f);
    
    PixelShaderOutput output;
    
    float4 sampleColor = gTexture.Sample(gSampler, input.texcoord);
    output.color.rgb = (1.0f - weight) * sampleColor.rgb;
    output.color.a = sampleColor.a;
    
    return output;
}
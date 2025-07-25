#include "DepthNormalOutline.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

Texture2D<float> gDepthTexture : register(t1);
Texture2D<float4> gNormalTexture : register(t2);
SamplerState gSamplerPoint : register(s1);

ConstantBuffer<DepthNormalOutlineData> gdata : register(b0);
ConstantBuffer<CameraInv> gCameraInv : register(b1);

static const float2 kIndex[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } }
};

static const float kPrewittH[3][3] =
{
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f }
};

static const float kPrewittV[3][3] =
{
    { -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
    { 0.0f, 0.0f, 0.0f },
    { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f }
};

static const float kDepthMul = 0.5f;
static const float kDepthAttenScale = 0.02f;
static const float kNormalMul = 3.0f;
static const float kThreshold = 0.7f;

float LinearizeDepth(float ndcZ)
{
    float4 vPos = mul(float4(0.0f, 0.0f, ndcZ, 1.0f), gCameraInv.invProj);
    return vPos.z * rcp(vPos.w);
}

float3 DecodeNormal(float3 enc)       // [0,1] → [-1,1]
{
    return normalize(enc * 2.0f - 1.0f);
}

PixelShaderOutput main(VertexShaderOutput input)
{
    int width, height;
    gTexture.GetDimensions(width, height);
    float2 uvStep = float2(rcp(width), rcp(height));

    float zCenter = LinearizeDepth(gDepthTexture.Sample(gSamplerPoint, input.texcoord));
    float3 nCenter = DecodeNormal(gNormalTexture.Sample(gSamplerPoint, input.texcoord).xyz);

    float2 gradDepth = 0.0f;
    float2 gradNormal = 0.0f;

    [unroll]
    for (int ix = 0; ix < 3; ++ix)
    {
        [unroll]
        for (int iy = 0; iy < 3; ++iy)
        {
            float2 offsetUV = input.texcoord + kIndex[ix][iy] * uvStep;

            //---- 深度勾配 ----
            float z = LinearizeDepth(gDepthTexture.Sample(gSamplerPoint, offsetUV));
            gradDepth.x += z * kPrewittH[ix][iy];
            gradDepth.y += z * kPrewittV[ix][iy];

            //---- 法線勾配 ----  (中央との差分ベースでノイズ耐性↑)
            float3 n = DecodeNormal(gNormalTexture.Sample(gSamplerPoint, offsetUV).xyz);
            float deltaN = 1.0f - dot(nCenter, n); // [0,2]
            gradNormal.x += deltaN * kPrewittH[ix][iy];
            gradNormal.y += deltaN * kPrewittV[ix][iy];
        }
    }

    float depthEdge = length(gradDepth);
    float normalEdge = length(gradNormal);

    // 正規化 & 重み付け
    float depthAtten = saturate(1.0f - zCenter * kDepthAttenScale);
    depthEdge = saturate(depthEdge * kDepthMul * depthAtten);
    normalEdge = saturate(normalEdge * kNormalMul);

    // 深度/法線のどちらかが閾値を超えればアウトライン
    float edgeStrength = max(depthEdge, normalEdge);
    float edgeMask = step(kThreshold, edgeStrength); // 0 or 1

    PixelShaderOutput o;
    float3 sceneCol = gTexture.Sample(gSampler, input.texcoord).rgb;
    o.color = float4(lerp(sceneCol, 0.0f.xxx, edgeMask), 1.0f); // 黒線
    return o;
}
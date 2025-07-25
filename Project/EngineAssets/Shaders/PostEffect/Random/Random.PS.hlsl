#include "Random.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<RandomData> gdata : register(b0);

float rand2dTo1d(float2 value, float2 dotDir = float2(12.9898, 78.233))
{
    float2 smallValue = sin(value);
    float random = dot(smallValue, dotDir);
    random = frac(sin(random) * 143758.5453);
    return random;
}

PixelShaderOutput main(VertexShaderOutput input)
{
    float random = rand2dTo1d(input.texcoord * gdata.param0.x);
    
    PixelShaderOutput output;
    output.color = float4(random, random, random, 1.0f);
    return output;
}
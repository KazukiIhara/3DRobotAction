#include "RadialBlur.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<RadialBlurData> gdata : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    const float2 center = gdata.param0.xy;
    const float blurWidth = gdata.param1.x;
    const uint numSamples = 10;
    
    
    float3 outputColor = float3(0.0f, 0.0f, 0.0f);
    float2 direction = input.texcoord - center;
    for (uint sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
    {
        float2 texcoord = input.texcoord + direction * blurWidth * float(sampleIndex);
        outputColor += gTexture.Sample(gSampler, texcoord).rgb;
    }
    
    outputColor *= rcp(float(numSamples));
    PixelShaderOutput output;
    output.color.rgb = outputColor;
    output.color.a = 1.0f;
    return output;
}
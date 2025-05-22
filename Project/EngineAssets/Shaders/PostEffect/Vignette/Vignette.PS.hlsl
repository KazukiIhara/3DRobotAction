#include "Vignette.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<VignetteData> gdata : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    float2 correct = input.texcoord * (1.0f - input.texcoord.yx);
    float vignette = correct.x * correct.y * gdata.param0.x;
    vignette = saturate(pow(vignette, gdata.param0.y));
    output.color.rgb *= vignette;
    
    return output;
}
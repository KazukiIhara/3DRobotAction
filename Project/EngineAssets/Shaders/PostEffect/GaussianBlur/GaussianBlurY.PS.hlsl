#include "GaussianBlur.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<GaussianBlurData> gdata : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    float2 uv = input.texcoord;
    float2 texOffset = float2(0.0f, gdata.param0.y); // Y•ûŒü‚Ì‚Ý
    
    const float weights[7] = { gdata.param1.xyzw, gdata.param2.xyz };

    float4 color = gTexture.Sample(gSampler, uv) * weights[0];

   [unroll]
    for (uint i = 1; i <= 6; ++i)
    {
        if (i <= gdata.param3.x)
        {
            float2 offset = texOffset * float(i);
            color += gTexture.Sample(gSampler, uv + offset) * weights[i];
            color += gTexture.Sample(gSampler, uv - offset) * weights[i];
        }  
    }

    output.color = color;
    return output;
}

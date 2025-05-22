#include "Object2D.hlsli"

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    // Texture‚Ìa‚Ì’l‚ª0.5ˆÈ‰º‚ÌŽž‚ÉPixel‚ð”p‹p
    if (textureColor.a <= 0.5)
    {
        discard;
    }
    
    output.color = gMaterial.color * textureColor;
    
      
    // output.color‚Ìa’l‚ª0‚ÌŽž‚ÉPixel‚ð”p‹p
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    return output;
}


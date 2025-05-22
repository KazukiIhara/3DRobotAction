#include "Sprite.hlsli"

StructuredBuffer<SpriteData> gInstanceData : register(t0);
Texture2D gTextures[] : register(t1000);
SamplerState gSampler : register(s0);

float4 main(MeshOutput input) : SV_Target0
{
    float4 output;
    uint instanceID = input.instanceIndex;
    SpriteData sprite = gInstanceData[instanceID];
    
    float2 uv = input.uv;
    float4 texColor = gTextures[sprite.textureIndex].Sample(gSampler, uv);
    output = texColor * sprite.baseColor;
    
    return output;
}
#include "../Primitive3D/Primitive3D.hlsli"

Texture2D gTextures[] : register(t1000);
SamplerState gSampler : register(s0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

struct GBufferOutput
{
    float4 albedo : SV_Target0;
    float4 normal : SV_Target1;
};

GBufferOutput main(MeshOutput input)
{
    GBufferOutput output;
    
    uint instanceID = input.instanceIndex;
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    float2 uv = input.uv;
    float4 texColor = gTextures[mat.textureIndex].Sample(gSampler, uv);
    output.albedo = texColor * mat.baseColor;
    
    float3 normal = normalize(input.normal); // ”O‚Ì‚½‚ß³‹K‰»
    output.normal = float4(normal * 0.5f + 0.5f, 1.0f); // [-1,1] ¨ [0,1]ƒ}ƒbƒsƒ“ƒO‚µ‚ÄŠi”[
    
    return output;
}
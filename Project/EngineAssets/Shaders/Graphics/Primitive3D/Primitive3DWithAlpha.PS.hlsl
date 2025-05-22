#include "../Primitive3D/Primitive3D.hlsli"

Texture2D gTextures[] : register(t1000);
SamplerState gSampler : register(s0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

float4 main(MeshOutputWithAlpha input) : SV_Target0
{
    float4 output;
    
    uint instanceID = input.instanceIndex;
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    float2 uv = input.uv;
    float4 texColor = gTextures[mat.textureIndex].Sample(gSampler, uv);
    output = texColor * mat.baseColor;
    
    return output;
}
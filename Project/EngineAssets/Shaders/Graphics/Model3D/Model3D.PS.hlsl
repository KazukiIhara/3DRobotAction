#include "Model3D.hlsli"

// �}�e���A���萔�o�b�t�@
ConstantBuffer<MaterialData3D> gMaterialData : register(b1);

// �e�N�X�`���iBindless�j
Texture2D<float4> gTextures[1024] : register(t1000);

// �T���v���[
SamplerState gSampler : register(s0);

// 3�o�͂̍\����
struct GBufferOutput
{
    float4 albedo : SV_Target0;
    float4 normal : SV_Target1;
};

GBufferOutput main(MeshOutput input)
{
    GBufferOutput output;

    // --- �A���x�h�o�� ---
    float2 uv = mul(float4(input.uv, 0.0f, 1.0f), gMaterialData.uvMatrix).xy;
    float4 texColor = gTextures[gMaterialData.texIdx].Sample(gSampler, uv);
    output.albedo = texColor * gMaterialData.baseColor * input.color;

    // --- �@���o�� ---
    float3 normal = normalize(input.normal); // �O�̂��ߐ��K��
    output.normal = float4(normal * 0.5f + 0.5f, 1.0f); // [-1,1] �� [0,1]�}�b�s���O���Ċi�[
    

    return output;
}


//-----------------------------------
// �萔
//-----------------------------------

static const float2 QuadUVs[4] =
{
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f),
    float2(0.0f, 1.0f),
    float2(1.0f, 1.0f)
};

// Mesh�V�F�[�_�[����s�N�Z���V�F�[�_�[�ɓn���\����
struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    uint instanceIndex : TEXCOORD1;
    float3 normal : NORMAL0;
};

// �������Mesh�V�F�[�_�[����s�N�Z���V�F�[�_�[�ɓn���\����
struct MeshOutputWithAlpha
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    uint instanceIndex : TEXCOORD1;
};

// �V���h�E�p�y��Vertex�o��
struct ShadowMeshOutput
{
    float4 position : SV_Position;
};

// �}�e���A���f�[�^
struct PrimitiveMaterialData3D
{
    uint textureIndex;
    float3 _padding0;
    float4 baseColor;
    float4x4 uvMatrix;
};

// �J����
struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
    float _padding;
};

// ���[�g�萔
struct RootConstants
{
    uint baseInstanceIndex;
    uint _pad0, _pad1, _pad2;
};

// AS�Ŏg���\����
struct ASPayload
{
    uint instanceID;
};
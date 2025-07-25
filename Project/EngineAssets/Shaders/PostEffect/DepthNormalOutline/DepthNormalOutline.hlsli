struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct DepthNormalOutlineData
{
    float4 param0;
    float4 param1; 
    float4 param2;
    float4 param3;
};

struct CameraInv
{
    float4x4 invView;
    float4x4 invProj;
};
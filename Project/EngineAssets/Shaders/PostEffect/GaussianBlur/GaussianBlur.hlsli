struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct GaussianBlurData
{
    float4 param0; // xy {texelSize.x,texelSize.y}
    float4 param1; // xyzw weight[0] ~ weight[3]
    float4 param2; // xyz weight[4] ~ weight[6]
    float4 param3; // x karnelNum
};
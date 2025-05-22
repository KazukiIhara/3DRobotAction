struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct RadialBlurData
{
    float4 param0; // xy {Center.x,Center.y}
    float4 param1; // x {BlurWidth}
    float4 param2;
    float4 param3;
};
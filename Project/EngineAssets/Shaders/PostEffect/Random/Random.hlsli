struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct RandomData
{
    float4 param0; // x-seed
    float4 param1;
    float4 param2;
    float4 param3;
};
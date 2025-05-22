#include "Line3D.hlsli"

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    // 頂点カラーをそのまま出力
    output.color = input.color;

    return output;
}
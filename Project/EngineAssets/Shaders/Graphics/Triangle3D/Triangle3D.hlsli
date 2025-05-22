
// インスタンシングデータ
struct TriangleData3D
{
    float4x4 worldMatrix;
    float4x4 worldInverseTranspose;
    float4 vertices[3];
};
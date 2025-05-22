struct BoxData3D
{
    float4x4 worldMatrix;
    float4x4 worldInverseTranspose;
    float4 offsets[8];
};
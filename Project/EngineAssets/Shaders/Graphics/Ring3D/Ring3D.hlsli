struct RingData3D
{
    float4x4 worldMatrix;
    float4x4 worldInverseTranspose;
    uint ringDivide;
    float outerRadius;
    float innerRadius;
    float radianParDivide;
    float3 _padding;
};

// ƒ^ƒCƒ‹‚ÌÅ‘å”
static const uint kMaxTileCount = 256 * 256;
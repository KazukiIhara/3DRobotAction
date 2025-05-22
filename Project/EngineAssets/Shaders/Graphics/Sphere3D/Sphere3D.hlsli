struct SphereData3D
{
    float4x4 worldMatrix;
    float4x4 worldInverseTranspose;
    float radius;
    uint longitudeSegments;
    uint latitudeSegments;
    float _padding;
};

// タイルの最大数 (緯度分割数256)x(経度分割数256)
static const uint kMaxTileCount = 256 * 256;
struct SphereData3D
{
    float4x4 worldMatrix;
    float4x4 worldInverseTranspose;
    float radius;
    uint longitudeSegments;
    uint latitudeSegments;
    float _padding;
};

// �^�C���̍ő吔 (�ܓx������256)x(�o�x������256)
static const uint kMaxTileCount = 256 * 256;
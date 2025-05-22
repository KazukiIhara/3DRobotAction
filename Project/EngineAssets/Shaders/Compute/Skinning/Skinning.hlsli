
struct Vertex
{
    float4 position;
    float2 texcoord;
    float3 normal;
    float3 tangent;
};

struct VertexInfluence
{
    float4 weight;
    int4 index;
};

struct SkinningInformation
{
    int numVertices;
};

struct Well
{ 
    float4x4 skeletonSpaceMatrix;
    float4x4 skeletonSpaceInverseTransposeMatrix;
};
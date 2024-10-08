#include "PixelInput.hlsli"
#include "VertexInput.hlsli"

PixelInput main(VertexInput vertex)
{
    PixelInput output;
    
    output.position = float4(vertex.position.xyz, 1);
    output.normal = vertex.normal;
    output.uv = vertex.uv;
    
    return output;
}
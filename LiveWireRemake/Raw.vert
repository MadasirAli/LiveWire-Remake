#include "PixelInput.hlsli"
#include "VertexInput.hlsli"
#include "CameraCBuffer.hlsli"
#include "TransformCBuffer.hlsli"

PixelInput main(VertexInput vertex)
{
    PixelInput output;
    
    output.position = float4(vertex.position, 1);
    output.position = mul(output.position, WorldTransformMatrix);
    output.position = mul(output.position, ViewMatrix);
    output.position = mul(output.position, ProjectionMatrix);
    
    output.normal = vertex.normal;
    output.uv = vertex.uv;
    
    return output;
}
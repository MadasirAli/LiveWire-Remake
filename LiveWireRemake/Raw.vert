#include "PixelInput.hlsli"
#include "VertexInput.hlsli"
#include "CameraCBuffer.hlsli"
#include "TransformCBuffer.hlsli"

PixelInput main(VertexInput vertex)
{
    PixelInput output;
    
    output.position = mul(mul(float4(vertex.position.xzy, 1.0f), WorldTransformMatrix), ProjectionMatrix);
    
    output.normal = vertex.normal;
    output.uv = vertex.uv;
    
    return output;
}
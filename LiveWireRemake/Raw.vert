#include "PixelInput.hlsli"
#include "VertexInput.hlsli"
#include "CameraCBuffer.hlsli"
#include "TransformCBuffer.hlsli"

PixelInput main(VertexInput vertex)
{
    PixelInput output;
    
    matrix mvp = ProjectionMatrix * WorldTransformMatrix;
    output.normal = (float3)mul(mvp, float4(vertex.position.xyz, 1));
    
    output.position = float4(vertex.position.xyz, 1);
    
    //output.normal = vertex.normal;
    output.uv = vertex.uv;
    
    return output;
}
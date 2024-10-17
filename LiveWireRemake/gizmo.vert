#include "VertexInput.hlsli"
#include "TransformCBuffer.hlsli"
#include "CameraCBuffer.hlsli"

float4 main(VertexInput vertex) : SV_Position
{
    float4 position = float4(vertex.position, 1);
    position = mul(position, WorldTransformMatrix);
    position = mul(position, ViewMatrix);
    position = mul(position, ProjectionMatrix);
    
    return position;
}
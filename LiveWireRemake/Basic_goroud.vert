#include "PixelInput_goroud.hlsli"
#include "VertexInput.hlsli"
#include "CameraCBuffer.hlsli"
#include "TransformCBuffer.hlsli"
#include "LightCBuffer.hlsli"

PixelInput main(VertexInput vertex)
{
    PixelInput output;
    
    output.position = float4(vertex.position, 1);
    output.position = mul(output.position, WorldTransformMatrix);
    output.position = mul(output.position, ViewMatrix);
    output.position = mul(output.position, ProjectionMatrix);
    
    float4 color = float4(AmbientLightColor * AmbientLightIntensity, 0);
    color += cos(float4(LightColor * LightIntensity, 0));

    output.uv = vertex.uv;
    
    return output;
}
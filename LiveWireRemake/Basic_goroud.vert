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
    
    float4 worldPos = output.position;
    
    output.position = mul(output.position, ViewMatrix);
    output.position = mul(output.position, ProjectionMatrix);
   
    float3 normal = mul(vertex.normal, (float3x3)WorldTransformMatrix);
    normal = normalize(normal);
    
    float4 eyePos = ViewMatrix[3];
    float3 eyeDir = normalize((eyePos - worldPos).xyz);
    
    float3 lightDir = 0;
    lightDir.x = cos(radians(LightRotation.x)) * sin(radians(LightRotation.y));
    lightDir.y = -sin(radians(LightRotation.x));
    lightDir.z = cos(radians(LightRotation.x)) * cos(radians(LightRotation.y));
    
    lightDir = normalize(lightDir);
    
    float4 color = saturate(mul(dot(-lightDir, (float3)normal), LightIntensity));
    color += saturate(pow(saturate(dot(eyeDir, reflect(lightDir, normal))), 128));
    
    color += float4(AmbientLightColor * AmbientLightIntensity, 0);

    output.color = color;
    output.uv = vertex.uv;
    
    return output;
}
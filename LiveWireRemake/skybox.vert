#include "CameraCBuffer.hlsli"
#include "VertexInput.hlsli"

struct PixelInput
{
    float4 position : SV_Position;
    float4 texCoords : TEXCOORD;
};

PixelInput main(VertexInput input)
{
    PixelInput output;
    
    output.texCoords = float4(input.position, 1);
    
    output.position = float4(input.position, 1);
    output.position = float4(mul((float3)output.position, (float3x3)ViewMatrix), 1);
    output.position = mul(output.position, ProjectionMatrix);
    
    return output;
}
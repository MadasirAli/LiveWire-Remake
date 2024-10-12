#include "CameraCBuffer.hlsli"
#include "VertexInput.hlsli"

float4 main(VertexInput input) : SV_Position
{
    float4 position = float4(input.position, 1);
    position = float4(mul((float3)position, (float3x3)ViewMatrix), 1);
    
    return position.xyww;
}
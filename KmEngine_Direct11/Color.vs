////////////////////////////
// Filename: color.vs
////////////////////////////

////////////////////////////
// Globals
////////////////////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

////////////////////////////
// Typedefs
////////////////////////////
struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

////////////////////////////
// Vertex Shader
////////////////////////////
PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;

    // For proper matrix calculate, set vector unit to 4
    input.position.w = 1.0f;

    // Calculate vertex position for world, view and projection matrix
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Set color for pixel shader
    output.color = input.color;
    
    return output;
}
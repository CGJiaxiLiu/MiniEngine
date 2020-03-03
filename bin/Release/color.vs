cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	float4 const_color;
};

struct VertexInputType
{
    float4 pos : POSITION;
    float intensity : INTENSITY;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.pos.w = 1.0f;
	input.pos.z = 0.0f;
	input.pos = mul(input.pos, worldMatrix);
	//input.pos = mul(input.pos, viewMatrix);
	input.pos = mul(input.pos, projectionMatrix);
	//input.pos.x *= hRatio;
	//input.pos.y *= vRatio;
	output.position = input.pos;
	
    // Calculate the position of the vertex against the world, view, and projection matrices.
    
    //output.position = mul(output.position, viewMatrix);
    //output.position = mul(output.position, projectionMatrix);
    
    // Store the input color for the pixel shader to use.
    output.color = const_color;
	//output.color = float4(0.0f, 1.0f, 1.0f, 1.0f);
    
    return output;
}
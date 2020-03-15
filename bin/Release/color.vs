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
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
	float2 tex : TEXCOORD0;
};

PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    input.pos.w = 1.0f;
	input.pos.z = 0.0f;
	input.pos = mul(input.pos, worldMatrix);
	input.pos = mul(input.pos, viewMatrix);
	input.pos = mul(input.pos, projectionMatrix);
	
	output.position = input.pos;
    output.color = const_color;
	output.tex = input.tex;
	
    return output;
}
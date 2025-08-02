struct VSInput
{
    float2 position : POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer IMGUI_IMAGE_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 projectionMatrix;
}

cbuffer ORIGINAL_IMAGE_CONSTANT_BUFFER : register(b1)
{
 	int redElementEnabled;
    int greenElementEnabled;
    int blueElementEnabled;
    int alphaElementEnabled;
}
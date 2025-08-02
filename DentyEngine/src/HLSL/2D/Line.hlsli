struct VSInput
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VSOutput
{
	float4 position : SV_POSITION;
    float4 color : COLOR;
};
struct VSInput
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer SCENE_CONSTNAT_BUFFER : register(b0)
{
    float4 uvScrollValue;
};
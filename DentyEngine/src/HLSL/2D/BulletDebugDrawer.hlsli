struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer SCENE_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 viewProjection;
}
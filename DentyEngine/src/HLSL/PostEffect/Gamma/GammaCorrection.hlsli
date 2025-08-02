struct GammaCorrectionFunctions
{
    // Gamma 2.2
    static float3 GammaCorrectionTwoPointTwo(float3 rgb)
    {
        return pow(rgb, (1.0f / 2.2f));
    }
};
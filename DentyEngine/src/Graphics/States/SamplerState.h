#pragma once

namespace DentyEngine
{
    class SamplerState final
    {
    public:
        enum class Type
        {
            PointWrap,
            LinearWrap,
            AnisotropicWrap,
            ComparisonLinearBorder,

            Max
        };
    public:
        SamplerState(ID3D11Device* device, const D3D11_SAMPLER_DESC& desc);
        ~SamplerState() = default;

        ID3D11SamplerState* GetState() const { return _state.Get(); }

        static void CreateBuildInSamplerStates(ID3D11Device* device);
        static ID3D11SamplerState* GetBuildInSamplerStatesAt(Type type);
    private:
        ComRef<ID3D11SamplerState> _state;

        static std::vector<ComRef<ID3D11SamplerState>> _buildInSamplerStates;
        static bool _created;
    };
};

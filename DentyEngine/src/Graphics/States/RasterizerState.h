#pragma once

namespace DentyEngine
{
    class RasterizerState final
    {
    public:
        enum class Type
        {
            FillCullBack,
            WireframeCullBack,
            WireframeNoCull,
            FillCullBackClockwise,
            FillNoCull,

            Max
        };
    public:
        RasterizerState() = delete;
        RasterizerState(ID3D11Device* device, const D3D11_RASTERIZER_DESC& desc);
        ~RasterizerState() = default;

        DELETE_COPY_MOVE_CONSTRUCTORS(RasterizerState)

        ID3D11RasterizerState* GetState() const { return _state.Get(); }

        static void CreateBuildInRasterizerStates(ID3D11Device* device);
        static ID3D11RasterizerState* GetBuildInRasterizerStatesAt(Type type);
    private:
        ComRef<ID3D11RasterizerState> _state;

        static std::vector<ComRef<ID3D11RasterizerState>> _buildInRasterizerStates;
        static bool _created;
    };
};

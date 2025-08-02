#pragma once

namespace DentyEngine
{
    class DepthStencilState final
    {
    public:
        enum class Type
        {
            DepthOnStencilOn,
            DepthOnStencilOff,
            DepthOffStencilOn,
            DepthOffStencilOff,
            DepthOnWriteOffStencilOff,

            Max
        };
    public:
        DepthStencilState() = delete;
        DepthStencilState(ID3D11Device* device, const D3D11_DEPTH_STENCIL_DESC& desc);
        ~DepthStencilState() = default;

        DELETE_COPY_MOVE_CONSTRUCTORS(DepthStencilState)

        ID3D11DepthStencilState* GetState() const { return _state.Get(); }

        static void CreateBuildInDepthStencilStates(ID3D11Device* device);
        static ID3D11DepthStencilState* GetBuildInDepthStencilStatesAt(Type type);
    private:
        ComRef<ID3D11DepthStencilState> _state;

        static std::vector<ComRef<ID3D11DepthStencilState>> _buildInDepthStencilStates;
        static bool _created;
    };
};

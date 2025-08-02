#pragma once

#include "System/Core/IManager.h"

#include "Graphics/Renderer/BuildInShader.h"
#include "Graphics/Renderer/BuildInTexture.h"

namespace DentyEngine
{
    class GraphicsManager final : public IManager
    {
    public:
        ~GraphicsManager() override = default;

        DELETE_COPY_MOVE_CONSTRUCTORS(GraphicsManager)

        static GraphicsManager& GetInstance()
        {
            static GraphicsManager instance;
            return instance;
        }

        void Initialize() ;
        void OnGui() override;
        void Finalize() ;

        void GenerateBuildIn();

        ID3D11Device* GetDevice() const { return _device.Get(); }
        ID3D11DeviceContext* GetContext() const { return _context.Get(); }
        IDXGISwapChain1* GetSwapChain() const { return _swapChain.Get(); }
        ID3D11RenderTargetView* GetRenderTargetView() const { return _renderTargetView.Get(); }
        const D3D11_VIEWPORT& GetViewport() const { return _viewport; }
    private:
        GraphicsManager();

        void AcquireHighPerformanceAdapter(IDXGIFactory6* factory);
        void CreateDeviceAndContext(UINT createDeviceFlags);
        void CreateSwapChain(IDXGIFactory6* factory);
        void CreateViewport();
    private:
        ComRef<ID3D11Device> _device;
        ComRef<ID3D11DeviceContext> _context;
        ComRef<IDXGISwapChain1> _swapChain;
        ComRef<IDXGIAdapter3> _adapter;
        ComRef<ID3D11RenderTargetView> _renderTargetView;

        Scope<BuildInShader> _buildInShader;
        Scope<BuildInTexture> _buildInTexture;

        D3D11_VIEWPORT _viewport;
    };
};
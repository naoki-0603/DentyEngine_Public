#include "stdafx.h"

#include "GraphicsManager.h"

#include "System/Core/Window.h"

#include "Graphics/States/BlendState.h"
#include "Graphics/States/DepthStencilState.h"
#include "Graphics/States/SamplerState.h"
#include "Graphics/States/RasterizerState.h"

namespace DentyEngine
{
    #define DENTY_USE_DEPTH_STENCIL_VIEW_FOR_MAIN (0)

    GraphicsManager::GraphicsManager() :
		IManager(), _device(), _context(),
		_swapChain(), _adapter(), _renderTargetView(), _buildInShader(),
		_buildInTexture(), _viewport()
    {
    }

    void GraphicsManager::Initialize()
    {
        HRESULT hr = S_OK;
        UINT createDeviceFlags = 0;
        UINT createFactoryFlags = 0;
#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

        ComRef<IDXGIFactory6> factory;
        // Get the most high-performance GPU in this computer
        {
            hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(factory.GetAddressOf()));

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

            AcquireHighPerformanceAdapter(factory.Get());
        }

        CreateDeviceAndContext(createDeviceFlags);
        CreateSwapChain(factory.Get());
        CreateViewport();
    }

    void GraphicsManager::OnGui()
    {
        
    }

    void GraphicsManager::Finalize()
    {
        
    }

    void GraphicsManager::GenerateBuildIn()
    {
        BlendState::CreateBuildInBlendStates(_device.Get());
        DepthStencilState::CreateBuildInDepthStencilStates(_device.Get());
        SamplerState::CreateBuildInSamplerStates(_device.Get());
        RasterizerState::CreateBuildInRasterizerStates(_device.Get());

        _buildInShader = std::make_unique<BuildInShader>(_device.Get());
        _buildInTexture = std::make_unique<BuildInTexture>(_device.Get());
    }

    void GraphicsManager::AcquireHighPerformanceAdapter(IDXGIFactory6* factory)
    {
        HRESULT hr = S_OK;

        ComRef<IDXGIAdapter3> enumeratedAdapter;
        for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapterByGpuPreference(
            adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
            IID_PPV_ARGS(enumeratedAdapter.ReleaseAndGetAddressOf()));
            ++adapterIndex
            )
        {
            DXGI_ADAPTER_DESC1 adapterDesc;
            hr = enumeratedAdapter->GetDesc1(&adapterDesc);
            _ASSERT_EXPR(SUCCEEDED(hr), Utils::HRTrace(hr));

            if (adapterDesc.VendorId == 0x1002/*AMD*/ || adapterDesc.VendorId == 0x10DE/*NVIDIA*/)
            {
                // DENTY_ENGINE_INFO((std::wstring(adapterDesc.Description) + L" has been selected.").c_str());
                // DENTY_ENGINE_INFO(std::string("VendorId:" + std::to_string(adapterDesc.VendorId)).c_str());
                // DENTY_ENGINE_INFO(std::string("DeviceId:" + std::to_string(adapterDesc.DeviceId)).c_str());
                // DENTY_ENGINE_INFO(std::string("SubSysId:" + std::to_string(adapterDesc.SubSysId)).c_str());
                // DENTY_ENGINE_INFO(std::string("Revision:" + std::to_string(adapterDesc.Revision)).c_str());
                // DENTY_ENGINE_INFO(std::string("DedicatedVideoMemory:" + std::to_string(adapterDesc.DedicatedVideoMemory)).c_str());
                // DENTY_ENGINE_INFO(std::string("DedicatedSystemMemory:" + std::to_string(adapterDesc.DedicatedSystemMemory)).c_str());
                // DENTY_ENGINE_INFO(std::string("SharedSystemMemory:" + std::to_string(adapterDesc.SharedSystemMemory)).c_str());
                // DENTY_ENGINE_INFO(std::string("AdapterLuid.HighPart:" + std::to_string(adapterDesc.AdapterLuid.HighPart)).c_str());
                // DENTY_ENGINE_INFO(std::string("AdapterLuid.LowPart:" + std::to_string(adapterDesc.AdapterLuid.LowPart)).c_str());
                // DENTY_ENGINE_INFO(std::string("Flags:" + std::to_string(adapterDesc.Flags)).c_str());
                break;
            }
        }

        _adapter.Attach(enumeratedAdapter.Detach());
    }

    void GraphicsManager::CreateDeviceAndContext(UINT createDeviceFlags)
    {
        HRESULT hr = S_OK;

        // Set feature levels
        D3D_FEATURE_LEVEL featureLevels[]
        {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_11_1
        };

        // Create device
        hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createDeviceFlags,
            featureLevels, _countof(featureLevels),
            D3D11_SDK_VERSION,
            _device.ReleaseAndGetAddressOf(),
            nullptr,
            _context.ReleaseAndGetAddressOf()
        );

        DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
    }

    void GraphicsManager::CreateSwapChain(IDXGIFactory6* factory)
    {
        HRESULT hr = S_OK;

        const HWND hwnd = Window::GetInfo().GetHwnd();

        RECT rect = { };
        GetClientRect(hwnd, &rect);

        // Set swapchain  desc
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc1 = { };
        {
            swapChainDesc1.Width = static_cast<UINT>(rect.right);
            swapChainDesc1.Height = static_cast<UINT>(rect.bottom);
            swapChainDesc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            swapChainDesc1.Stereo = FALSE;
            swapChainDesc1.SampleDesc.Count = 1;
            swapChainDesc1.SampleDesc.Quality = 0;
            swapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc1.BufferCount = 2;
            swapChainDesc1.Scaling = DXGI_SCALING_STRETCH;
            swapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            swapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
            swapChainDesc1.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        }

        hr = factory->CreateSwapChainForHwnd(
            _device.Get(),
            hwnd,
            &swapChainDesc1,
            nullptr,
            nullptr, 
            _swapChain.ReleaseAndGetAddressOf()
        );

        DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

        hr = factory->MakeWindowAssociation(
            hwnd,
            DXGI_MWA_NO_ALT_ENTER
        );

        DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

        // Create color buffer
        {
            ComRef<ID3D11Texture2D> renderTargetBuffer;
            hr = _swapChain->GetBuffer(0, IID_PPV_ARGS(renderTargetBuffer.GetAddressOf()));
            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

            D3D11_TEXTURE2D_DESC texture2dDesc = {};
            renderTargetBuffer->GetDesc(&texture2dDesc);

            D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
            renderTargetViewDesc.Format = texture2dDesc.Format;
            renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            renderTargetViewDesc.Texture2D.MipSlice = 0;

            hr = _device->CreateRenderTargetView(
                renderTargetBuffer.Get(), &renderTargetViewDesc, _renderTargetView.ReleaseAndGetAddressOf()
            );
            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }

        // Create depth stencil view
        {
#if DENTY_USE_DEPTH_STENCIL_VIEW_FOR_MAIN
            // Create buffer
            ComRef<ID3D11Texture2D> depthStencilBuffer = { };
            D3D11_TEXTURE2D_DESC texture2dDesc = { };
            texture2dDesc.Width = static_cast<UINT>(windowSize.x);
            texture2dDesc.Height = static_cast<UINT>(windowSize.y);
            texture2dDesc.ArraySize = 1;
            texture2dDesc.MipLevels = 1;
            texture2dDesc.CPUAccessFlags = 0;
            texture2dDesc.SampleDesc.Count = 1;
            texture2dDesc.SampleDesc.Quality = 0;
            texture2dDesc.MiscFlags = 0;
            texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
            texture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            texture2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

            hr = _device->CreateTexture2D(&texture2dDesc, nullptr, depthStencilBuffer.ReleaseAndGetAddressOf());

            DENTY_ASSERT(SUCCEEDED(hr), HRTrace(hr));

            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = { };
            depthStencilViewDesc.Format = texture2dDesc.Format;
            depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilViewDesc.Texture2D.MipSlice = 0;

            hr = _device->CreateDepthStencilView(depthStencilBuffer.Get(),
                &depthStencilViewDesc, _depthStencilView.ReleaseAndGetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), HRTrace(hr));
#endif
        }
    }

    void GraphicsManager::CreateViewport()
    {
        const Vector2 windowSize = Window::GetInfo().GetSize<Vector2>();

        _viewport.TopLeftX = 0;
        _viewport.TopLeftY = 0;
        _viewport.Width = static_cast<FLOAT>(windowSize.x);
        _viewport.Height = static_cast<FLOAT>(windowSize.y);
        _viewport.MinDepth = 0.0f;
        _viewport.MaxDepth = 1.0f;
    }
};
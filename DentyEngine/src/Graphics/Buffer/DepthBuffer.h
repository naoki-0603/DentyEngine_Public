#pragma once

namespace DentyEngine
{
    class DepthBuffer final
    {
        friend class SceneBuffer;
        friend class FrameBuffer;
    public:
        DepthBuffer();
        ~DepthBuffer() = default;

        DELETE_COPY_MOVE_CONSTRUCTORS(DepthBuffer)

        void Create(ID3D11Device* device, const Vector2& size, const DXGI_FORMAT format, bool makeSrv = true);

        void OnResize(ID3D11Device* device, const Vector2& size);

        [[nodiscard]]
        const Vector2& GetSize() const { return _size; }

        [[nodiscard]]
        DXGI_FORMAT GetFormat() const { return _format;}

        [[nodiscard]]
        ID3D11ShaderResourceView* const* GetResource() const;

        [[nodiscard]]
        bool IsValid() const;
    private:
        // Use when only bind depth stencil view.
        ComRef<ID3D11RenderTargetView> _cachedRenderTargetView;

        ComRef<ID3D11DepthStencilView> _depthStencilView;
        ComRef<ID3D11DepthStencilView> _cachedDepthStencilView;
        ComRef<ID3D11ShaderResourceView> _shaderResourceView;

        Vector2 _size;
        DXGI_FORMAT _format;
    };
};
#pragma once

namespace DentyEngine
{
    class ColorBuffer final
    {
        friend class SceneBuffer;
        friend class FrameBuffer;
    public:
        ColorBuffer();
        ColorBuffer(const ColorBuffer&) = default;
        ~ColorBuffer()  = default;

        void Create(ID3D11Device* device,
            const Vector2& size,
            const DXGI_FORMAT format, bool makeSrv = true);

        void CopyTo(ID3D11Resource** resource) const;

        void OnResize(ID3D11Device* device, const Vector2& size);
        //
        // Getter
        //
        [[nodiscard]]
        const Vector2& GetSize() const { return _size; }

        [[nodiscard]]
        DXGI_FORMAT GetFormat() const { return _format; }

        [[nodiscard]]
        ComRef<ID3D11ShaderResourceView> GetResourceAsComPtr() const { return _shaderResourceView; }

        [[nodiscard]]
        ID3D11ShaderResourceView* const* GetResource() const;

        [[nodiscard]]
        bool IsValid() const;
    private:
        ComRef<ID3D11Texture2D> _buffer;
        ComRef<ID3D11RenderTargetView> _renderTargetView;
        ComRef<ID3D11RenderTargetView> _cachedRenderTargetView;
        ComRef<ID3D11ShaderResourceView> _shaderResourceView;

    	Vector2 _size;
        DXGI_FORMAT _format;
    };
};
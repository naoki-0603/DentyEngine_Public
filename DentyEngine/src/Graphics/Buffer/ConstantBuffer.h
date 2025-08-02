#pragma once

namespace DentyEngine
{
    class ConstantBuffer final
    {
    public:
        enum class ReservedBindSlot : uint32_t
        {
            Shadow = 7,
            CollisionVisualizerCamera = 8,
            PhotoCamera = 9,
            Camera = 10,
            Light = 11,
            Time = 12,
        };
    public:
        ConstantBuffer(ID3D11Device* device, size_t sizeOfType);
        ~ConstantBuffer() = default;

        ConstantBuffer& operator=(const ConstantBuffer& source);

    	void UpdateSubresource(ID3D11DeviceContext* context, const void* constants);

        void BindVertexShader(ID3D11DeviceContext* context, UINT startSlot);
        void BindPixelShader(ID3D11DeviceContext* context, UINT startSlot);
        void BindHullShader(ID3D11DeviceContext* context, UINT startSlot);
        void BindDomainShader(ID3D11DeviceContext* context, UINT startSlot);
        void BindGeometryShader(ID3D11DeviceContext* context, UINT startSlot);
        void BindComputeShader(ID3D11DeviceContext* context, UINT startSlot);

        void BindAll(ID3D11DeviceContext* context, UINT startSlot);

        [[nodiscard]]
        ID3D11Buffer* const* GetBufferAddressOf() const { return _buffer.GetAddressOf(); }
    private:
        ComRef<ID3D11Buffer> _buffer;
    };
};
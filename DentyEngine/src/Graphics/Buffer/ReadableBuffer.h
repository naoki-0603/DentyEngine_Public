#pragma once

namespace DentyEngine
{
	// Inherited class is able to call "ReadPixel" function.
	// "SceneBuffer" and "FrameBuffer" inherit this class.
	class ReadableBuffer
	{
	public:
		ReadableBuffer() = default;
		virtual ~ReadableBuffer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ReadableBuffer)

		// if failed, return -1.
		[[nodiscard]]
		virtual int32_t ReadPixel(
			ID3D11Device* device, ID3D11DeviceContext* context,
			ID3D11ShaderResourceView* bufferResource,
			DXGI_FORMAT readBufferFormat,
			const Vector2& readBufferSize, const Vector2& fetchPosition
		) const;
	};
}
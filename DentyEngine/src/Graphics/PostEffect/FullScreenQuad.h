#pragma once

namespace DentyEngine
{
	class Shader;

	class FullScreenQuad final
	{
	public:
		struct SrvInfo
		{
			SrvInfo() : shaderResourceViews(), startSlot(), viewsNum() {}

			ID3D11ShaderResourceView* const* shaderResourceViews;
			uint32_t startSlot;
			uint32_t viewsNum;
		};

		struct ConstantInfo
		{
			ConstantInfo() : constants(), startSlot() {}

			const void* constants;
			uint32_t startSlot;
		};
	public:
		FullScreenQuad();
		~FullScreenQuad() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(FullScreenQuad)

		//
		// Member functions.
		//
		void Blit(const Ref<Shader>& shader, const SrvInfo& srvInfo, const ConstantInfo& constantInfo);
	};
}

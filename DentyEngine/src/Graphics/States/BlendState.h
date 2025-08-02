#pragma once

namespace DentyEngine
{
	class BlendState final
	{
	public:
		enum class Type
		{
		    NoBlend,
			Alpha,
			Add,
			Subtract,
			Multiply,
			AlphaCoverage,

			Max
		};
	public:
		BlendState() = delete;
		BlendState(ID3D11Device* device, const D3D11_BLEND_DESC& desc);
		~BlendState() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(BlendState)

		ID3D11BlendState* GetState() const { return _state.Get(); }

		static void CreateBuildInBlendStates(ID3D11Device* device);
		static ID3D11BlendState* GetBuildInBlendStatesAt(Type type);
	private:
		ComRef<ID3D11BlendState> _state;

		static std::vector<ComRef<ID3D11BlendState>> _buildInBlendStates;
		static bool _created;
	};
};
#pragma once

namespace DentyEngine
{
	class InputLayout final
	{
	public:
		enum class Type : int32_t
		{
			None = -1,
			MeshPositionOnly = 0,
			DebugLine,
			CPUParticle,
			ExpandedImage,
			Sprite,
			MeshWithoutBone,
			MeshWithBone,

			Max
		};
	public:
		InputLayout() = default;
		~InputLayout() = default;

		static const std::array<D3D11_INPUT_ELEMENT_DESC, 1> MESH_POSITION_ONLY_INPUT_LAYOUT;
		static const std::array<D3D11_INPUT_ELEMENT_DESC, 2> DEBUG_LINE_INPUT_LAYOUT;
		static const std::array<D3D11_INPUT_ELEMENT_DESC, 2> PARTICLE_INPUT_LAYOUT;
		static const std::array<D3D11_INPUT_ELEMENT_DESC, 3> EXPANDED_IMAGE_INPUT_LAYOUT;
		static const std::array<D3D11_INPUT_ELEMENT_DESC, 3> SPRITE_INPUT_LAYOUT;
		static const std::array<D3D11_INPUT_ELEMENT_DESC, 5> MESH_INPUT_LAYOUT_WITHOUT_BONE;
		static const std::array<D3D11_INPUT_ELEMENT_DESC, 7> MESH_INPUT_LAYOUT_WITH_BONE;
	};
};
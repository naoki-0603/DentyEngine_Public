#pragma once

namespace DentyEngine
{
	namespace MeshResource
	{
		class SceneTree;
	}

	class SceneTreeSerializer final
	{
	public:
		SceneTreeSerializer() = default;
		~SceneTreeSerializer() noexcept = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(SceneTreeSerializer)

		[[nodiscard]]
		static bool SerializeAsBinary(const std::filesystem::path&, const Ref<const  MeshResource::SceneTree>&);

		[[nodiscard]]
		static bool DeserializeFromBinary(const std::filesystem::path&, Ref< MeshResource::SceneTree>&);
	};
}
#pragma once

namespace DentyEngine
{
	class Scene;

	class SceneSerializer final
	{
	public:
		SceneSerializer();
		~SceneSerializer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(SceneSerializer)

		[[nodiscard]]
		static bool SerializeAsBinary(const std::filesystem::path&, const Ref<const Scene>&);

		[[nodiscard]]
		static bool DeserializeFromBinary(const std::filesystem::path&, Ref<Scene>&);
	public:
		static const std::string SCENE_FILE_OUTPUT_PATH;
	};
}
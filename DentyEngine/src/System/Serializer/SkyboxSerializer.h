#pragma once

namespace DentyEngine
{
	class Skybox;

	class SkyboxSerializer final
	{
	public:
		SkyboxSerializer();
		~SkyboxSerializer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(SkyboxSerializer)

		[[nodiscard]]
		static bool SerializeAsBinary(const std::filesystem::path&, const Ref<const Skybox>&);

		[[nodiscard]]
		static bool DeserializeFromBinary(const std::filesystem::path&, Ref<Skybox>&);
	};
}
#pragma once

namespace DentyEngine
{
	class Shader;

	class ShaderSerializer final
	{
	public:
		ShaderSerializer();
		~ShaderSerializer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ShaderSerializer)

		[[nodiscard]]
		static bool SerializeAsJson(const std::filesystem::path&, const Ref<const Shader>&);

		[[nodiscard]]
		static bool DeserializeFromJson(const std::filesystem::path&, Ref<Shader>&);
	public:
		static const std::string SHADER_FILE_OUTPUT_PATH;
		static const std::string BUILD_IN_SHADER_OUTPUT_PATH;
	};
}

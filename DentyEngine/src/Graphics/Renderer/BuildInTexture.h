#pragma once

namespace DentyEngine
{
	class BuildInTexture final
	{
	public:
		BuildInTexture(ID3D11Device* device);
		~BuildInTexture() = default;

	private:
		void Create(ID3D11Device* device);
	private:
		// First -> name, Seconds -> paths
		using BuildInTextureData = std::unordered_map<std::string, std::filesystem::path>;
		static BuildInTextureData _buildInTextureData;
	};
}
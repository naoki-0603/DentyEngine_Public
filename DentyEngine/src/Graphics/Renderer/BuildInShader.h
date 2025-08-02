#pragma once

namespace DentyEngine
{
	class BuildInShader final
	{
	public:
		BuildInShader(ID3D11Device* device);
		~BuildInShader() = default;

	private:
		void Create(ID3D11Device* device);
		void Create3DShaders(ID3D11Device* device);
		void Create2DShaders(ID3D11Device* device);
		void CreatePostEffectShaders(ID3D11Device* device);

	private:
		// First -> name, Seconds -> paths
		using BuildInShaderData = std::unordered_map<std::string, std::vector<std::string>>;
		static BuildInShaderData _buildInShaderData;
	};
}
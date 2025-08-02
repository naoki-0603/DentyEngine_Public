#include "stdafx.h"

#include "BuildInTexture.h"

#include "Texture2D.h"
#include "CubeMap.h"

namespace DentyEngine
{
	BuildInTexture::BuildInTextureData BuildInTexture::_buildInTextureData = { };

	BuildInTexture::BuildInTexture(ID3D11Device* device)
	{
		// Editor
		{
			// ContentBrowser
			_buildInTextureData.emplace(std::make_pair("FolderFileExists", "Assets\\Textures\\Editor\\ContentBrowser\\Folder.png"));
			_buildInTextureData.emplace(std::make_pair("FolderEmpty", "Assets\\Textures\\Editor\\ContentBrowser\\FolderEmpty.png"));
			_buildInTextureData.emplace(std::make_pair("BackArrow", "Assets\\Textures\\Editor\\ContentBrowser\\Back.png"));
			_buildInTextureData.emplace(std::make_pair("Scene", "Assets\\Textures\\Editor\\Common\\Scene.png"));
			_buildInTextureData.emplace(std::make_pair("Mesh", "Assets\\Textures\\Editor\\ContentBrowser\\Mesh.png"));
			_buildInTextureData.emplace(std::make_pair("Material", "Assets\\Textures\\Editor\\ContentBrowser\\Material.png"));
			_buildInTextureData.emplace(std::make_pair("Prefab", "Assets\\Textures\\Editor\\ContentBrowser\\Prefab.png"));
			_buildInTextureData.emplace(std::make_pair("Shader", "Assets\\Textures\\Editor\\ContentBrowser\\Shader.png"));

			// ToolBar
			_buildInTextureData.emplace(std::make_pair("Play", "Assets\\Textures\\Editor\\Play.png"));
			_buildInTextureData.emplace(std::make_pair("Pause", "Assets\\Textures\\Editor\\Pause.png"));
			_buildInTextureData.emplace(std::make_pair("Step", "Assets\\Textures\\Editor\\Step.png"));
		}

		// Skybox (CubeMap)
		{
			_buildInTextureData.emplace(std::make_pair("Skybox", "Assets\\Textures\\Skymap\\PureSky\\PureSky.dds"));
		}

		Create(device);
	}

	void BuildInTexture::Create(ID3D11Device* device)
	{
		// Editor
		{
			const auto folderFileExistsTexture = std::make_shared<Texture2D>();
			folderFileExistsTexture->Load(device, _buildInTextureData.at("FolderFileExists").string());

			const auto folderEmptyTexture = std::make_shared<Texture2D>();
			folderEmptyTexture->Load(device, _buildInTextureData.at("FolderEmpty").string());

			const auto backArrowTexture = std::make_shared<Texture2D>();
			backArrowTexture->Load(device, _buildInTextureData.at("BackArrow").string());

			const auto sceneTexture = std::make_shared<Texture2D>();
			sceneTexture->Load(device, _buildInTextureData.at("Scene").string());

			const auto meshTexture = std::make_shared<Texture2D>();
			meshTexture->Load(device, _buildInTextureData.at("Mesh").string());

			const auto materialTexture = std::make_shared<Texture2D>();
			materialTexture->Load(device, _buildInTextureData.at("Material").string());

			const auto shaderTexture = std::make_shared<Texture2D>();
			shaderTexture->Load(device, _buildInTextureData.at("Shader").string());

			const auto prefabTexture = std::make_shared<Texture2D>();
			prefabTexture->Load(device, _buildInTextureData.at("Prefab").string());

			const auto playTexture = std::make_shared<Texture2D>();
			playTexture->Load(device, _buildInTextureData.at("Play").string());

			const auto pauseTexture = std::make_shared<Texture2D>();
			pauseTexture->Load(device, _buildInTextureData.at("Pause").string());

			const auto stepTexture = std::make_shared<Texture2D>();
			stepTexture->Load(device, _buildInTextureData.at("Step").string());
		}

		// Skybox
		{
			const auto skyboxCubeMap = std::make_shared<CubeMap>();
			skyboxCubeMap->Load(device, _buildInTextureData.at("Skybox").string());
		}
	}
}
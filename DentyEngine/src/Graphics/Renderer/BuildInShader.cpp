#include "stdafx.h"

#include "BuildInShader.h"

#include "InputLayout.h"
#include "Texture2D.h"

#include "Graphics/Skybox.h"
#include "Graphics/Mesh/StaticMesh.h"
#include "Graphics/Mesh/SkinnedMesh.h"

#include "Graphics/PostEffect/GaussianBlur.h"

#include "Graphics/Sprite/Sprite.h"
#include "Graphics/Sprite/Sprite3D.h"

#include "System/Physics/BulletDebugDrawer.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ShaderLibrary.h"

#include "System/Graphics/Renderer/CPUParticleSystem.h"

#include "System/Serializer/SerializeExtensions.h"


namespace DentyEngine
{
	BuildInShader::BuildInShaderData BuildInShader::_buildInShaderData = {};

	BuildInShader::BuildInShader(ID3D11Device* device)
	{
		// Static mesh shader
		{
			std::vector<std::string> staticMeshShaderPaths = {};
			staticMeshShaderPaths.resize(2u);

			staticMeshShaderPaths.at(0) = "DentyEngine/src/HLSL/3D/Mesh/StaticMeshVS.hlsl";
			staticMeshShaderPaths.at(1) = "DentyEngine/src/HLSL/3D/Mesh/StaticMeshPS.hlsl";

			_buildInShaderData.emplace("StaticMesh", std::move(staticMeshShaderPaths));
		}

		// Skinned mesh shader
		{
			std::vector<std::string> skinnedMeshShaderPaths = { };
			skinnedMeshShaderPaths.resize(2u);

			skinnedMeshShaderPaths.at(0) = "DentyEngine/src/HLSL/3D/Mesh/SkinnedMeshVS.hlsl";
			skinnedMeshShaderPaths.at(1) = "DentyEngine/src/HLSL/3D/Mesh/SkinnedMeshPS.hlsl";

			_buildInShaderData.emplace("SkinnedMesh", std::move(skinnedMeshShaderPaths));
		}

		// Static mesh shadow shader.
		{
			std::vector<std::string> staticMeshShadowPaths = { };
			staticMeshShadowPaths.resize(2u);

			staticMeshShadowPaths.at(0) = "DentyEngine/src/HLSL/3D/Mesh/Lighting/Shadow/StaticMeshShadowVS.hlsl";
			staticMeshShadowPaths.at(1) = "DentyEngine/src/HLSL/3D/Mesh/Lighting/Shadow/MeshShadowPS.hlsl";

			_buildInShaderData.emplace("StaticMeshShadow", std::move(staticMeshShadowPaths));
		}

		// Skinned mesh shadow shader.
		{
			std::vector<std::string> skinnedMeshShadowPaths = { };
			skinnedMeshShadowPaths.emplace_back("DentyEngine/src/HLSL/3D/Mesh/Lighting/Shadow/SkinnedMeshShadowVS.hlsl");
			skinnedMeshShadowPaths.emplace_back("DentyEngine/src/HLSL/3D/Mesh/Lighting/Shadow/MeshShadowPS.hlsl");

			_buildInShaderData.emplace("SkinnedMeshShadow", std::move(skinnedMeshShadowPaths));
		}

		// Collision visualizer mesh shader.
		{
			std::vector<std::string> collisionVisualizerMeshPaths = { };
			collisionVisualizerMeshPaths.emplace_back("DentyEngine/src/HLSL/3D/Mesh/CollisionVisualizerMeshVS.hlsl");
			collisionVisualizerMeshPaths.emplace_back("DentyEngine/src/HLSL/3D/Mesh/CollisionVisualizerMeshPS.hlsl");

			_buildInShaderData.emplace("CollisionVisualizerMesh", std::move(collisionVisualizerMeshPaths));
		}

		// Photo static mesh shader.
		{
			std::vector<std::string> photoMeshPaths = { };
			photoMeshPaths.emplace_back("DentyEngine/src/HLSL/3D/Mesh/PhotoStaticMeshVS.hlsl");
			photoMeshPaths.emplace_back("DentyEngine/src/HLSL/3D/Mesh/PhotoStaticMeshPS.hlsl");

			_buildInShaderData.emplace("PhotoStaticMesh", std::move(photoMeshPaths));
		}

		// Photo skinned mesh shader.
		{
			std::vector<std::string> photoMeshPaths = { };
			photoMeshPaths.emplace_back("DentyEngine/src/HLSL/3D/Mesh/PhotoSkinnedMeshVS.hlsl");
			photoMeshPaths.emplace_back("DentyEngine/src/HLSL/3D/Mesh/PhotoSkinnedMeshPS.hlsl");

			_buildInShaderData.emplace("PhotoSkinnedMesh", std::move(photoMeshPaths));
		}

		// Skybox shader
		{
			std::vector<std::string> skyboxHlslPaths = { };
			skyboxHlslPaths.emplace_back("DentyEngine/src/HLSL/3D/SkyboxVS.hlsl");
			skyboxHlslPaths.emplace_back("DentyEngine/src/HLSL/3D/SkyboxPS.hlsl");

			_buildInShaderData.emplace("Skybox", std::move(skyboxHlslPaths));
		}

		//// Photo mesh shader
		//{
		//	std::vector<std::string> postEffectMeshPaths = { };
		//	postEffectMeshPaths.emplace_back("DentyEngine/HLSL/3D/Mesh/PostEffectMeshVS.hlsl");
		//	postEffectMeshPaths.emplace_back("DentyEngine/HLSL/3D/Mesh/PostEffectMeshPS.hlsl");

		//	_buildInShaderData.emplace("PostEffectMesh", std::move(postEffectMeshPaths));
		//}

		// Sprite shader.
		{
			std::vector<std::string> spriteHlslPaths = { };
			spriteHlslPaths.emplace_back("DentyEngine/src/HLSL/2D/SpriteVS.hlsl");
			spriteHlslPaths.emplace_back("DentyEngine/src/HLSL/2D/SpritePS.hlsl");

			_buildInShaderData.emplace("Sprite", std::move(spriteHlslPaths));
		}

		// Sprite3D shader.
		{
			std::vector<std::string> sprite3DHlslPaths = { };
			sprite3DHlslPaths.emplace_back("DentyEngine/src/HLSL/2D/Sprite3DVS.hlsl");
			sprite3DHlslPaths.emplace_back("DentyEngine/src/HLSL/2D/Sprite3DPS.hlsl");

			_buildInShaderData.emplace("Sprite3D", std::move(sprite3DHlslPaths));
		}

		// Grid shader
		{
			std::vector<std::string> gridHlslPaths = { };
			gridHlslPaths.emplace_back("DentyEngine/src/HLSL/2D/GridVS.hlsl");
			gridHlslPaths.emplace_back("DentyEngine/src/HLSL/2D/GridPS.hlsl");

			_buildInShaderData.emplace("Grid", std::move(gridHlslPaths));
		}

		// Line shader
		{
			std::vector<std::string> lineHlslPaths = { };
			lineHlslPaths.emplace_back("DentyEngine/src/HLSL/2D/LineVS.hlsl");
			lineHlslPaths.emplace_back("DentyEngine/src/HLSL/2D/LinePS.hlsl");

			_buildInShaderData.emplace("Line", std::move(lineHlslPaths));
		}

		// Expanded image shader.
		{
			std::vector<std::string> expandedImagePaths = {};
			expandedImagePaths.emplace_back("DentyEngine/src/HLSL/2D/ExpandedImageVS.hlsl");
			expandedImagePaths.emplace_back("DentyEngine/src/HLSL/2D/ExpandedImagePS.hlsl");

			_buildInShaderData.emplace("ExpandedImage", std::move(expandedImagePaths));
		}

		// Bullet physics debug drawer
		{
			std::vector<std::string> debugDrawerHlslPaths = { };
			debugDrawerHlslPaths.emplace_back("DentyEngine/src/HLSL/2D/BulletDebugDrawerVS.hlsl");
			debugDrawerHlslPaths.emplace_back("DentyEngine/src/HLSL/2D/BulletDebugDrawerPS.hlsl");

			_buildInShaderData.emplace("DebugDrawer", std::move(debugDrawerHlslPaths));
		}

		// CPU Particle shader
		{
			std::vector<std::string> cpuParticleHLSLPaths = { };
			cpuParticleHLSLPaths.emplace_back("DentyEngine/src/HLSL/2D/CPUParticleVS.hlsl");
			cpuParticleHLSLPaths.emplace_back("DentyEngine/src/HLSL/2D/CPUParticlePS.hlsl");

			_buildInShaderData.emplace("CPUParticle", std::move(cpuParticleHLSLPaths));
		}

		// PostEffect shaders.
		{
			// Full screen quad.
			{
				std::vector<std::string> fullScreenQuadHlslPaths = { };
				fullScreenQuadHlslPaths.emplace_back("DentyEngine/src/HLSL/PostEffect/FullScreenQuadVS.hlsl");
				fullScreenQuadHlslPaths.emplace_back("DentyEngine/src/HLSL/PostEffect/FullScreenQuadPS.hlsl");

				_buildInShaderData.emplace("FullScreenQuad", std::move(fullScreenQuadHlslPaths));
			}

			// Gaussian blur horizontal.
			{
				std::vector<std::string> gaussianBlurHorizontalHlslPaths = { };
				gaussianBlurHorizontalHlslPaths.emplace_back("DentyEngine/src/HLSL/PostEffect/FullScreenQuadVS.hlsl");
				gaussianBlurHorizontalHlslPaths.emplace_back("DentyEngine/src/HLSL/PostEffect/GaussianBlur/GaussianBlurHorizontalPS.hlsl");

				_buildInShaderData.emplace("GaussianBlurHorizontal", std::move(gaussianBlurHorizontalHlslPaths));
			}

			// Gaussian blur vertical.
			{
				std::vector<std::string> gaussianBlurVerticalHlslPaths = { };
				gaussianBlurVerticalHlslPaths.emplace_back("DentyEngine/src/HLSL/PostEffect/FullScreenQuadVS.hlsl");
				gaussianBlurVerticalHlslPaths.emplace_back("DentyEngine/src/HLSL/PostEffect/GaussianBlur/GaussianBlurVerticalPS.hlsl");

				_buildInShaderData.emplace("GaussianBlurVertical", std::move(gaussianBlurVerticalHlslPaths));
			}

			// Bloom (luminance extraction)
			{
				std::vector<std::string> bloomHlslPaths = { };
				bloomHlslPaths.emplace_back("DentyEngine/src/HLSL/PostEffect/FullScreenQuadVS.hlsl");
				bloomHlslPaths.emplace_back("DentyEngine/src/HLSL/PostEffect/Bloom/LuminanceExtractionPS.hlsl");

				_buildInShaderData.emplace("Bloom", std::move(bloomHlslPaths));
			}
		}

		Create(device);
	}

	void BuildInShader::Create(ID3D11Device* device)
	{
		Create3DShaders(device);
		Create2DShaders(device);

		CreatePostEffectShaders(device);
	}

	void BuildInShader::Create3DShaders(ID3D11Device* device)
	{
		ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

		// Static mesh shader
		{
			// If already exists, doesn't create.
			std::filesystem::path staticMeshShaderPath = ShaderLibrary::STATIC_MESH_SHADER_PATH;
			if (Utils::IsNeedSerialize(staticMeshShaderPath, SHADER_FILE_EXTENSION))
			{
				const auto staticMeshShader = std::make_shared<Shader>();
				staticMeshShader->CreateBuildIn(device, "StaticMesh", sizeof(StaticMeshConstants));
				staticMeshShader->CreateVertexShader(
					device, _buildInShaderData.at("StaticMesh").at(0),
					InputLayout::MESH_INPUT_LAYOUT_WITHOUT_BONE.data(),
					InputLayout::MESH_INPUT_LAYOUT_WITHOUT_BONE.size()
				);
				staticMeshShader->_inputLayoutType = InputLayout::Type::MeshWithoutBone;

				staticMeshShader->CreatePixelShader(
					device, _buildInShaderData.at("StaticMesh").at(1)
				);

				staticMeshShader->SetFilePathToSerialized(staticMeshShaderPath.string());

				// Update serialized file.
				staticMeshShader->Save();

				shaderLibrary->Add(staticMeshShader);
			}
		}

		// Skinned mesh shader
		{
			// If already exists, doesn't create.
			std::filesystem::path skinnedMeshShaderPath = ShaderLibrary::SKINNED_MESH_SHADER_PATH;
			if (Utils::IsNeedSerialize(skinnedMeshShaderPath, SHADER_FILE_EXTENSION))
			{
				const auto skinnedMeshShader = std::make_shared<Shader>();
				skinnedMeshShader->CreateBuildIn(device, "SkinnedMesh", sizeof(SkinnedMeshConstants));
				skinnedMeshShader->CreateVertexShader(
					device, _buildInShaderData.at("SkinnedMesh").at(0),
					InputLayout::MESH_INPUT_LAYOUT_WITH_BONE.data(),
					InputLayout::MESH_INPUT_LAYOUT_WITH_BONE.size()
				);
				skinnedMeshShader->_inputLayoutType = InputLayout::Type::MeshWithBone;

				skinnedMeshShader->CreatePixelShader(
					device, _buildInShaderData.at("SkinnedMesh").at(1)
				);

				skinnedMeshShader->SetFilePathToSerialized(skinnedMeshShaderPath.string());

				// Update serialized file.
				skinnedMeshShader->Save();

				shaderLibrary->Add(skinnedMeshShader);
			}
		}

		// Static mesh shadow shader
		{
			const std::filesystem::path staticMeshShadowShaderPath = ShaderLibrary::STATIC_MESH_SHADOW_SHADER_PATH;
			if (Utils::IsNeedSerialize(staticMeshShadowShaderPath, SHADER_FILE_EXTENSION))
			{
				const auto staticMeshShadowShader = std::make_shared<Shader>();
				staticMeshShadowShader->CreateBuildIn(device, "StaticMeshShadow", sizeof(StaticMeshShadowConstants));
				staticMeshShadowShader->CreateVertexShader(
					device, _buildInShaderData.at("StaticMeshShadow").at(0),
					InputLayout::MESH_INPUT_LAYOUT_WITHOUT_BONE.data(),
					InputLayout::MESH_INPUT_LAYOUT_WITHOUT_BONE.size()
				);
				staticMeshShadowShader->_inputLayoutType = InputLayout::Type::MeshWithoutBone;

				staticMeshShadowShader->CreatePixelShader(
					device, _buildInShaderData.at("StaticMeshShadow").at(1)
				);

				staticMeshShadowShader->SetFilePathToSerialized(staticMeshShadowShaderPath.string());
				staticMeshShadowShader->Save();

				shaderLibrary->Add(staticMeshShadowShader);
			}
		}

		// Skinned mesh shadow shader
		{
			const std::filesystem::path skinnedMeshShadowShaderPath = ShaderLibrary::SKINNED_MESH_SHADOW_SHADER_PATH;
			if (Utils::IsNeedSerialize(skinnedMeshShadowShaderPath, SHADER_FILE_EXTENSION))
			{
				const auto skinnedMeshShadowShader = std::make_shared<Shader>();
				skinnedMeshShadowShader->CreateBuildIn(device, "SkinnedMeshShadow", sizeof(SkinnedMeshShadowConstants));
				skinnedMeshShadowShader->CreateVertexShader(
					device, _buildInShaderData.at("SkinnedMeshShadow").at(0),
					InputLayout::MESH_INPUT_LAYOUT_WITH_BONE.data(),
					InputLayout::MESH_INPUT_LAYOUT_WITH_BONE.size()
				);
				skinnedMeshShadowShader->_inputLayoutType = InputLayout::Type::MeshWithBone;

				skinnedMeshShadowShader->CreatePixelShader(
					device, _buildInShaderData.at("SkinnedMeshShadow").at(1)
				);

				skinnedMeshShadowShader->SetFilePathToSerialized(skinnedMeshShadowShaderPath.string());
				skinnedMeshShadowShader->Save();

				shaderLibrary->Add(skinnedMeshShadowShader);
			}
		}

		// Collision visualizer mesh shader
		{
			const std::filesystem::path collisionVisualizerMeshShaderPath = ShaderLibrary::COLLISION_VISUALIZER_MESH_SHADER_PATH;
			if (Utils::IsNeedSerialize(collisionVisualizerMeshShaderPath, SHADER_FILE_EXTENSION))
			{
				const auto collisionVisualizerMeshShader = std::make_shared<Shader>();
				collisionVisualizerMeshShader->CreateBuildIn(device, "CollisionVisualizerMesh", sizeof(StaticMeshConstants));
				collisionVisualizerMeshShader->CreateVertexShader(
					device, _buildInShaderData.at("CollisionVisualizerMesh").at(0),
					InputLayout::MESH_INPUT_LAYOUT_WITHOUT_BONE.data(),
					InputLayout::MESH_INPUT_LAYOUT_WITHOUT_BONE.size()
				);
				collisionVisualizerMeshShader->_inputLayoutType = InputLayout::Type::MeshWithoutBone;

				collisionVisualizerMeshShader->CreatePixelShader(
					device, _buildInShaderData.at("CollisionVisualizerMesh").at(1)
				);

				collisionVisualizerMeshShader->SetFilePathToSerialized(collisionVisualizerMeshShaderPath.string());
				collisionVisualizerMeshShader->Save();

				shaderLibrary->Add(collisionVisualizerMeshShader);
			}
		}

		// Photo static mesh shader.
		{
		    const auto photoShaderPath = ShaderLibrary::PHOTO_STATIC_MESH_SHADER_PATH;
			if (Utils::IsNeedSerialize(photoShaderPath, SHADER_FILE_EXTENSION))
			{
			    const auto photoStaticMeshShader = std::make_shared<Shader>();
				photoStaticMeshShader->CreateBuildIn(device, "PhotoStaticMesh", sizeof(StaticMeshConstants));
				photoStaticMeshShader->CreateVertexShader(
				    device, _buildInShaderData.at("PhotoStaticMesh").at(0),
					InputLayout::MESH_INPUT_LAYOUT_WITHOUT_BONE.data(),
					InputLayout::MESH_INPUT_LAYOUT_WITHOUT_BONE.size()
				);
				photoStaticMeshShader->_inputLayoutType = InputLayout::Type::MeshWithoutBone;

				photoStaticMeshShader->CreatePixelShader(
				    device, _buildInShaderData.at("PhotoStaticMesh").at(1)
				);

				photoStaticMeshShader->SetFilePathToSerialized(photoShaderPath);
				photoStaticMeshShader->Save();

				shaderLibrary->Add(photoStaticMeshShader);
			}
		}

		// Photo skinned mesh shader.
		{
		    const auto photoShaderPath = ShaderLibrary::PHOTO_SKINNED_MESH_SHADER_PATH;
			if (Utils::IsNeedSerialize(photoShaderPath, SHADER_FILE_EXTENSION))
			{
			    const auto photoSkinnedMeshShader = std::make_shared<Shader>();
				photoSkinnedMeshShader->CreateBuildIn(device, "PhotoSkinnedMesh", sizeof(SkinnedMeshConstants));
				photoSkinnedMeshShader->CreateVertexShader(
				    device, _buildInShaderData.at("PhotoSkinnedMesh").at(0),
					InputLayout::MESH_INPUT_LAYOUT_WITH_BONE.data(),
					InputLayout::MESH_INPUT_LAYOUT_WITH_BONE.size()
				);
				photoSkinnedMeshShader->_inputLayoutType = InputLayout::Type::MeshWithBone;

				photoSkinnedMeshShader->CreatePixelShader(
				    device, _buildInShaderData.at("PhotoSkinnedMesh").at(1)
				);

				photoSkinnedMeshShader->SetFilePathToSerialized(photoShaderPath);
				photoSkinnedMeshShader->Save();

				shaderLibrary->Add(photoSkinnedMeshShader);
			}
		}

		// Skybox shader
		{
			// If already exists, doesn't create.
			std::filesystem::path skyboxShaderPath = ShaderLibrary::SKYBOX_PATH;

			if (Utils::IsNeedSerialize(skyboxShaderPath, SHADER_FILE_EXTENSION))
			{
				const auto skyboxShader = std::make_shared<Shader>();
				skyboxShader->CreateBuildIn(device, "Skybox", sizeof(Skybox::Constants));
				skyboxShader->CreateVertexShader(device,
					_buildInShaderData.at("Skybox").at(0),
					InputLayout::MESH_POSITION_ONLY_INPUT_LAYOUT.data(),
					InputLayout::MESH_POSITION_ONLY_INPUT_LAYOUT.size()
				);
				skyboxShader->_inputLayoutType = InputLayout::Type::MeshPositionOnly;

				skyboxShader->CreatePixelShader(device,
					_buildInShaderData.at("Skybox").at(1)
				);

				skyboxShader->SetFilePathToSerialized(skyboxShaderPath.string());

				skyboxShader->Save();

				shaderLibrary->Add(skyboxShader);
			}
		}
	}

	void BuildInShader::Create2DShaders(ID3D11Device* device)
	{
		ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

		// Sprite shader
		{
			// If already exists, doesn't create.
            const std::filesystem::path spriteShaderPath = ShaderLibrary::SPRITE_SHADER_PATH;
			if (Utils::IsNeedSerialize(spriteShaderPath, SHADER_FILE_EXTENSION))
			{
				const auto spriteShader = std::make_shared<Shader>();
				spriteShader->CreateBuildIn(device, "Sprite", sizeof(SpriteConstants));
				spriteShader->CreateVertexShader(device,
					_buildInShaderData.at("Sprite").at(0),
					InputLayout::SPRITE_INPUT_LAYOUT.data(),
					InputLayout::SPRITE_INPUT_LAYOUT.size()
				);
				spriteShader->_inputLayoutType = InputLayout::Type::Sprite;

				spriteShader->CreatePixelShader(device,
					_buildInShaderData.at("Sprite").at(1)
				);

				spriteShader->SetFilePathToSerialized(spriteShaderPath.string());

				spriteShader->Save();

				shaderLibrary->Add(spriteShader);
			}
		}

		// Sprite3D shader
		{
			// If already exists, doesn't create.
			const std::filesystem::path sprite3DShaderPath = ShaderLibrary::SPRITE_3D_SHADER_PATH;
			if (Utils::IsNeedSerialize(sprite3DShaderPath, SHADER_FILE_EXTENSION))
			{
				const auto sprite3DShader = std::make_shared<Shader>();
				sprite3DShader->CreateBuildIn(device, "Sprite3D", sizeof(Sprite3DConstants));
				sprite3DShader->CreateVertexShader(device,
					_buildInShaderData.at("Sprite3D").at(0),
					InputLayout::SPRITE_INPUT_LAYOUT.data(),
					InputLayout::SPRITE_INPUT_LAYOUT.size()
				);
				sprite3DShader->_inputLayoutType = InputLayout::Type::Sprite;

				sprite3DShader->CreatePixelShader(device,
					_buildInShaderData.at("Sprite3D").at(1)
				);

				sprite3DShader->SetFilePathToSerialized(sprite3DShaderPath.string());

				sprite3DShader->Save();

				shaderLibrary->Add(sprite3DShader);
			}
		}

		// Grid shader
		{
			// If already exists, doesn't create.
			std::filesystem::path gridShaderPath = ShaderLibrary::GRID_SHADER_PATH;
			if (Utils::IsNeedSerialize(gridShaderPath, SHADER_FILE_EXTENSION))
			{
				const auto gridShader = std::make_shared<Shader>();
				gridShader->CreateBuildIn(device, "Grid", 0);
				gridShader->CreateVertexShader(device,
					_buildInShaderData.at("Grid").at(0),
					InputLayout::MESH_POSITION_ONLY_INPUT_LAYOUT.data(),
					InputLayout::MESH_POSITION_ONLY_INPUT_LAYOUT.size()
				);
				gridShader->_inputLayoutType = InputLayout::Type::MeshPositionOnly;

				gridShader->CreatePixelShader(device,
					_buildInShaderData.at("Grid").at(1)
				);

				gridShader->SetFilePathToSerialized(gridShaderPath.string());

				gridShader->Save();

				shaderLibrary->Add(gridShader);
			}
		}

		// line shader
		{
			// If already exists, doesn't create.
			std::filesystem::path lineShaderPath = ShaderLibrary::LINE_SHADER_PATH;
			if (Utils::IsNeedSerialize(lineShaderPath, SHADER_FILE_EXTENSION))
			{
				const auto lineShader = std::make_shared<Shader>();
				lineShader->CreateBuildIn(device, "Line", 0);
				lineShader->CreateVertexShader(device,
					_buildInShaderData.at("Line").at(0),
					InputLayout::DEBUG_LINE_INPUT_LAYOUT.data(),
					InputLayout::DEBUG_LINE_INPUT_LAYOUT.size()
				);
				lineShader->_inputLayoutType = InputLayout::Type::DebugLine;

				lineShader->CreatePixelShader(device,
					_buildInShaderData.at("Line").at(1)
				);

				lineShader->SetFilePathToSerialized(lineShaderPath.string());

				lineShader->Save();

				shaderLibrary->Add(lineShader);
			}
		}

		// expanded image shader
		{
			// If already exists, doesn't create.
			std::filesystem::path expandedImageShaderPath = ShaderLibrary::EXPANDED_IMAGE_SHADER_PATH;
			if (Utils::IsNeedSerialize(expandedImageShaderPath, SHADER_FILE_EXTENSION))
			{
				const auto expandedImageShader = std::make_shared<Shader>();
				expandedImageShader->CreateBuildIn(device, "ExpandedImage", sizeof(ExpandedImageConstants));
				expandedImageShader->CreateVertexShader(device,
					_buildInShaderData.at("ExpandedImage").at(0),
					InputLayout::EXPANDED_IMAGE_INPUT_LAYOUT.data(),
					InputLayout::EXPANDED_IMAGE_INPUT_LAYOUT.size()
				);
				expandedImageShader->_inputLayoutType = InputLayout::Type::ExpandedImage;

				expandedImageShader->CreatePixelShader(device,
					_buildInShaderData.at("ExpandedImage").at(1)
				);

				expandedImageShader->SetFilePathToSerialized(expandedImageShaderPath.string());

				expandedImageShader->Save();

				shaderLibrary->Add(expandedImageShader);
			}
		}

		// Bullet physics debug drawer shader.
		{
			// If already exists, doesn't create.
			std::filesystem::path debugDrawerShaderPath = ShaderLibrary::DEBUG_DRAWER_PATH;
			if (Utils::IsNeedSerialize(debugDrawerShaderPath, SHADER_FILE_EXTENSION))
			{
				const auto debugDrawerShader = std::make_shared<Shader>();
				debugDrawerShader->CreateBuildIn(device, "DebugDrawer", sizeof(BulletDebugDrawerLineDataConstants));
				debugDrawerShader->CreateVertexShader(device,
					_buildInShaderData.at("DebugDrawer").at(0),
					InputLayout::DEBUG_LINE_INPUT_LAYOUT.data(),
					InputLayout::DEBUG_LINE_INPUT_LAYOUT.size()
				);
				debugDrawerShader->_inputLayoutType = InputLayout::Type::DebugLine;

				debugDrawerShader->CreatePixelShader(
					device, _buildInShaderData.at("DebugDrawer").at(1)
				);

				debugDrawerShader->SetFilePathToSerialized(debugDrawerShaderPath.string());

				debugDrawerShader->Save();

				shaderLibrary->Add(debugDrawerShader);
			}
		}

		// CPU Particle
		{
			const std::filesystem::path cpuParticleShaderPath = ShaderLibrary::CPU_PARTICLE_SHADER_PATH;
			if (Utils::IsNeedSerialize(cpuParticleShaderPath, SHADER_FILE_EXTENSION))
			{
				const auto cpuParticleShader = std::make_shared<Shader>();
				cpuParticleShader->CreateBuildIn(device, "CPUParticle", sizeof(CPUParticleConstants));
				cpuParticleShader->CreateVertexShader(
					device,
					_buildInShaderData.at("CPUParticle").at(0),
					InputLayout::PARTICLE_INPUT_LAYOUT.data(),
					InputLayout::PARTICLE_INPUT_LAYOUT.size()
				);
				cpuParticleShader->_inputLayoutType = InputLayout::Type::CPUParticle;

				cpuParticleShader->CreatePixelShader(
					device, _buildInShaderData.at("CPUParticle").at(1)
				);
				cpuParticleShader->SetFilePathToSerialized(cpuParticleShaderPath.string());

				cpuParticleShader->Save();

				shaderLibrary->Add(cpuParticleShader);
			}
		}
	}

	void BuildInShader::CreatePostEffectShaders(ID3D11Device* device)
	{
		ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

		// Full screen quad.
		{
			// If already exists, doesn't create.
			std::filesystem::path fullScreenQuadPath = ShaderLibrary::FULL_SCREEN_QUAD_PATH;
			if (Utils::IsNeedSerialize(fullScreenQuadPath, SHADER_FILE_EXTENSION))
			{
				const auto fullScreenQuadShader = std::make_shared<Shader>();
				fullScreenQuadShader->CreateBuildIn(device, "FullScreenQuad", 0);
				fullScreenQuadShader->CreateVertexShader(device,
					_buildInShaderData.at("FullScreenQuad").at(0),
					nullptr, 0
				);
				fullScreenQuadShader->CreatePixelShader(device,
					_buildInShaderData.at("FullScreenQuad").at(1)
				);

				fullScreenQuadShader->SetFilePathToSerialized(fullScreenQuadPath.string());

				fullScreenQuadShader->Save();

				shaderLibrary->Add(fullScreenQuadShader);
			}
		}

		// Gaussian blur horizontal shader
		{
			// If already exists, doesn't create.
			std::filesystem::path gaussianBlurHorizontalPath = ShaderLibrary::GAUSSIAN_BLUR_HORIZONTAL_PATH;
			if (Utils::IsNeedSerialize(gaussianBlurHorizontalPath, SHADER_FILE_EXTENSION))
			{
				const auto gaussianHorizontalShader = std::make_shared<Shader>();
				gaussianHorizontalShader->CreateBuildIn(device, "GaussianBlurHorizontal", sizeof(GaussianBlur::Constants));
				gaussianHorizontalShader->CreateVertexShader(device,
					_buildInShaderData.at("GaussianBlurHorizontal").at(0),
					nullptr, 0
				);
				gaussianHorizontalShader->CreatePixelShader(device,
					_buildInShaderData.at("GaussianBlurHorizontal").at(1)
				);

				gaussianHorizontalShader->SetFilePathToSerialized(gaussianBlurHorizontalPath.string());

				gaussianHorizontalShader->Save();

				shaderLibrary->Add(gaussianHorizontalShader);
			}
		}

		// Gaussian blur vertical shader
		{
			// If already exists, doesn't create.
			std::filesystem::path gaussianBlurVerticalPath = ShaderLibrary::GAUSSIAN_BLUR_VERTICAL_PATH;
			if (Utils::IsNeedSerialize(gaussianBlurVerticalPath, SHADER_FILE_EXTENSION))
			{
				const auto gaussianBlurVerticalShader = std::make_shared<Shader>();
				gaussianBlurVerticalShader->CreateBuildIn(device, "GaussianBlurVertical", sizeof(GaussianBlur::Constants));
				gaussianBlurVerticalShader->CreateVertexShader(device,
					_buildInShaderData.at("GaussianBlurVertical").at(0),
					nullptr, 0
				);
				gaussianBlurVerticalShader->CreatePixelShader(device,
					_buildInShaderData.at("GaussianBlurVertical").at(1)
				);

				gaussianBlurVerticalShader->SetFilePathToSerialized(gaussianBlurVerticalPath.string());

				gaussianBlurVerticalShader->Save();

				shaderLibrary->Add(gaussianBlurVerticalShader);
			}
		}

		// Bloom (luminance extraction)
		{
			// If already exists, doesn't create.
			std::filesystem::path bloomPath = ShaderLibrary::BLOOM_PATH;
			if (Utils::IsNeedSerialize(bloomPath, SHADER_FILE_EXTENSION))
			{
				const auto bloomShader = std::make_shared<Shader>();
				bloomShader->CreateBuildIn(device, "Bloom", 0);
				bloomShader->CreateVertexShader(device,
					_buildInShaderData.at("Bloom").at(0),
					nullptr, 0
				);
				bloomShader->CreatePixelShader(device,
					_buildInShaderData.at("Bloom").at(1)
				);

				bloomShader->SetFilePathToSerialized(bloomPath.string());

				bloomShader->Save();

				shaderLibrary->Add(bloomShader);
			}
		}
	}
}

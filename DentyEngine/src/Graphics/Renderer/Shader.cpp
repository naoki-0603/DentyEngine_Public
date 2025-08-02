#include "stdafx.h"

#include "Shader.h"

#include "InputLayout.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ShaderLibrary.h"

#include "Utilities/StringUtilities.h"

#include "Graphics/Buffer/ConstantBuffer.h"

#include "Graphics/Renderer/Texture2D.h"

#include "System/Graphics/GraphicsManager.h"
#include "System/Resource/Library/TextureLibrary.h"
#include "System/Serializer/SerializeExtensions.h"
#include "System/Serializer/ShaderSerializer.h"

namespace DentyEngine
{
	const char* const Shader::DEFAULT_VS_TARGET = "vs_5_0";
	const char* const Shader::DEFAULT_PS_TARGET = "ps_5_0";
	const char* const Shader::DEFAULT_CS_TARGET = "cs_5_0";
	const char* const Shader::DEFAULT_HS_TARGET = "hs_5_0";
	const char* const Shader::DEFAULT_DS_TARGET = "ds_5_0";
	const char* const Shader::DEFAULT_GS_TARGET = "gs_5_0";

    Ref<Texture2D> Shader::_shaderAssetTexture = nullptr;

	Shader::Shader() :
		Asset(), Object(), _inputLayoutType(InputLayout::Type::None)
	{
	}

	Shader& Shader::operator=(const Shader& source)
	{
		Asset::operator=(source);
		Object::operator=(source);

		_inputLayoutType = source._inputLayoutType;
		_id = source._id;
		_pathHolder = source._pathHolder;
		_constantBufferSize = source._constantBufferSize;
		_vertexShader = source._vertexShader;
		_hullShader = source._hullShader;
		_domainShader = source._domainShader;
		_geometryShader = source._geometryShader;
		_pixelShader = source._pixelShader;

		source._inputLayout.CopyTo(_inputLayout.ReleaseAndGetAddressOf());

		_inputLayoutType = source._inputLayoutType;
		_computeShaders = source._computeShaders;
		
		return *this;
	}

	void Shader::Create(ID3D11Device* device, const std::string& name, size_t constantBufferSize)
	{
		_name = name;
		_constantBufferSize = constantBufferSize;

		if (constantBufferSize > 0)
		{
			constantBuffer = std::make_shared<ConstantBuffer>(device, constantBufferSize);
		}

		// Save.
		{
			String filePath = ShaderSerializer::SHADER_FILE_OUTPUT_PATH + name;
			filePath.Append(SHADER_FILE_EXTENSION);

			_filePathToSerialized = String::ToPath(filePath);

			Save();
		}

		// Add to library.
		{
		    ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();
		    shaderLibrary->Add(shared_from_this());
		}
	}

	void Shader::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void Shader::OnSerialized() const
	{
	}

	void Shader::OnDeserialized()
	{
		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

		// Restore shaders.
		{
			// Vertex shader.
			if (_pathHolder.vertexShaderFilePath != "")
			{
				using InputLayoutType = InputLayout::Type;

				switch (_inputLayoutType)
				{
				case InputLayoutType::None:
					{
						CreateVertexShader(device, _pathHolder.vertexShaderFilePath, nullptr, 0);
					}
					break;
				case InputLayoutType::MeshPositionOnly:
				{
					const D3D11_INPUT_ELEMENT_DESC* elementDesc = InputLayout::MESH_POSITION_ONLY_INPUT_LAYOUT.data();
					constexpr size_t elementNum = InputLayout::MESH_POSITION_ONLY_INPUT_LAYOUT.size();

					CreateVertexShader(device, _pathHolder.vertexShaderFilePath, elementDesc, elementNum);
				}
					break;
				case InputLayoutType::DebugLine:
				{
					const D3D11_INPUT_ELEMENT_DESC* elementDesc = InputLayout::DEBUG_LINE_INPUT_LAYOUT.data();
					constexpr size_t elementNum = InputLayout::DEBUG_LINE_INPUT_LAYOUT.size();

					CreateVertexShader(device, _pathHolder.vertexShaderFilePath, elementDesc, elementNum);
				}
					break;
				case InputLayoutType::CPUParticle:
				{
					const D3D11_INPUT_ELEMENT_DESC* elementDesc = InputLayout::PARTICLE_INPUT_LAYOUT.data();
					constexpr size_t elementNum = InputLayout::PARTICLE_INPUT_LAYOUT.size();

					CreateVertexShader(device, _pathHolder.vertexShaderFilePath, elementDesc, elementNum);
				}
					break;
				case InputLayoutType::ExpandedImage:
				{
					const D3D11_INPUT_ELEMENT_DESC* elementDesc = InputLayout::EXPANDED_IMAGE_INPUT_LAYOUT.data();
					constexpr size_t elementNum = InputLayout::EXPANDED_IMAGE_INPUT_LAYOUT.size();

					CreateVertexShader(device, _pathHolder.vertexShaderFilePath, elementDesc, elementNum);
				}
					break;
				case InputLayoutType::Sprite:
				{
					const D3D11_INPUT_ELEMENT_DESC* elementDesc = InputLayout::SPRITE_INPUT_LAYOUT.data();
					constexpr size_t elementNum = InputLayout::SPRITE_INPUT_LAYOUT.size();

					CreateVertexShader(device, _pathHolder.vertexShaderFilePath, elementDesc, elementNum);
				}
					break;
				case InputLayoutType::MeshWithoutBone:
				{
					const D3D11_INPUT_ELEMENT_DESC* elementDesc = InputLayout::MESH_INPUT_LAYOUT_WITHOUT_BONE.data();
					constexpr size_t elementNum = InputLayout::MESH_INPUT_LAYOUT_WITHOUT_BONE.size();

					CreateVertexShader(device, _pathHolder.vertexShaderFilePath, elementDesc, elementNum);
				}
					break;
				case InputLayoutType::MeshWithBone:
				{
					const D3D11_INPUT_ELEMENT_DESC* elementDesc = InputLayout::MESH_INPUT_LAYOUT_WITH_BONE.data();
					constexpr size_t elementNum = InputLayout::MESH_INPUT_LAYOUT_WITH_BONE.size();

					CreateVertexShader(device, _pathHolder.vertexShaderFilePath, elementDesc, elementNum);
				}
					break;
				}
			}

			// Hull shader.
			{
				if (_pathHolder.hullShaderFilePath != "")
				{
					CreateHullShader(device, _pathHolder.hullShaderFilePath);
				}
			}

			// Domain shader.
			{
				if (_pathHolder.domainShaderFilePath != "")
				{
					CreateDomainShader(device, _pathHolder.domainShaderFilePath);
				}
			}

			// Geometry shader.
			{
				if (_pathHolder.geometryShaderFilePath != "")
				{
					CreateGeometryShader(device, _pathHolder.geometryShaderFilePath);
				}
			}

			// Pixel shader.
			{
				if (_pathHolder.pixelShaderFilePath != "")
				{
					CreatePixelShader(device, _pathHolder.pixelShaderFilePath);
				}
			}

			// Compute shaders.
			{
				if (not _pathHolder.computeShaderFilePaths.empty())
				{
					for (auto&& path : _pathHolder.computeShaderFilePaths)
					{
						CreateComputeShader(device, path);
					}
				}
			}
		}

		// Restore constant buffer.
		if (_constantBufferSize > 0)
		{
			constantBuffer = std::make_shared<ConstantBuffer>(device, _constantBufferSize);
		}
	}

	void Shader::Save(bool isUpdate) const
	{
		// If failed 
		if (!ShaderSerializer::SerializeAsJson(_filePathToSerialized.Get(), shared_from_this()))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " shader!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " shader!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR

		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " shader!";
		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " shader!";
		}
	}

	void Shader::Load(const std::filesystem::path& filePath)
	{
		// If failed
		auto shader = std::make_shared<Shader>();

		if (!ShaderSerializer::DeserializeFromJson(filePath, shader))
		{
			DENTY_SET_ERR_COLOR

			DENTY_ERR_LOG << "Failed to load " << filePath << " shader!";

			return;
		}

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " shader!";

		shader->Clone(shared_from_this());
	}

	void Shader::Rename(std::string_view newName, [[maybe_unused]] bool deleteOldFile)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		_name = newName;
	}

	void Shader::OnReload()
	{
		// Nothing to do.
	}

	void Shader::Clone(const Ref<Asset>& asset)
	{
		DENTY_ASSERT(asset->GetAssetType() == AssetType::Shader, "Asset type must be Shader. (in Shader::Clone)");

		auto&& target = std::static_pointer_cast<Shader>(asset);

		target->_name = _name;
		target->_id = _id;
		target->_pathHolder = _pathHolder;
		target->_constantBufferSize = _constantBufferSize;
		target->_vertexShader = _vertexShader;
		target->_hullShader = _hullShader;
		target->_domainShader = _domainShader;
		target->_geometryShader = _geometryShader;
		target->_pixelShader = _pixelShader;

		target->constantBuffer = constantBuffer;

		_inputLayout.CopyTo(target->_inputLayout.ReleaseAndGetAddressOf());

		target->_inputLayoutType = _inputLayoutType;
		target->_computeShaders = _computeShaders;

		target->_filePath = _filePath;
		target->_filePathToSerialized = _filePathToSerialized;
	}

	void Shader::CreateVertexShader(ID3D11Device* device, const std::string& filePath, const D3D11_INPUT_ELEMENT_DESC* elementDesc, size_t elementNum)
	{
		const ComRef<ID3DBlob> shaderCode = CompileShader(filePath, DEFAULT_VS_TARGET);
		const bool compileSuccessful = shaderCode->GetBufferSize() > 0;
		if (compileSuccessful)
		{
			_vertexShader = std::make_shared<VertexShader>();
			_vertexShader->Create(device, shaderCode);

			_pathHolder.vertexShaderFilePath = filePath;
		}

		if (compileSuccessful && elementDesc)
		{
			const HRESULT hr = device->CreateInputLayout(
				elementDesc, static_cast<UINT>(elementNum),
				shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(),
				_inputLayout.ReleaseAndGetAddressOf()
			);

			_ASSERT_EXPR(SUCCEEDED(hr), Utils::HRTrace(hr));
		}
	}

	void Shader::CreateHullShader(ID3D11Device* device, const std::string& filePath)
	{
		const ComRef<ID3DBlob> shaderCode = CompileShader(filePath, DEFAULT_HS_TARGET);
		const bool compileSuccessful = shaderCode->GetBufferSize() > 0;
		if (compileSuccessful)
		{
			_hullShader = std::make_shared<HullShader>();
			_hullShader->Create(device, shaderCode);

			_pathHolder.hullShaderFilePath = filePath;
		}
	}

	void Shader::CreateDomainShader(ID3D11Device* device, const std::string& filePath)
	{
		const ComRef<ID3DBlob> shaderCode = CompileShader(filePath, DEFAULT_DS_TARGET);
		const bool compileSuccessful = shaderCode->GetBufferSize() > 0;
		if (compileSuccessful)
		{
			_domainShader = std::make_shared<DomainShader>();
			_domainShader->Create(device, shaderCode);

			_pathHolder.domainShaderFilePath = filePath;
		}
	}

	void Shader::CreateGeometryShader(ID3D11Device* device, const std::string& filePath)
	{
		const ComRef<ID3DBlob> shaderCode = CompileShader(filePath, DEFAULT_GS_TARGET);
		const bool compileSuccessful = shaderCode->GetBufferSize() > 0;
		if (compileSuccessful)
		{
			_geometryShader = std::make_shared<GeometryShader>();
			_geometryShader->Create(device, shaderCode);

			_pathHolder.geometryShaderFilePath = filePath;
		}
	}

	void Shader::CreatePixelShader(ID3D11Device* device, const std::string& filePath)
	{
		const ComRef<ID3DBlob> shaderCode = CompileShader(filePath, DEFAULT_PS_TARGET);
		const bool compileSuccessful = shaderCode->GetBufferSize() > 0;
		if (compileSuccessful)
		{
			_pixelShader = std::make_shared<PixelShader>();
			_pixelShader->Create(device, shaderCode);

			_pathHolder.pixelShaderFilePath = filePath;
		}
	}

	void Shader::CreateComputeShader(ID3D11Device* device, const std::string& filePath)
	{
		const ComRef<ID3DBlob> shaderCode = CompileShader(filePath, DEFAULT_CS_TARGET);
		const bool compileSuccessful = shaderCode->GetBufferSize() > 0;
		if (compileSuccessful)
		{
			std::string fileName = StringUtils::ExtractFileNameFromPath(filePath);
			const auto computeShader = std::make_shared<ComputeShader>();
			computeShader->Create(device, shaderCode);

			_computeShaders.emplace(fileName, computeShader);

			_pathHolder.computeShaderFilePaths.emplace_back(filePath);
		}
	}

	void Shader::BindVertexShader(ID3D11DeviceContext* context)
	{
		if (_vertexShader)
		{
			_vertexShader->Bind(context);

			if (_inputLayout)
			{
				context->IASetInputLayout(_inputLayout.Get());
			}

			return;
		}
		// TODO Add error log.
	}

	void Shader::BindHullShader(ID3D11DeviceContext* context)
	{
		if (_hullShader)
		{
			_hullShader->Bind(context);

			return;

		}
		// TODO Add error log.

	}

	void Shader::BindDomainShader(ID3D11DeviceContext* context)
	{
		if (_domainShader)
		{
			_domainShader->Bind(context);

			return;

		}
		// TODO Add error log.

	}

	void Shader::BindGeometryShader(ID3D11DeviceContext* context)
	{
		if (_geometryShader)
		{
			_geometryShader->Bind(context);

			return;
		}
		// TODO Add error log.

	}

	void Shader::BindPixelShader(ID3D11DeviceContext* context)
	{
		if (_pixelShader)
		{
			_pixelShader->Bind(context);

			return;
		}
		// TODO Add error log.

	}

	void Shader::BindComputeShader(ID3D11DeviceContext* context, const std::string& fileName)
	{
		auto& it = _computeShaders.at(fileName);
		if (it)
		{
			it->Bind(context);

			return;
		}

		// TODO Add error log.
	}

	void Shader::ReCompile()
	{
		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

		// Vertex shader.
		{
			if (not _pathHolder.vertexShaderFilePath.empty())
			{
				const ComRef<ID3DBlob> blob = CompileShader(_pathHolder.vertexShaderFilePath, DEFAULT_VS_TARGET);
				_vertexShader->OnReCompile(device, blob);
			}
		}

		// Hull shader.
		{
			if (not _pathHolder.hullShaderFilePath.empty())
			{
				const ComRef<ID3DBlob> blob = CompileShader(_pathHolder.hullShaderFilePath, DEFAULT_HS_TARGET);
				_hullShader->OnReCompile(device, blob);
			}
		}

		// Domain shader.
		{
			if (not _pathHolder.domainShaderFilePath.empty())
			{
				const ComRef<ID3DBlob> blob = CompileShader(_pathHolder.domainShaderFilePath, DEFAULT_DS_TARGET);
				_domainShader->OnReCompile(device, blob);
			}
		}

		// Geometry shader.
		{
			if (not _pathHolder.geometryShaderFilePath.empty())
			{
				const ComRef<ID3DBlob> blob = CompileShader(_pathHolder.geometryShaderFilePath, DEFAULT_GS_TARGET);
				_geometryShader->OnReCompile(device, blob);
			}
			
		}

		// Pixel shader.
		{
			if (not _pathHolder.pixelShaderFilePath.empty())
			{
				const ComRef<ID3DBlob> blob = CompileShader(_pathHolder.pixelShaderFilePath, DEFAULT_PS_TARGET);
				_pixelShader->OnReCompile(device, blob);
			}
		}

		// Compute shaders.
		{
			for (int index = 0; index < _pathHolder.computeShaderFilePaths.size(); ++index)
			{
				const std::filesystem::path path = _pathHolder.computeShaderFilePaths.at(index);

				const ComRef<ID3DBlob> blob = CompileShader(path.string(), DEFAULT_CS_TARGET);

				_computeShaders.at(path.filename().stem().string())->OnReCompile(device, blob);
			}
		}

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << _name << " Shader recompile succeeded!";
	}

    void Shader::LoadAssetTexture()
    {
		DENTY_ASSERT(not _shaderAssetTexture, "Shader asset texture already loaded!");

		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
		_shaderAssetTexture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::SHADER_DEFAULT_THUMBNAIL_FILE_PATH).value();

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Shader asset texture successfully loaded!";
    }

    void Shader::CreateBuildIn(ID3D11Device* device, const std::string& name, size_t constantBufferSize)
	{
		_name = name;
		_constantBufferSize = constantBufferSize;

		if (constantBufferSize > 0)
		{
			constantBuffer = std::make_shared<ConstantBuffer>(device, constantBufferSize);
		}

		// Save.
		{
			String filePath = ShaderSerializer::BUILD_IN_SHADER_OUTPUT_PATH + name;
			filePath.Append(SHADER_FILE_EXTENSION);

			_filePathToSerialized = String::ToPath(filePath);

			Save();
		}
	}

	ComRef<ID3DBlob> Shader::CompileShader(const std::string& filePath, const char* shaderTarget)
	{
		constexpr UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

		ComRef<ID3DBlob> shaderCode, errorMessage;

		const std::wstring wFilePath = StringUtils::StringToWString(filePath);
		const HRESULT hr = D3DCompileFromFile(
			wFilePath.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			shaderTarget,
			compileFlag,
			0,
			shaderCode.GetAddressOf(),
			errorMessage.GetAddressOf()
		);

		if (FAILED(hr))
		{
			// TODO 
			if (errorMessage)
			{
				// Debug
				OutputDebugStringA(static_cast<char*>(errorMessage->GetBufferPointer()));
				DENTY_ASSERT(false, "Failed to compile shader");
			}

			return nullptr;
		}

		return shaderCode;
	}
};
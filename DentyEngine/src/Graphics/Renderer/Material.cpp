#include "stdafx.h"

#include "Material.h"

#include "CubeMap.h"
#include "Texture2D.h"

#include "System/Editor/AssetPhotoStadium.h"

#include "System/Graphics/GraphicsManager.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MaterialLibrary.h"
#include "System/Resource/Library/ShaderLibrary.h"
#include "System/Resource/Library/TextureLibrary.h"

#include "System/Serializer/MaterialSerializer.h"
#include "System/Serializer/SerializeExtensions.h"

namespace DentyEngine
{
	MaterialConstants::TextureUsed::TextureUsed() noexcept :
		albedo(), metalness(), normal(), roughness(),
		emissive(), ambientOcclusion(), optionalSlot1(), optionalSlot2()
	{
	}

	MaterialConstants::MaterialConstants() noexcept :
		baseColor(), emissiveColor(), baseRate(), roughness(),
		metallic(), specular(), emissiveIntensity(), renderingMode(),
		influenceHemiSphereLight(), padding(), texturesUsed()
	{
	}

	MaterialParameters::MaterialParameters() noexcept :
		baseColor(Color::WHITE),
		emissiveColor(Color::WHITE),
		baseRate(1.0f), roughness(1.0f), metallic(0.0f),
		specular(0.5f), emissiveIntensity(0.0f), renderingMode(RenderingMode::Opaque),
		influenceHemisphereLight()
	{
	}

	MaterialParameters::MaterialParameters(const MaterialParameters& source) noexcept :
		baseColor(source.baseColor), emissiveColor(source.emissiveColor),
		baseRate(source.baseRate), roughness(source.roughness), metallic(source.metallic),
		specular(source.specular), emissiveIntensity(source.emissiveIntensity),
		renderingMode(source.renderingMode), influenceHemisphereLight(source.influenceHemisphereLight)
	{
	}

	MaterialInitData::MaterialInitData() noexcept :
		name(), filePath(), shader(), parameters(), isDefault(false)
	{
	}

	Material::Material() noexcept :
		Object(), Asset(), parameters(), _textures(),
		_shaderResourceViews(), _texturesFilePath(), _shader(), _shaderFilePath(), _default()
	{
		// Resize textures.
		_textures.resize(static_cast<size_t>(MaterialTextureIndex::Max));
		_texturesFilePath.resize(static_cast<size_t>(MaterialTextureIndex::Max));

		_shaderResourceViews.resize(static_cast<size_t>(MaterialTextureIndex::Max));
	}

	Material::Material(const Material& source) noexcept :
		Object(source), Asset(source),
		parameters(source.parameters),
		_textures(source._textures),
		_texturesFilePath(source._texturesFilePath), _shader(source._shader), _shaderFilePath(source._shaderFilePath), _default(source._default)
	{
		_shaderResourceViews.resize(static_cast<size_t>(MaterialTextureIndex::Max));
	}

	void Material::Create(const MaterialInitData& data)
	{
		DENTY_CHECK_CONDITION(data.shader);

		const FilePath path = FilePath::MakePath(data.filePath.ObtainDirectory(), data.name, MATERIAL_FILE_EXTENSION);

		_name = data.name;
		_filePath = path;
		_filePathToSerialized = path;

		_shader = data.shader;
		_shaderFilePath = _shader->GetFilePathToSerialized();
		_default = data.isDefault;

		parameters = data.parameters;

		if (!_default)
		{
			Save();
		}

		// Add to material library.
		MaterialLibrary* materialLibrary = LibraryManager::GetInstance().GetMaterialLibrary();
		materialLibrary->Add(shared_from_this());
	}

	void Material::BindTextures() const
	{
		ID3D11DeviceContext* const context = GraphicsManager::GetInstance().GetContext();

		context->PSSetShaderResources(0u, _shaderResourceViews.size(), _shaderResourceViews.data());
	}

	void Material::RemoveTextureAt(MaterialTextureIndex index)
	{
		// No texture found.
		if (!HasTextureAt(index))
		{
			DENTY_SET_WARN_COLOR
			DENTY_WARN_LOG << "No " << std::string(NAMEOF_ENUM(index)) << " texture found!";

			return;
		}

		_textures.at(static_cast<size_t>(index)) = nullptr;
		_texturesFilePath.at(static_cast<size_t>(index)) = FilePath();
		_shaderResourceViews.at(static_cast<size_t>(index)) = nullptr;
	}

	void Material::ReplaceTextureAt(Ref<Texture2D> texture, MaterialTextureIndex index)
	{
		// If already had, remove texture.
		if (HasTextureAt(index))
		{
			RemoveTextureAt(index);
		}

		SetTextureAt(texture, index);
	}

	void Material::ReplaceShaderHard(Ref<Shader> shader)
	{
		_shader = shader;
		_shaderFilePath = shader->GetFilePathToSerialized();

		Save(true);
	}

	void Material::ReplaceShaderSoft(Ref<Shader> shader) noexcept
	{
		_shader = shader;
	}

	void Material::SetTextureAt(Ref<Texture2D> texture, MaterialTextureIndex index)
	{
		if (HasTextureAt(index))
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Already " << std::string(NAMEOF_ENUM(index)) << " texture set!";

			return;
		}

		_textures.at(static_cast<size_t>(index)) = texture;
		_texturesFilePath.at(static_cast<size_t>(index)) = texture->GetFilePathToSerialized();
		_shaderResourceViews.at(static_cast<size_t>(index)) = texture->GetResourceAsComPtr().Get();
	}

	std::optional<Ref<Texture2D>> Material::GetTextureAt(MaterialTextureIndex index) const noexcept
	{
		if (!HasTextureAt(index))
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Texture " << std::string(NAMEOF_ENUM(index)) << " not found!";

			return std::nullopt;
		}

		return _textures.at(static_cast<size_t>(index));
	}

	Ref<Shader> Material::GetShader() const noexcept
	{
		return _shader;
	}

	bool Material::HasTextureAt(MaterialTextureIndex index) const
	{
		return (_textures.at(static_cast<size_t>(index)) != nullptr);
	}

	bool Material::IsEqual(const Ref<Material>& target) const
	{
		// Check textures same.
		{
			for (int8_t index = 0; index < static_cast<int8_t>(MaterialTextureIndex::Max); ++index)
			{
				if (_texturesFilePath.at(index) != target->_texturesFilePath.at(index))
					return false;
			}
		}

		// Check each parameters same.
		{
			if (!parameters.baseColor.IsApproxEqual(target->parameters.baseColor))
				return false;

			if (!parameters.emissiveColor.IsApproxEqual(target->parameters.emissiveColor))
				return false;

			if (!Utils::ApproxEqual(parameters.baseRate, target->parameters.baseRate))
				return false;

			if (!Utils::ApproxEqual(parameters.roughness, target->parameters.roughness))
				return false;

			if (!Utils::ApproxEqual(parameters.metallic, target->parameters.metallic))
				return false;

			if (!Utils::ApproxEqual(parameters.specular, target->parameters.specular))
				return false;

			if (!Utils::ApproxEqual(parameters.emissiveIntensity, target->parameters.emissiveIntensity))
				return false;

			if (parameters.renderingMode != target->parameters.renderingMode)
				return false;

			if (parameters.influenceHemisphereLight != target->parameters.influenceHemisphereLight)
				return false;
		}

		return true;
	}

	bool Material::IsDefault() const noexcept
	{
		return _default;
	}

	//
	// Object class
	//

	void Material::OnSerialized() const
	{
		// Nothing to do...
	}

	void Material::OnDeserialized()
	{
		// Restore textures.
		{
			const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

			for (int8_t index = 0; index < static_cast<int8_t>(MaterialTextureIndex::Max); ++index)
			{
				const std::optional<Ref<Texture2D>> texture = textureLibrary->Find<Texture2D>(_texturesFilePath.at(index));
				if (texture.has_value())
				{
					_textures.at(index) = texture.value();
					_shaderResourceViews.at(index) = texture.value()->GetResourceAsComPtr().Get();
				}
			}
		}

		// Restore shader.
		{
			const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

			const std::optional<Ref<Shader>> shader = shaderLibrary->Find(_shaderFilePath);
			DENTY_CHECK_CONDITION(shader.has_value());

			_shader = shader.value();
		}
	}

	//
	// Asset class
	//

	void Material::Save(bool isUpdate) const
	{
		// If failed.
		if (!MaterialSerializer::SerializeAsJson(_filePathToSerialized.Get(), shared_from_this()))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " material!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " material!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR

		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " material!";
		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " material!";
		}
	}

	void Material::Load(const std::filesystem::path& filePath)
	{
		auto material = std::make_shared<Material>();

		// If failed.
		if (!MaterialSerializer::DeserializeFromJson(filePath, material))
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Failed to load " << filePath << " material!";

			return;
		}

		material->Clone(shared_from_this());

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " material!";
	}

	void Material::Rename([[maybe_unused]] std::string_view newName, [[maybe_unused]] bool deleteOldFile)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__);
	}

	void Material::OnReload()
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__);
	}

	void Material::Clone(const Ref<Asset>& asset)
	{
		DENTY_CHECK_CONDITION(asset->GetAssetType() == Asset::AssetType::Material);

		auto&& target = std::static_pointer_cast<Material>(asset);

		// Object class.
		{
			target->_name = _name;
		}

		// Asset class.
		{
			target->_previewImageFilePath = _previewImageFilePath;
			target->_filePath = _filePath;
			target->_filePathToSerialized = _filePathToSerialized;
		}

		// This class.
		{
			target->parameters = parameters;
			target->_textures = _textures;
			target->_shaderResourceViews = _shaderResourceViews;
			target->_texturesFilePath = _texturesFilePath;
			target->_shader = _shader;
			target->_shaderFilePath = _shaderFilePath;
			target->_default = _default;
		}
	}

	Asset::AssetType Material::GetAssetType() const noexcept
	{
		return AssetType::Material;
	}

	void Material::Setup()
	{
		// Get shader & texture library.
		const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();
		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

		const Ref<Shader> staticMeshShader = shaderLibrary->Find(ShaderLibrary::STATIC_MESH_SHADER_PATH).value();
		const Ref<Shader> skinnedMeshShader = shaderLibrary->Find(ShaderLibrary::SKINNED_MESH_SHADER_PATH).value();
		const Ref<Shader> staticShadowMeshShader = shaderLibrary->Find(ShaderLibrary::STATIC_MESH_SHADOW_SHADER_PATH).value();
		const Ref<Shader> skinnedShadowMeshShader = shaderLibrary->Find(ShaderLibrary::SKINNED_MESH_SHADOW_SHADER_PATH).value();

		// Create static default material.
		{
			MaterialInitData data = {};
			data.name = "StaticDefaultMaterial";
			data.shader = staticMeshShader;
			data.isDefault = true;

			_staticDefaultMaterial = std::make_shared<Material>();
			_staticDefaultMaterial->Create(data);
		}

		// Create skinned default material.
		{
			MaterialInitData data = {};
			data.name = "SkinnedDefaultMaterial";
			data.shader = skinnedMeshShader;
			data.isDefault = true;

			_skinnedDefaultMaterial = std::make_shared<Material>();
			_skinnedDefaultMaterial->Create(data);
		}

		// Create static shadow default material.
		{
			MaterialInitData data = { };
			data.name = "StaticShadowDefaultMaterial";
			data.shader = staticShadowMeshShader;
			data.isDefault = true;

			_staticShadowDefaultMaterial = std::make_shared<Material>();
			_staticShadowDefaultMaterial->Create(data);
		}

		// Create skinned shadow default material.
		{
			MaterialInitData data = { };
			data.name = "SkinnedShadowDefaultMaterial";
			data.shader = skinnedShadowMeshShader;
			data.isDefault = true;

			_skinnedShadowDefaultMaterial = std::make_shared<Material>();
			_skinnedShadowDefaultMaterial->Create(data);
		}

		// Get default thumbnail texture from library.
		{
			const std::optional<Ref<Texture2D>> thumbnail = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::MATERIAL_DEFAULT_THUMBNAIL_FILE_PATH);
			DENTY_CHECK_CONDITION(thumbnail.has_value());

			_defaultThumbnail = thumbnail.value();
		}
	}

	Ref<Material> Material::GetStaticDefaultMaterial() noexcept
	{
		return _staticDefaultMaterial;
	}

	Ref<Material> Material::GetSkinnedDefaultMaterial() noexcept
	{
		return _skinnedDefaultMaterial;
	}

	Ref<Material> Material::GetStaticShadowDefaultMaterial() noexcept
	{
		return _staticShadowDefaultMaterial;
	}

	Ref<Material> Material::GetSkinnedShadowDefaultMaterial() noexcept
	{
		return _skinnedShadowDefaultMaterial;
	}

	Ref<Texture2D> Material::GetDefaultThumbNail() noexcept
	{
		return _defaultThumbnail;
	}

	//
	// Static variables.
	//
	const String Material::ADD_FILENAME_AT_END = "_DentyMaterial";
	const std::array<std::string, 2u> Material::RENDERING_MODES =
	{
		"Opaque", "Transparent"
	};

	Ref<Material> Material::_staticDefaultMaterial = nullptr;
	Ref<Material> Material::_skinnedDefaultMaterial = nullptr;
	Ref<Material> Material::_staticShadowDefaultMaterial = nullptr;
	Ref<Material> Material::_skinnedShadowDefaultMaterial = nullptr;

	Ref<Texture2D> Material::_defaultThumbnail = nullptr;
}

namespace DentyEngine
{
	
};

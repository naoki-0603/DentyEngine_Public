#include "stdafx.h"

#include "AssetPhotoStadium.h"

#include "Graphics/Buffer/ColorBuffer.h"

#include "Graphics/Renderer/Material.h"
#include "Graphics/Renderer/Texture2D.h"

#include "System/Graphics/GraphicsManager.h"

#include "System/Resource/Asset.h"

#include "System/Serializer/SerializeExtensions.h"

namespace DentyEngine
{
	AssetPhotoStadium::AssetPhotoStadium() 
	{
	}

	Ref<Texture2D> AssetPhotoStadium::TakePhoto(const Asset* asset) const
	{
		// Get d3d11 device and context.
		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		// Get assets type.
		using AssetType = Asset::AssetType;
        const AssetType assetType = asset->GetAssetType();

		// Create photo camera.
		auto photoTaker = AssetPhotoTaker();
		photoTaker.Initialize(device, context);

		switch (assetType)
		{
		case AssetType::Mesh:
			{
				const auto colorBuffer = photoTaker.TakeMeshPhoto((Mesh*)(asset));

				// Save texture.
				{
					ComRef<ID3D11Resource> resource = nullptr;
					colorBuffer->CopyTo(resource.GetAddressOf());

					auto filePath = DENTY_FILE_PATH_REPLACE_EXTENSION(asset->GetFilePathToSerialized(), SAVE_TEXTURE_FILE_EXTENSION);

					// If successful, serialize.
					if (Texture2D::SaveTextureToFile(
						context, resource.Get(), filePath, colorBuffer->GetSize(), colorBuffer->GetFormat()
					))
					{
						const auto texture = std::make_shared<Texture2D>();

						// Load and add to texture library.
						texture->Load(device, filePath);
					}
				}
			}
			break;
		case AssetType::Material:
			{
				const auto colorBuffer = photoTaker.TakeMaterialPhoto((Material*)(asset));

				// Save texture.
				{
					ComRef<ID3D11Resource> resource = nullptr;
					colorBuffer->CopyTo(resource.GetAddressOf());

					auto filePath = DENTY_FILE_PATH_REPLACE_EXTENSION(asset->GetFilePathToSerialized(), SAVE_TEXTURE_FILE_EXTENSION);

					// If successful, serialize.
					if (Texture2D::SaveTextureToFile(
						context, resource.Get(), filePath, colorBuffer->GetSize(), colorBuffer->GetFormat()
					))
					{
						const auto texture = std::make_shared<Texture2D>();

						// Load and add to texture library.
						texture->Load(device, filePath);
					}
				}
			}
			break;
		case AssetType::Prefab:
			{
				const auto colorBuffer = photoTaker.TakePrefabPhoto((Prefab*)(asset));

				// Save texture.
				{
					ComRef<ID3D11Resource> resource = nullptr;
					colorBuffer->CopyTo(resource.GetAddressOf());

					auto filePath = DENTY_FILE_PATH_REPLACE_EXTENSION(asset->GetFilePathToSerialized(), SAVE_TEXTURE_FILE_EXTENSION);

					// If successful, serialize.
					if (Texture2D::SaveTextureToFile(
						context, resource.Get(), filePath, colorBuffer->GetSize(), colorBuffer->GetFormat()
					))
					{
						const auto texture = std::make_shared<Texture2D>();

						// Load and add to texture library.
						texture->Load(device, filePath);
					}
				}
			}
			break;
		default:
			DENTY_ASSERT(false, "AssetPhotoStadium::TakePhoto");
			break;
		}

		return nullptr;
	}
}

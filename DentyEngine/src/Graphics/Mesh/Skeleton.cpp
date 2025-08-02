#include "stdafx.h"

#include "Skeleton.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"
#include "System/Serializer/SkeletonSerializer.h"

namespace DentyEngine
{
	namespace MeshResource
	{
		const String Skeleton::ADD_FILENAME_AT_END = "_DentyAvatar";

		Ref<Texture2D> Skeleton::_avatarAssetTexture = nullptr;

	    Skeleton::Skeleton() noexcept :
			Object(), Asset(), _resource()
        {
        }

		Skeleton::Skeleton(const SkeletonResource& resource) noexcept :
            Object(), Asset(), _resource(resource)
        {
        }

		void Skeleton::Save(bool isUpdate) const
		{
			// If failed 
			if (!SkeletonSerializer::SerializeAsJson(_filePathToSerialized.Get(), shared_from_this()))
			{
				DENTY_SET_ERR_COLOR

				if (isUpdate)
				{
					DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " avatar!";
				}
				else
				{
					DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " avatar!";
				}

				return;
			}

			DENTY_SET_INFO_COLOR

			if (isUpdate)
			{
				DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " avatar!";
			}
			else
			{
				DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " avatar!";
			}
		}

		void Skeleton::Load(const std::filesystem::path& filePath)
		{
			// If failed
			auto skeleton = std::make_shared<Skeleton>();

			if (!SkeletonSerializer::DeserializeFromJson(filePath, skeleton))
			{
				DENTY_SET_ERR_COLOR
				DENTY_ERR_LOG << "Failed to load " << filePath << " avatar!";

				return;
			}

			DENTY_SET_INFO_COLOR
			DENTY_INFO_LOG << "Succeed to load " << filePath << " avatar!";

			skeleton->Clone(shared_from_this());
		}

		void Skeleton::Rename([[maybe_unused]] std::string_view newName, [[maybe_unused]] bool deleteOldFile)
		{
		}

		void Skeleton::OnReload()
		{
		}

		void Skeleton::Clone(const Ref<Asset>& asset)
		{
			DENTY_ASSERT(asset->GetAssetType() == AssetType::Avatar, "Asset type must be avatar! (in Skeleton::Clone)");

			auto&& target = std::static_pointer_cast<Skeleton>(asset);

			target->_resource = _resource;

			// Object
			{
				target->_name = _name;
			}

			// Asset
			{
				target->_filePath = _filePath;
				target->_filePathToSerialized = _filePathToSerialized;
			}
		}

		void Skeleton::OnSerialized() const
		{
			Object::OnSerialized();
		}

		void Skeleton::OnDeserialized()
		{
			Object::OnDeserialized();
		}

		void Skeleton::OnEvent(Event* e)
		{
			Object::OnEvent(e);
		}

		void Skeleton::OnDestroy()
		{
			Object::OnDestroy();
		}

		void Skeleton::Destroy()
		{
			Object::Destroy();
		}

        void Skeleton::AddBone(const Bone& bone)
		{
			_resource.bones.emplace_back(bone);
		}

        void Skeleton::UpdateBone(std::string_view boneName, [[maybe_unused]] const Matrix& localToGlobal, const Matrix& orientation)
        {
			auto&& bone = FindBoneByNameRef(boneName);
			bone->_resource.localTransform = orientation;
        }

        const Bone* Skeleton::FindBoneByIndex(uint32_t index) const
		{
			const uint32_t size = static_cast<uint32_t>(_resource.bones.size());
			if (index >= size)
			{
				DENTY_SET_ERR_COLOR
				DENTY_ERR_LOG << "Index out of range! (in Skeleton::FindBoneByIndex)";

				return nullptr;
			}

			return &_resource.bones.at(index);
		}

		const Bone* Skeleton::FindBoneByName(std::string_view name) const
		{
			for (int boneIndex = 0; boneIndex < _resource.bones.size(); ++boneIndex)
			{
				const Bone* bone = &_resource.bones.at(boneIndex);

				if (bone->GetResource().name != name)
					continue;

				return bone;
			}

			return nullptr;
		}

		void Skeleton::LoadAssetTexture()
		{
			DENTY_ASSERT(not _avatarAssetTexture, "Avatar asset texture already loaded!");

			const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
			_avatarAssetTexture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::SCENE_TREE_DEFAULT_THUMBNAIL_FILE_PATH).value();

			DENTY_SET_INFO_COLOR
			DENTY_INFO_LOG << "Avatar asset texture successfully loaded!";
		}

		Bone* Skeleton::FindBoneByNameRef(std::string_view name)
		{
			for (auto&& bone : _resource.bones)
			{
				if (bone.GetResource().name != name)
					continue;

				return &bone;
			}

			return nullptr;
		}

		Bone* Skeleton::FindInitialBoneByNameRef(std::string_view name)
		{
			for (auto&& bone : _resource.initialBones)
			{
				if (bone.GetResource().name != name)
					continue;

				return &bone;
			}

			return nullptr;
		}

	}
}

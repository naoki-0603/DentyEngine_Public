#include "stdafx.h"

#include "AnimationClip.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

#include "Graphics/Mesh/SceneTree.h"

#include "Graphics/Renderer/Texture2D.h"

#include "System/Serializer/AnimationClipSerializer.h"

namespace DentyEngine
{
	namespace MeshResource
	{
		const String AnimationClip::ADD_FILENAME_AT_END = "_AnimationClip";

		Ref<Texture2D> AnimationClip::_animationClipAssetTexture = nullptr;

		std::optional<Keyframe::Node> Keyframe::FindNodeByBoneName(std::string_view boneName) const
		{
			for (const auto& node : nodes)
			{
				if (node.boneName.Get() != boneName)
					continue;

				return node;
			}

			return std::nullopt;
		}

		AnimationClip::AnimationClip() :
			Object(), Asset(), _id(), _ticksPerSecond(), _duration(), _ticksPerFrame(), _ticksPerDuration(), _sequence()
		{
		}

		void AnimationClip::OnEvent(Event* e)
		{
			Object::OnEvent(e);
		}

		void AnimationClip::Destroy()
		{
			Object::Destroy();
		}

		void AnimationClip::OnDestroy()
		{
			Object::OnDestroy();
		}

		void AnimationClip::OnSerialized() const
		{
			Object::OnSerialized();
		}

		void AnimationClip::OnDeserialized()
		{
			Object::OnDeserialized();
		}

		void AnimationClip::Save(bool isUpdate) const
		{
			// If failed 
			if (!AnimationClipSerializer::SerializeAsBinary(_filePathToSerialized.Get(), shared_from_this()))
			{
				DENTY_SET_ERR_COLOR

				if (isUpdate)
				{
					DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " animation clip!";
				}
				else
				{
					DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " animation clip!";
				}

				return;
			}

			DENTY_SET_INFO_COLOR

			if (isUpdate)
			{
				DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " animation clip!";
			}
			else
			{
				DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " animation clip!";
			}
		}

		void AnimationClip::Load(const std::filesystem::path& filePath)
		{
			// If failed
			auto animationClip = std::make_shared<AnimationClip>();

			if (!AnimationClipSerializer::DeserializeFromBinary(filePath, animationClip))
			{
				DENTY_SET_ERR_COLOR
				DENTY_ERR_LOG << "Failed to load " << filePath << " animation clip!";

				return;
			}

			DENTY_SET_INFO_COLOR
			DENTY_INFO_LOG << "Succeed to load " << filePath << " animation clip!";

			animationClip->Clone(shared_from_this());
		}

		void AnimationClip::Rename(std::string_view newName, [[maybe_unused]] bool deleteOldFile)
		{
			_name = newName;

			/*std::filesystem::remove(_filePathToSerialized);

			DENTY_REPLACE_FILENAME(_filePath, newName);
			DENTY_REPLACE_FILENAME(_filePathToSerialized, newName);*/
		}

		void AnimationClip::OnReload()
		{
		}

		void AnimationClip::Clone(const Ref<Asset>& asset)
		{
			DENTY_ASSERT(asset->GetAssetType() == AssetType::Animation, "Asset type must be animation! (in AnimationClip::Clone)");

			auto&& target = std::static_pointer_cast<MeshResource::AnimationClip>(asset);

			// Object
			{
				target->_name = _name;
				target->_uuid = _uuid;
			}

			// Asset
			{
				target->_previewImage = _previewImage;
				target->_previewImageFilePath = _previewImageFilePath;
				target->_filePath = _filePath;
				target->_filePathToSerialized = _filePathToSerialized;
			}

			// This class
			{
				target->_id = _id;
				target->_duration = _duration;
				target->_ticksPerSecond = _ticksPerSecond;
				target->_ticksPerDuration = _ticksPerDuration;
				target->_ticksPerFrame = _ticksPerFrame;
				target->_sequence = _sequence;
			}
		}

		void AnimationClip::AddKeyframe(const Keyframe& keyframe)
		{
			_sequence.emplace_back(keyframe);
		}

		const Keyframe& AnimationClip::FindKeyframeAt(uint32_t index) const
		{
			if (index >= _sequence.size())
			{
				DENTY_SET_ERR_COLOR
				DENTY_ERR_LOG << "Index out of range!";

				DENTY_ASSERT(false, "");
			}

			return _sequence.at(index);
		}

		void AnimationClip::LoadAssetTexture()
		{
			DENTY_ASSERT(not _animationClipAssetTexture, "Animation clip asset texture already loaded!");

			const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
			const auto texture = textureLibrary->Find<Texture2D>(DENTY_FILE_PATH("Assets/Textures/Editor/ContentBrowser/AnimationClip_DentyTexture.Json"));

			if (texture.has_value())
			{
				_animationClipAssetTexture = texture.value();

				DENTY_SET_INFO_COLOR
				DENTY_INFO_LOG << "Animation clip asset texture successfully loaded!";

				return;
			}
		}
	}
}

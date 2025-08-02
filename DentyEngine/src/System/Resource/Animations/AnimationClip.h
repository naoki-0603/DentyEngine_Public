#pragma once

#include "System/Core/DString.h"
#include "System/Core/Object.h"

#include "System/Resource/Asset.h"

#include "System/Core/ID/AnimationClipID.h"

#include "Graphics/Mesh/Bone.h"

namespace DentyEngine
{
	namespace MeshResource
	{
		struct Keyframe
		{
		public:
			struct Node
			{
				Node() : boneName(), time(),
					position(), rotation(), scaling(),
					orientation(), globalTransform(),
					hasPositionValue(), hasRotationValue(), hasScalingValue() {}

				String boneName;

				float time;

				Vector3 position;
				Quaternion rotation;
				Vector3 scaling;

				// Local
				Matrix orientation;

				// Global
				Matrix globalTransform;

				bool hasPositionValue;
				bool hasRotationValue;
				bool hasScalingValue;
			private:
				friend class cereal::access;

				template <class Archive>
				void save(Archive& archive, const uint32_t version) const
				{
					if (version <= 1u)
					{
					    archive(
							CEREAL_NVP(boneName),
							CEREAL_NVP(time),
							CEREAL_NVP(position),
							CEREAL_NVP(rotation),
							CEREAL_NVP(scaling),
							CEREAL_NVP(orientation),
							CEREAL_NVP(globalTransform),
							CEREAL_NVP(hasPositionValue),
							CEREAL_NVP(hasRotationValue),
							CEREAL_NVP(hasScalingValue)
					    );
					}
				}

				template <class Archive>
				void load(Archive& archive, const uint32_t version)
				{
					if (version <= 1u)
					{
					    archive(
							CEREAL_NVP(boneName),
							CEREAL_NVP(time),
							CEREAL_NVP(position),
							CEREAL_NVP(rotation),
							CEREAL_NVP(scaling),
							CEREAL_NVP(orientation),
							CEREAL_NVP(globalTransform),
							CEREAL_NVP(hasPositionValue),
							CEREAL_NVP(hasRotationValue),
							CEREAL_NVP(hasScalingValue)
					    );
					}
				}
			};
		public:
			Keyframe() : nodes() {}

			std::vector<Node> nodes;
		public:
			[[nodiscard]]
			std::optional<Node> FindNodeByBoneName(std::string_view boneName) const;

		private:
			friend class cereal::access;

			template <class Archive>
			void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
			{
				archive(
					CEREAL_NVP(nodes)
				);
			}

			template <class Archive>
			void load(Archive& archive, [[maybe_unused]] const uint32_t version)
			{
				archive(
					CEREAL_NVP(nodes)
				);
			}
		};

		class AnimationClip final : public Object, public Asset, public std::enable_shared_from_this<AnimationClip>
		{
		public:
			AnimationClip();
			~AnimationClip() override = default;

			void OnEvent(Event* e) override;

			void Destroy() override;
			void OnDestroy() override;

			void OnSerialized() const override;
			void OnDeserialized() override;

			void Save(bool isUpdate = false) const override;
			void Load(const std::filesystem::path& filePath) override;

			void Rename(std::string_view newName, bool deleteOldFile) override;
			void OnReload() override;

			void Clone(const Ref<Asset>& asset) override;

			void AddKeyframe(const Keyframe& keyframe);

			[[nodiscard]]
			const Keyframe& FindKeyframeAt(uint32_t index) const;

			//
			// Setter
			//
			void SetDuration(float duration) { _duration = duration; }
			void SetTicksPerSecond(float ticksPerSecond) { _ticksPerSecond = ticksPerSecond; }
			void SetTicksPerDuration(float ticksPerDuration) { _ticksPerDuration = ticksPerDuration; }
			void SetTicksPerFrame(float ticksPerFrame) { _ticksPerFrame = ticksPerFrame; }

			//
			// Getter
			//
			[[nodiscard]]
			float GetDuration() const { return _duration; }

			[[nodiscard]]
			float GetTicksPerSecond() const { return (_ticksPerSecond <= 0.0f ? 30.0f : _ticksPerSecond); }

			[[nodiscard]]
			float GetTicksPerFrame() const { return _ticksPerFrame; }

			[[nodiscard]]
			float GetTicksPerDuration() const { return _ticksPerDuration; }

			[[nodiscard]]
			const std::vector<Keyframe>& GetKeyframes() const { return _sequence; }

			[[nodiscard]]
			size_t GetKeyframeCount() const { return _sequence.size(); }

			[[nodiscard]]
			AnimationClipID GetAnimationClipID() const { return _id; }

			[[nodiscard]]
			AssetType GetAssetType() const override { return AssetType::Animation; }

			//
			// Static functions.
			//
			[[nodiscard]]
			static Ref<Texture2D> GetAssetTexture() { return _animationClipAssetTexture; }

			static void LoadAssetTexture();
		private:
			friend class cereal::access;

			template <class Archive>
			void save(Archive& archive, const uint32_t version) const
			{
				if (version <= 1u)
				{
				    archive(
					cereal::base_class<Object>(this),
					cereal::base_class<Asset>(this),
					CEREAL_NVP(_id),
					CEREAL_NVP(_ticksPerSecond),
					CEREAL_NVP(_duration),
					CEREAL_NVP(_ticksPerFrame),
					CEREAL_NVP(_ticksPerDuration),
					CEREAL_NVP(_sequence)
				);
				}

				OnSerialized();
			}

			template <class Archive>
			void load(Archive& archive, const uint32_t version)
			{
				if (version <= 1u)
				{
				    archive(
					cereal::base_class<Object>(this),
					cereal::base_class<Asset>(this),
					CEREAL_NVP(_id),
					CEREAL_NVP(_ticksPerSecond),
					CEREAL_NVP(_duration),
					CEREAL_NVP(_ticksPerFrame),
					CEREAL_NVP(_ticksPerDuration),
					CEREAL_NVP(_sequence)
				);
				}

				OnDeserialized();
			}
		private:
			AnimationClipID _id;

			float _ticksPerSecond;
			float _duration;

			float _ticksPerFrame; // 1.0f / _ticksPerSecond.
			float _ticksPerDuration; // _duration / _ticksPerSecond.

			std::vector<Keyframe> _sequence;
		private:
			//
			// Static variables.
			//
			static const String ADD_FILENAME_AT_END;

			static Ref<Texture2D> _animationClipAssetTexture;
		};
	}
}

CEREAL_CLASS_VERSION(DentyEngine::MeshResource::Keyframe::Node, 1u)
CEREAL_CLASS_VERSION(DentyEngine::MeshResource::Keyframe, 1u)
CEREAL_CLASS_VERSION(DentyEngine::MeshResource::AnimationClip, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::MeshResource::AnimationClip)
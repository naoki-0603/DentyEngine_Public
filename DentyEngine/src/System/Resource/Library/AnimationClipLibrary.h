#pragma once

#include "System/Resource/ILibrary.h"

#include "System/Core/ID/AnimationClipID.h"

#include "System/Core/DFilePath.h"

namespace DentyEngine
{
	namespace MeshResource
	{
		class AnimationClip;
	}

	class AnimationClipLibrary final : public ILibrary
	{
	public:
		//// Use for unordered_map key.
		//struct AnimationClipKey
		//{
		//	AnimationClipKey() : path(), id(0) {}

		//	// Contains file path.
		//	std::filesystem::path path;

		//	AnimationClipID id;

		//	// operators.
		//public:
		//	bool operator==(const AnimationClipKey& other) const
		//	{
		//		return (path == other.path || id == other.id);
		//	}
		//};

		//struct AnimationClipKeyHash
		//{
		//	std::size_t operator()(const AnimationClipKey& animationClip) const
		//	{
		//		const std::size_t h1 = std::hash<std::filesystem::path>()(animationClip.path);
		//		const std::size_t h2 = std::hash<AnimationClipID>()(animationClip.id);

		//		return h1 ^ h2;
		//	}
		//};
	public:
		AnimationClipLibrary();
		~AnimationClipLibrary() override = default;

		void Initialize() override;
		void Finalize() override;
		void Update() override;

		void OnGui() override;
		void OnEvent(Event* e) override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Add(const Ref<MeshResource::AnimationClip>& clip);

		void Remove(const FilePath& filePath);

		[[nodiscard]]
		std::optional<Ref<MeshResource::AnimationClip>> Find(const FilePath& filePath) const;

		[[nodiscard]]
		bool Reload(const FilePath& path) override;

		//void Remove(AnimationClipID id);


		//[[nodiscard]] Ref<MeshResource::AnimationClip> Find(AnimationClipID animationClipID) const;
	private:
		void Reload(AnimationClipID id);

	private:
		//using AnimationClips = std::unordered_map<AnimationClipKey, Ref<MeshResource::AnimationClip>, AnimationClipKeyHash>;
		using AnimationClips = std::unordered_map<FilePath, Ref<MeshResource::AnimationClip>, FilePath::Hash>;

		AnimationClips _caches;
	};
}

#pragma once

#include "System/Core/DString.h"
#include "System/Resource/ILibrary.h"

namespace DentyEngine
{
	namespace MeshResource
	{
		class Skeleton;
	}

	class SkeletonLibrary final : public ILibrary
	{
	public:
		SkeletonLibrary();
		~SkeletonLibrary() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(SkeletonLibrary)

		void Initialize() override;
		void Update() override;
		void OnGui() override;
		void OnEvent(Event* e) override;
		void Finalize() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Add(const Ref<MeshResource::Skeleton>&);

		void Remove(const FilePath& filePath);

		[[nodiscard]]
		std::optional<Ref<MeshResource::Skeleton>> Find(const FilePath& filePath) const;

		[[nodiscard]]
		bool Reload(const FilePath& path) override;
	private:
		[[nodiscard]]
		bool IsSkeletonFile(const String&) const;

	private:
		using Skeletons = std::unordered_map<FilePath, Ref<MeshResource::Skeleton>, FilePath::Hash>;

		Skeletons _caches;
	};
}

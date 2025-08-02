#pragma once

#include "System/Core/DString.h"
#include "System/Resource/ILibrary.h"

namespace DentyEngine
{
	class Skybox;

	class SkyboxLibrary final : public ILibrary
	{
	public:
		SkyboxLibrary();
		~SkyboxLibrary() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(SkyboxLibrary)

		void Initialize() override;
		void Update() override;
		void OnGui() override;
		void OnEvent(Event* e) override;
		void Finalize() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Add(const Ref<Skybox>& skybox);

		void Remove(const FilePath& filePath);

		[[nodiscard]]
		std::optional<Ref<Skybox>> Find(const FilePath& filePath) const;

		[[nodiscard]]
		bool Reload(const FilePath& path) override;
	private:
		[[nodiscard]]
		bool IsSkyboxFile(const String& filePath) const;
	private:
		using Skyboxes = std::unordered_map<FilePath, Ref<Skybox>, FilePath::Hash>;

		Skyboxes _caches;
	};
}

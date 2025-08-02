#pragma once

#include "../ILibrary.h"

#include "System/Core/ID/MaterialID.h"

#include "System/Core/DString.h"

namespace DentyEngine
{
	//
	// Events
	//
	class EventOnMaterialAssetUpdated;
	class EventOnMaterialEditorClosed;
	class EventOnMaterialParameterChanged;

	class Material;
	class CubeMaterial;

	class MaterialLibrary final : public ILibrary
	{
	public:
		MaterialLibrary();
		~MaterialLibrary() override = default;

		void Initialize() override;
		void Finalize() override;
		void Update() override;
		void OnGui() override;
		void OnEvent(Event* e) override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Add(Ref<Material> material);
		void Add(Ref<CubeMaterial> cubeMaterial);

		void Remove(const FilePath& filePath);

		[[nodiscard]]
		std::optional<Ref<Material>> Find(const FilePath& filePath) const;

		[[nodiscard]]
		std::optional<Ref<CubeMaterial>> FindCube(const FilePath& filePath) const;

		[[nodiscard]]
		bool Reload(const FilePath& path) override;

		// Event functions.
	private:
		void OnMaterialAssetUpdated(EventOnMaterialAssetUpdated* e);

	private:
		[[nodiscard]]
		bool IsMaterialFile(const String&) const;

		[[nodiscard]]
		bool IsCubeMaterialFile(const FilePath& filePath) const noexcept;
	private:
		// This use in runtime.
		// Doesn't serialize.
		std::unordered_map<FilePath, Ref<Asset>, FilePath::Hash> _caches;
	};
}

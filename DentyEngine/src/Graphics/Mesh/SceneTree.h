#pragma once

#include "System/Core/DString.h"
#include "Node.h"

#include "System/Core/Object.h"

#include "System/Resource/Asset.h"

namespace DentyEngine
{
	class MeshImporter;

	class Mesh;

	namespace MeshResource
	{
		struct SceneTreeResource
		{
			SceneTreeResource() : name(), parentIndex(0), nodeType(), transform(),
		        computedTransform(), meshFilePath(), mesh(), root(), isSkinned() {}

			std::string name;

			// -1 means doesn't have parent.
			int32_t parentIndex;

			NodeType nodeType;

			Matrix transform;
			Matrix computedTransform;

			FilePath meshFilePath;

			// Only exists if node type is mesh.
			Ref<Mesh> mesh;

			bool root;
			bool isSkinned;
		private:
			friend class cereal::access;

			template <class Archive>
			void save(Archive& archive, const uint32_t version) const
			{
				if (version <= 1u)
				{
					archive(
						CEREAL_NVP(name),
						CEREAL_NVP(nodeType),
						CEREAL_NVP(transform),
						CEREAL_NVP(computedTransform),
						CEREAL_NVP(meshFilePath),
						CEREAL_NVP(root),
						CEREAL_NVP(isSkinned)
					);    
				}
			}

			template <class Archive>
			void load(Archive& archive, const uint32_t version)
			{
				if (version <= 1u)
				{
				    archive(
						CEREAL_NVP(name),
						CEREAL_NVP(nodeType),
						CEREAL_NVP(transform),
						CEREAL_NVP(computedTransform),
						CEREAL_NVP(meshFilePath),
						CEREAL_NVP(root),
						CEREAL_NVP(isSkinned)
				    );
				}
			}
		};

		class SceneTree final : public Object, public Asset, public std::enable_shared_from_this<SceneTree>
		{
			friend class DentyEngine::MeshImporter;
		public:
			SceneTree();
			~SceneTree() override = default;

			void Save(bool isUpdate) const override;
			void Load(const std::filesystem::path& filePath) override;

			void Rename(std::string_view newName, bool deleteOldFile) override;
			void OnReload() override;

			void Clone(const Ref<Asset>& asset) override;

			void Add(const SceneTreeResource& resource) { _resources.emplace_back(resource); }

			// Call on SceneTreeLibrary::GenerateTreesMeshes function.
			void AttachMeshes();

			// Call on MeshImporter::SetMeshInfos function.
			void BindMeshes(Ref<Mesh> mesh);

			[[nodiscard]]
			std::optional<SceneTreeResource> Find(uint32_t index) const;

			[[nodiscard]]
			std::optional<SceneTreeResource> Find(std::string_view name) const;

			//
			// Getter
			//
			[[nodiscard]]
			const std::vector<SceneTreeResource>& GetResources() const { return _resources; }

			[[nodiscard]]
			AssetType GetAssetType() const override { return AssetType::SceneTree; }

			//
			// Static functions.
			//
			static void LoadAssetTexture();

			[[nodiscard]]
			static Ref<Texture2D> GetAssetTexture() { return _sceneTreeAssetTexture; }
		public:
			static const String ADD_FILENAME_AT_END;

		private:
			[[nodiscard]]
			Matrix ComputeTransform(const Matrix& matrix, const SceneTreeResource& treeResource);
		private:
			friend class cereal::access;

			template <class Archive>
			void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
			{
				archive(
					cereal::base_class<Object>(this),
					cereal::base_class<Asset>(this),
					CEREAL_NVP(_resources)
				);
			}

			template <class Archive>
			void load(Archive& archive, [[maybe_unused]] const uint32_t version)
			{
				archive(
					cereal::base_class<Object>(this),
					cereal::base_class<Asset>(this),
					CEREAL_NVP(_resources)
				);
			}
		private:
			std::vector<SceneTreeResource> _resources;

			static Ref<Texture2D> _sceneTreeAssetTexture;
		};
	}
}

CEREAL_CLASS_VERSION(DentyEngine::MeshResource::SceneTreeResource, 1u)
CEREAL_CLASS_VERSION(DentyEngine::MeshResource::SceneTree, 1u)
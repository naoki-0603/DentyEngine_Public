#pragma once

#include "System/Core/Object.h"

#include "System/Core/ID/MeshID.h"
#include "System/Core/ID/EntityID.h"

#include "Utilities/BoundingBox.h"

#include "Graphics/Renderer/Material.h"

#include "SystemUnit.h"

namespace DentyEngine
{
	class DXVertexBuffer;
	class DXIndexBuffer;

	namespace MeshResource
	{
		class SceneTree;
	}

	class Material;
	class Mesh : public Asset, public Object
	{
		friend class MeshEditor;
		friend class MeshImporter;
	public:
		// For serialize.
		Mesh();
		Mesh(std::string_view name);
		Mesh(const Mesh& source);
		virtual ~Mesh() override = default;

		virtual void OnUpdate([[maybe_unused]] const Matrix& world, [[maybe_unused]] const EntityID entityID, [[maybe_unused]] const Ref<Material>& rendererMaterial, [[maybe_unused]] const float deltaTime) {}
		virtual void OnGui();
		virtual void OnRender([[maybe_unused]] ID3D11DeviceContext* context, [[maybe_unused]] const Ref<Material>& rendererMaterial, [[maybe_unused]] const std::vector<Ref<Material>>& materials) {}
		virtual void OnRenderShadow([[maybe_unused]] ID3D11DeviceContext* context, [[maybe_unused]] uint32_t cascadeShadowAreaNo) {}

		virtual void OnEvent(Event* e) override;
		virtual void OnSerialized() const override;
		virtual void OnDeserialized() override;

		void Save(bool isUpdate = false) const override;
		void Load(const std::filesystem::path& filePath) override;

		[[maybe_unused]]
	    void Rename(std::string_view newName, bool deleteOldFile) override;

		void OnReload() override;

		void Clone(const Ref<Asset>& asset) override;

		//
		// Add functions.
		//
		void AddMaterial(const Ref<Material>& material);
		void AddSubMesh(const Ref<Mesh>& mesh);

		//
		// Remove functions.
		//
		void RemoveMaterial(std::string_view name);

		virtual void ChangeMaterial(std::string_view targetName, const Ref<Material>& newMaterial);

		//
		// Find functions.
		//
		template <class T>
		[[nodiscard]]
		Ref<T> FindSubMeshAt(int index) const;

		template <class T>
		[[nodiscard]]
		Ref<T> FindSubMeshByName(std::string_view name) const;

		// Doesn't find it, return -1.
		[[nodiscard]]
		std::optional<int32_t> FindIndexByName(std::string_view name) const;

		[[nodiscard]]
		std::optional<uint32_t> ObtainMaterialIndex(std::string_view name) const;

		[[nodiscard]]
		Ref<Material> FindMaterialAt(int index) const;

		[[nodiscard]]
		Ref<Material> FindMaterialByName(std::string_view name) const;

		//
		// Setter
		//
		void SetRoot(const Ref<Mesh>& root) { _root = root;}

		//
		// Getter
		//
		[[nodiscard]]
		Ref<Mesh> GetRoot() const { return _root.lock(); }

		[[nodiscard]]
		const String& GetNodeName() const { return _nodeName; }

		[[nodiscard]]
		const std::vector<Ref<Mesh>>& GetSubMeshes() const { return _subMeshes; }

		[[nodiscard]]
		const std::vector<Ref<Material>>& GetMaterials() const { return _materials; }

		[[nodiscard]]
		const Ref<Material>& GetMainMaterial() const { return _materials.front(); }

		[[nodiscard]]
		const BoundingBox& GetAllVerticesBoundingBox() const { return _allVerticesBoundingBox; }

		[[nodiscard]]
		const BoundingBox& GetBoundingBox() const { return _boundingBox; }

		[[nodiscard]]
		const MeshResource::SystemUnit& GetSystemUnit() const { return _systemUnit; }

		[[nodiscard]]
		AssetType GetAssetType() const override { return AssetType::Mesh; }

		[[nodiscard]]
		MeshID GetMeshID() const { return _id; }

		[[nodiscard]]
		const Ref<MeshResource::SceneTree>& GetSceneTree() const { return _sceneTree; }

		[[nodiscard]]
		size_t GetSubMeshCount() const { return _subMeshes.size(); }

		[[nodiscard]]
		size_t GetMaterialCount() const { return _materials.size(); }

		// Must be overriden.
		[[nodiscard]]
		virtual const char* GetMeshType() const { DENTY_ASSERT(false, "Forgot to override? (in Mesh::GetMeshType)"); return ""; }

		[[nodiscard]]
		bool HasSubMeshes() const { return (not _subMeshes.empty()); }

		[[nodiscard]]
		bool HasMaterial() const { return (not _materials.empty()); }

		[[nodiscard]]
		bool HasMaterial(std::string_view name) const;

		[[nodiscard]]
		bool SubMesh() const { return _subMesh; }

		[[nodiscard]]
		bool ParentNode() const { return _parentNode; }

		[[nodiscard]]
		bool UseDefaultMaterial() const { return _useDefaultMaterial; }

		//
		// Static functions.
		//
		[[nodiscard]]
		static Ref<Texture2D> GetAssetTexture() { return _meshAssetTexture; }

		static void LoadAssetTexture();
	protected:
		virtual void CopyOf(const Mesh* source);

	protected:
		std::weak_ptr<Mesh> _root;

		String _nodeName;

		// For serialize.
		FilePath _sceneTreeFilePath;
		Ref<MeshResource::SceneTree> _sceneTree;

		// For serialize.
		std::vector<FilePath> _materialFilePaths;

		// Usually use fbx or gltf mesh.
		std::vector<Ref<Mesh>> _subMeshes;
		std::vector<Ref<Material>> _materials;

		Ref<DXVertexBuffer> _vertexBuffer;

		// Node parent has this bounding box.
		// children are not use.
		BoundingBox _allVerticesBoundingBox;

		// Enclose each mesh.
		BoundingBox _boundingBox;

		MeshResource::SystemUnit _systemUnit;

		MeshID _id;

		bool _subMesh;
		bool _createOnlyMeshes;
		bool _parentNode;
		bool _useDefaultMaterial;
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
					CEREAL_NVP(_nodeName),
					CEREAL_NVP(_subMeshes),
					CEREAL_NVP(_sceneTreeFilePath),
					CEREAL_NVP(_materialFilePaths),
					CEREAL_NVP(_allVerticesBoundingBox),
					CEREAL_NVP(_boundingBox),
					CEREAL_NVP(_systemUnit),
					CEREAL_NVP(_id),
					CEREAL_NVP(_subMesh),
					CEREAL_NVP(_createOnlyMeshes),
					CEREAL_NVP(_parentNode),
					CEREAL_NVP(_useDefaultMaterial)
			    );
			}
			
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
			    archive(
					cereal::base_class<Object>(this),
					cereal::base_class<Asset>(this),
					CEREAL_NVP(_nodeName),
					CEREAL_NVP(_subMeshes),
					CEREAL_NVP(_sceneTreeFilePath),
					CEREAL_NVP(_materialFilePaths),
					CEREAL_NVP(_allVerticesBoundingBox),
					CEREAL_NVP(_boundingBox),
					CEREAL_NVP(_systemUnit),
					CEREAL_NVP(_id),
					CEREAL_NVP(_subMesh),
					CEREAL_NVP(_createOnlyMeshes),
					CEREAL_NVP(_parentNode),
					CEREAL_NVP(_useDefaultMaterial)
			    );
			}
		}
	private:
		static Ref<Texture2D> _meshAssetTexture; 
	};
};

CEREAL_CLASS_VERSION(DentyEngine::Mesh, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::Mesh)
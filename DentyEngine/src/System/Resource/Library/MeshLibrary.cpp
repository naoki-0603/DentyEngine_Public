#include "stdafx.h"

#include "MeshLibrary.h"

#include "Editor/Parts/EditorContentBrowser.h"

#include "Graphics/Mesh/Mesh.h"

#include "System/Events/EventScene.h"

#include "System/Serializer/SerializeExtensions.h"

#include "LibraryManager.h"

#include "MaterialLibrary.h"

#include "Graphics/Mesh/MeshImporter.h"

#include "Graphics/Mesh/StaticMesh.h"
#include "Graphics/Mesh/SkinnedMesh.h"

#include "Physics/ApproximationCollisionMesh.h"
#include "Physics/CollisionMesh.h"

#include "System/Events/EventEditor.h"

namespace DentyEngine
{
	const std::array<std::string, MeshLibrary::SUPPORTED_FILE_FORMAT_COUNT> MeshLibrary::SUPPORTED_FILE_FORMATS =
	{
		".collada", ".x", ".stp", ".obj", ".stl", ".ply", ".3ds",
		".gltf2", ".glb2", ".glb2", ".gltf", ".glb", ".assbin", ".assxml",
		".x3d", ".fbx", ".m3d", ".3mf", ".pbrt", ".assjson"
	};

	const std::string MeshLibrary::BOX_FILE_PATH = "Assets\\Meshes\\0_obj\\BuildIn\\Box_Static.DentyMesh";
	const std::string MeshLibrary::SPHERE_FILE_PATH = "Assets\\Meshes\\0_obj\\BuildIn\\Sphere_Static.DentyMesh";
	const std::string MeshLibrary::CAPSULE_FILE_PATH = "Assets\\Meshes\\0_obj\\BuildIn\\Capsule_Static.DentyMesh";
	const std::string MeshLibrary::PLANE_FILE_PATH = "Assets\\Meshes\\0_obj\\BuildIn\\Plane_Static.DentyMesh";

	MeshLibrary::MeshLibrary() :
		ILibrary(), _caches()
	{
	}

	void MeshLibrary::Initialize()
	{
		auto path = EditorContentBrowser::ENTRY_FILE_PATH;

		auto contentSerializer = [this](auto self, FilePath& folderPath) -> void
		{
			for (auto& content : std::filesystem::directory_iterator(folderPath.Get()))
			{
				const auto& entryPath = content.path();
				const std::string entry = entryPath.filename().string();

				if (content.is_directory())
				{
					self(self, folderPath /= entry);
					folderPath = folderPath.ParentPath();
				}
				else
				{
					if (!IsSupportedFormat(entryPath))
					{
						if (IsSerializedFile(entryPath))
						{
							if (entryPath.extension() == COLLISION_MESH_FILE_EXTENSION)
							{
								const auto collisionMesh = std::make_shared<CollisionMesh>();
								collisionMesh->Load(entryPath);

								Add(collisionMesh);
							}
							else if (entryPath.extension() == APPROXIMATION_COLLISION_MESH_FILE_EXTENSION)
							{
								const auto approximationCollisionMesh = std::make_shared<ApproximationCollisionMesh>();
								approximationCollisionMesh->Load(entryPath);

								Add(approximationCollisionMesh);
							}
						}

						continue;
					}

					bool skinned = false;

					// Load from binary file.
					if (IsSerializedFileExists(entryPath, skinned))
					{
						if (skinned)
						{
							const auto skinnedMesh = std::make_shared<SkinnedMesh>();
							skinnedMesh->Load(ConvertToSkinnedMeshPath(entryPath).Get());

							Add(skinnedMesh);
						}
						else
						{
							const auto staticMesh = std::make_shared<StaticMesh>();
							staticMesh->Load(ConvertToStaticMeshPath(entryPath).Get());

							Add(staticMesh);
						}
					}
					// Load use mesh importer.
					else 
					{
						// Import
						MeshImporter importer = {};
						const auto mesh = importer.Import(entryPath.string().c_str());

						// Save
						mesh->Save();

						// Bind scene tree meshes.
						importer.BindSceneTreeMeshes();

						// Add
						Add(mesh);
					}
				}
			}
		};
		contentSerializer(contentSerializer, path);
	}

	void MeshLibrary::Update()
	{
	}

	void MeshLibrary::OnGui()
	{
	}

	void MeshLibrary::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<EventOnSceneSaved>(BIND_EVENT_FUNCTION(OnSceneSaved));
		dispatcher.Dispatch<EventOnScenePlayButtonPressed>(BIND_EVENT_FUNCTION(OnScenePlayButtonPressed));
	}

	void MeshLibrary::Finalize()
	{
	}

	void MeshLibrary::OnSerialized() const
	{
	}

	void MeshLibrary::OnDeserialized()
	{
	}

	void MeshLibrary::Add(Ref<Mesh> mesh)
	{
		const auto& filePath = mesh->GetFilePathToSerialized();

		if (filePath.IsEmpty())
		{
			DENTY_ASSERT(false, "File path is empty! (in MeshLibrary::Add)");
		}

		_caches.try_emplace(filePath, mesh);
	}

	void MeshLibrary::Remove(const FilePath& filePath)
	{
		auto&& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			DENTY_SET_INFO_COLOR
			DENTY_INFO_LOG << "Succeed to remove " << filePath << " mesh from library!";

			_caches.erase(it);

			return;

		}

		DENTY_SET_ERR_COLOR
		DENTY_INFO_LOG << "Failed to remove " << filePath << " mesh from library!";
	}

    void MeshLibrary::AttachRootMeshes()
    {
		for (auto&& mesh : _caches | std::views::values)
		{
			for (auto&& subMesh : mesh->GetSubMeshes())
			{
				subMesh->SetRoot(mesh);
			}
		}
    }

    template <>
	std::optional<Ref<Mesh>> MeshLibrary::Find(const FilePath& filePath) const
	{
	    const auto& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			return it->second;
		}

		return std::nullopt;
	}

	template <>
	std::optional<Ref<StaticMesh>> MeshLibrary::Find(const FilePath& filePath) const
	{
	    const auto& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			return std::static_pointer_cast<StaticMesh>(it->second);
		}

		return std::nullopt;
	}

	template <>
	std::optional<Ref<SkinnedMesh>> MeshLibrary::Find(const FilePath& filePath) const
	{
	    const auto& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			return std::static_pointer_cast<SkinnedMesh>(it->second);

		}

		return std::nullopt;
	}

	template <>
	std::optional<Ref<CollisionMesh>> MeshLibrary::Find(const FilePath& filePath) const
	{
		const auto& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			return std::static_pointer_cast<CollisionMesh>(it->second);

		}

		return std::nullopt;
	}

	template <>
	std::optional<Ref<ApproximationCollisionMesh>> MeshLibrary::Find(const FilePath& filePath) const
	{
		const auto& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			return std::static_pointer_cast<ApproximationCollisionMesh>(it->second);
		}

		return std::nullopt;
	}

	bool MeshLibrary::Reload([[maybe_unused]] const FilePath& path)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return false;
	}

	bool MeshLibrary::Exists(const FilePath& filePath) const
	{
		const auto& it = _caches.find(filePath);

		return (it != _caches.end());
	}

	void MeshLibrary::OnMaterialEditorClosed([[maybe_unused]] EventOnMaterialEditorClosed* e)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)
	}

	void MeshLibrary::OnSceneSaved([[maybe_unused]] EventOnSceneSaved* e)
	{
	}

	void MeshLibrary::OnScenePlayButtonPressed([[maybe_unused]] EventOnScenePlayButtonPressed* e)
	{
	}

	FilePath MeshLibrary::ConvertToStaticMeshPath(const std::filesystem::path& entryPath) const
	{
		String staticMeshFilePath = entryPath.string();
		staticMeshFilePath.RemoveExtension();
		staticMeshFilePath.Append("_Static");
		staticMeshFilePath.Append(MESH_FILE_EXTENSION);

		return String::ToPath(staticMeshFilePath);
	}

	FilePath MeshLibrary::ConvertToSkinnedMeshPath(const std::filesystem::path& entryPath) const
	{
		String skinnedMeshFilePath = entryPath.string();
		skinnedMeshFilePath.RemoveExtension();
		skinnedMeshFilePath.Append("_Skinned");
		skinnedMeshFilePath.Append(MESH_FILE_EXTENSION);

		return String::ToPath(skinnedMeshFilePath);
	}

	bool MeshLibrary::IsSupportedFormat(const std::filesystem::path& entryPath) const
	{
		// Check is supported format.
		for (int i = 0; i < SUPPORTED_FILE_FORMAT_COUNT; ++i)
		{
			const auto& format = SUPPORTED_FILE_FORMATS.at(i);

			// If supported format, return true.
			if (format == entryPath.extension().string())
			{
				return true;
			}
		}

		// Not supported.
		return false;
	}

	bool MeshLibrary::IsSerializedFileExists(const std::filesystem::path& entryPath, bool& skinned) const
	{
		bool isFileExists = false;

		// Static
		{
			isFileExists = ConvertToStaticMeshPath(entryPath).IsValid();

			if (isFileExists)
			{
				skinned = false;

				return true;
			}
		}
		
		// Skinned
		{
			isFileExists = ConvertToSkinnedMeshPath(entryPath).IsValid();

			if (isFileExists)
			{
				skinned = true;

				return true;
			}
		}

		return false;
	}

	bool MeshLibrary::IsSerializedFile(const std::filesystem::path& entryPath) const
	{
		if (entryPath.extension() == MESH_FILE_EXTENSION)
			return true;

		if (entryPath.extension() == COLLISION_MESH_FILE_EXTENSION)
			return true;

		if (entryPath.extension() == APPROXIMATION_COLLISION_MESH_FILE_EXTENSION)
			return true;

		return false;
	}
}

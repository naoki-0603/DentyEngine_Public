#pragma once

#include "Editor/Graphics/Rendering/PreviewCamera.h"

#include "System/Resource/Prefab.h"

namespace DentyEngine
{
	class ColorBuffer;
	class FrameBuffer;

	class Mesh;
	class Material;

	class AssetPhotoTaker final
	{
	public:
		AssetPhotoTaker();
		~AssetPhotoTaker() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(AssetPhotoTaker)

		void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

		Ref<ColorBuffer> TakeMeshPhoto(Mesh* mesh);
		Ref<ColorBuffer> TakeMaterialPhoto(Material* material);
		Ref<ColorBuffer> TakePrefabPhoto(Prefab* prefab);
	public:
		static const Vector2 BUFFER_SIZE;
	private:
		void Begin(ID3D11DeviceContext* context);
		void End(ID3D11DeviceContext* context);

		// Setup camera.
		void Setup(ID3D11DeviceContext* context, const Mesh* mesh, const Vector3& direction = Vector3(1, 1, 1));

		/*[[nodiscard]]
		std::optional<std::string> ObtainMeshFilePathFromComponents(const Prefab::Info& info) const;*/
	private:
		Scope<PreviewCamera> _camera;

		Ref<FrameBuffer> _frameBuffer;
	};
}

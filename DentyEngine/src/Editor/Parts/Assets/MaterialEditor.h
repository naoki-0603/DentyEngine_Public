#pragma once

#include "../IEditor.h"

#include "AssetEditor.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

namespace DentyEngine
{
	enum class MaterialEditorSelectTexture : int32_t
	{
		None = -1,
		Albedo = 0,
		Metalness = 1,
		Normal = 2,
		Roughness = 3,
		Emissive = 4,
		AmbientOcclusion = 5,
		OptionalSlot1 = 6,
		OptionalSlot2 = 7,

		Max
	};

	class Material;
	class EventOnMaterialEditorOpened;
	class EventOnMaterialEditorClosed;

	class Texture2D;

	class MaterialEditor final : public IEditor, public AssetEditor
	{
	public:
		MaterialEditor();
		~MaterialEditor() override = default;

		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;

		void OnOpened(EventOnMaterialEditorOpened* e);
		void OnClosed(EventOnMaterialEditorClosed* e);

		[[nodiscard]]
		bool AssetExists() const override { return (_material != nullptr); }
	private:
		//
		// Callback functions
		//
		void TextureAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
	private:
		Ref<Material> _oldMaterial;
		Ref<Material> _material;

		Ref<Texture2D> _textureEmptyRef;
		Ref<Texture2D> _textureXMarkRef;

		std::string _renderingMode;

		MaterialEditorSelectTexture _selectTexture;
		bool _isOpenSelectTextureWindow;
	private:
		static const Vector2 BUTTON_SIZE;
		static const Vector2 THUMBNAIL_SIZE;
		static const Vector2 CROSS_MARK_SIZE;
	};
}

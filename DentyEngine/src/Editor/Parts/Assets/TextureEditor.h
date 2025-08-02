#pragma once

#include "AssetEditor.h"
#include "Editor/Parts/IEditor.h"

namespace DentyEngine
{
	class EventOnTextureEditorOpened;
	class EventOnTextureEditorClosed;

	class Texture;

	class TextureEditor final : public IEditor, public AssetEditor
	{
	public:
		TextureEditor();
		~TextureEditor() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(TextureEditor)

		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;

		void OnOpened(EventOnTextureEditorOpened* e);
		void OnClosed(EventOnTextureEditorClosed* e);

		[[nodiscard]]
		bool AssetExists() const override { return (_texture != nullptr); }
	private:
		Ref<Texture> _texture;
	};
}

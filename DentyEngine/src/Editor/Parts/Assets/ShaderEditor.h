#pragma once

#include "AssetEditor.h"
#include "../IEditor.h"

namespace DentyEngine
{
	class EventOnShaderEditorOpened;
	class EventOnShaderEditorClosed;

	class Shader;

	class ShaderEditor final : public IEditor, public AssetEditor
	{
	public:
		ShaderEditor();
		~ShaderEditor() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ShaderEditor)

		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;

		void OnOpened(EventOnShaderEditorOpened* e);
		void OnClosed(EventOnShaderEditorClosed* e);

		[[nodiscard]]
		bool AssetExists() const override { return (_shader != nullptr); }
	private:
		Ref<Shader> _shader;
	};
}

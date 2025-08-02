#pragma once

#include "IEditor.h"

#include "Graphics/Renderer/Texture2D.h"

namespace DentyEngine
{
	class EditorToolBar final : public IEditor
	{
	public:
		EditorToolBar(ID3D11Device* device);
		~EditorToolBar() override = default;

		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;
	private:
		Ref<Texture2D> _playButton;
		Ref<Texture2D> _pauseButton;
		Ref<Texture2D> _stepButton;
		Ref<Texture2D> _resumeButton;

		// For debugging.
		Vector2 _buttonSize;
		float _offsetFromStart;
		float _gapBetweenButton;

		bool _open;
	};
};
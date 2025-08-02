//#pragma once
//
//#include "EditorWindow.h"
//
//#include "Editor/Parts/IEditor.h"
//
//#include "Graphics/Buffer/FrameBuffer.h"
//
//namespace DentyEngine
//{
//	class Camera;
//
//	class Mesh;
//	class PostEffect;
//
//	class EditorPostEffectWindow final : public EditorWindow, public IEditor
//	{
//	public:
//		struct PreviewWindowInfo
//		{
//		public:
//			PreviewWindowInfo() :
//				planeMesh(), sphereMesh(), sceneBuffer(),
//				camera(), screenSize(512, 512), previousScreenSize(screenSize) {}
//
//		public:
//			Ref<StaticMesh> planeMesh;
//			Ref<StaticMesh> sphereMesh;
//
//			Ref<FrameBuffer> sceneBuffer;
//			Scope<Camera> camera;
//
//			Vector2 screenSize;
//			Vector2 previousScreenSize;
//		};
//
//		struct ParameterWindowInfo
//		{
//		public:
//			ParameterWindowInfo() : postEffects(), listShowed() { }
//
//			std::vector<Ref<PostEffect>> postEffects;
//
//			bool listShowed;
//		};
//	public:
//		EditorPostEffectWindow();
//		~EditorPostEffectWindow() override = default;
//
//		void Update();
//		void OnEvent(Event* e) override;
//		void OnGui(Scene* activeScene) override;
//
//		//
//		// Event functions.
//		//
//	private:
//		void OnOpened(Event* e) override;
//		void OnClosed(Event* e) override;
//		
//	private:
//		//
//		// Private functions.
//		//
//		void GenerateScene();
//		void SceneRender();
//
//		void RemoveOnGui();
//		void SwapOnGui();
//
//		void AddPostEffectOnGui();
//
//		// Make unique name.
//		// If doesn't make it, return "" string.
//		std::string MakeUniqueName(std::string_view targetName) const;
//	private:
//		//
//		// Non static variables.
//		//
//		PreviewWindowInfo _previewWindowInfo;
//		ParameterWindowInfo _parameterWindowInfo;
//
//		bool _pressedAddPostEffectButton;
//		bool _generatedScene;
//
//		//
//		// Static variables.
//		//
//		static const int MAKE_UNIQUE_NAME_TRIAL_COUNT_MAX;
//	};
//}

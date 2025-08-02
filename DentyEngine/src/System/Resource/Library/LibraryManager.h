#pragma once

#include "System/Core/IManager.h"

namespace DentyEngine
{
	class ScriptLibrary;
	class TextureLibrary;
	class ShaderLibrary;

	class SceneTreeLibrary;

	class SkeletonLibrary;

	class MeshLibrary;
	class MaterialLibrary;

	//class SpriteLibrary;

	class PrefabLibrary;

	class PreviewImageLibrary;

	class AnimationClipLibrary;
	class AnimatorControllerLibrary;

	class SkyboxLibrary;
	class SceneLibrary;

	class Texture;
	class LibraryManager final : public IManager
	{
	public:
		LibraryManager();
		~LibraryManager() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(LibraryManager)

		void Initialize() ;
		void Update() ;
		void OnEvent(Event* e) ;
		void OnGui() override;
		void Finalize() ;

		//
		// Getter
		//
		[[nodiscard]]
		TextureLibrary* GetTextureLibrary() const { return _textureLibrary.get();  }

		[[nodiscard]]
		ShaderLibrary* GetShaderLibrary() const { return _shaderLibrary.get(); }

		[[nodiscard]]
		SceneTreeLibrary* GetSceneTreeLibrary() const { return _sceneTreeLibrary.get(); }

		[[nodiscard]]
		SkeletonLibrary* GetSkeletonLibrary() const { return _skeletonLibrary.get(); }

		[[nodiscard]]
		MeshLibrary* GetMeshLibrary() const { return _meshLibrary.get(); }

		[[nodiscard]]
		MaterialLibrary* GetMaterialLibrary() const { return _materialLibrary.get(); }

		//[[nodiscard]]
		//SpriteLibrary* GetSpriteLibrary() const { return _spriteLibrary.get(); }

		[[nodiscard]]
		PrefabLibrary* GetPrefabLibrary() const { return _prefabLibrary.get(); }

		[[nodiscard]]
		AnimationClipLibrary* GetAnimationClipLibrary() const { return _animationClipLibrary.get(); }

		[[nodiscard]]
		AnimatorControllerLibrary* GetAnimatorControllerLibrary() const { return _animatorControllerLibrary.get(); }

		[[nodiscard]]
		SkyboxLibrary* GetSkyboxLibrary() const { return _skyboxLibrary.get(); }

		[[nodiscard]]
		ScriptLibrary* GetScriptLibrary() const { return _scriptLibrary.get(); }

		[[nodiscard]]
		SceneLibrary* GetSceneLibrary() const { return _sceneLibrary.get(); }

		//
		// Static function.
		//
		static LibraryManager& GetInstance();
	private:
		Scope<TextureLibrary> _textureLibrary;
		Scope<ShaderLibrary> _shaderLibrary;

		Scope<SceneTreeLibrary> _sceneTreeLibrary;

		Scope<SkeletonLibrary> _skeletonLibrary;

		Scope<MeshLibrary> _meshLibrary;
		Scope<MaterialLibrary> _materialLibrary;

		//Scope<SpriteLibrary> _spriteLibrary;

		Scope<PrefabLibrary> _prefabLibrary;

		Scope<AnimationClipLibrary> _animationClipLibrary;
		Scope<AnimatorControllerLibrary> _animatorControllerLibrary;

		Scope<SkyboxLibrary> _skyboxLibrary;

		Scope<ScriptLibrary> _scriptLibrary;

		Scope<SceneLibrary> _sceneLibrary;
	};
};
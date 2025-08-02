#include "stdafx.h"

#include "LibraryManager.h"

#include "MaterialLibrary.h"
#include "TextureLibrary.h"
#include "ShaderLibrary.h"
#include "MeshLibrary.h"
#include "PrefabLibrary.h"
#include "SceneLibrary.h"

#include "AnimatorControllerLibrary.h"
#include "AnimationClipLibrary.h"

#include "SceneTreeLibrary.h"
#include "SkeletonLibrary.h"
#include "SkyboxLibrary.h"
#include "ScriptLibrary.h"
//#include "SpriteLibrary.h"

#include "System/Events/EventScene.h"
#include "System/Events/EventEditor.h"

namespace DentyEngine
{
	LibraryManager::LibraryManager() :
		_textureLibrary(std::make_unique<TextureLibrary>()),
	    _shaderLibrary(std::make_unique<ShaderLibrary>()),
		_sceneTreeLibrary(std::make_unique<SceneTreeLibrary>()),
		_skeletonLibrary(std::make_unique<SkeletonLibrary>()),
		_meshLibrary(std::make_unique<MeshLibrary>()),
		_materialLibrary(std::make_unique<MaterialLibrary>()),
	    //_spriteLibrary(std::make_unique<SpriteLibrary>()),
		_prefabLibrary(std::make_unique<PrefabLibrary>()),
		_animationClipLibrary(std::make_unique<AnimationClipLibrary>()),
		_animatorControllerLibrary(std::make_unique<AnimatorControllerLibrary>()),
		_skyboxLibrary(std::make_unique<SkyboxLibrary>()),
		_scriptLibrary(std::make_unique<ScriptLibrary>()),
		_sceneLibrary(std::make_unique<SceneLibrary>())
	{
	}

	void LibraryManager::Initialize()
	{
		_shaderLibrary->Initialize();

		_sceneTreeLibrary->Initialize();

		_skeletonLibrary->Initialize();

		_textureLibrary->Initialize();

		_materialLibrary->Initialize();

		//_spriteLibrary->Initialize();

		_meshLibrary->Initialize();

		_sceneTreeLibrary->GenerateSceneTreesMeshes();
		_meshLibrary->AttachRootMeshes();

		_prefabLibrary->Initialize();

		_animationClipLibrary->Initialize();
		_animatorControllerLibrary->Initialize();

		_skyboxLibrary->Initialize();

		_scriptLibrary->Initialize();

		_sceneLibrary->Initialize();
	}

	void LibraryManager::Update()
	{
		_shaderLibrary->Update();

		_sceneTreeLibrary->Update();

		_skeletonLibrary->Update();

		_textureLibrary->Update();

		_materialLibrary->Update();

		//_spriteLibrary->Update();

		_meshLibrary->Update();

		_prefabLibrary->Update();

		_animationClipLibrary->Update();

		_animatorControllerLibrary->Update();

		_skyboxLibrary->Update();

		_scriptLibrary->Update();

	    _sceneLibrary->Update();
	}

	void LibraryManager::OnEvent(Event* e)
	{
		_shaderLibrary->OnEvent(e);

		_sceneTreeLibrary->OnEvent(e);

		_skeletonLibrary->OnEvent(e);
		
		_textureLibrary->OnEvent(e);

		_materialLibrary->OnEvent(e);

		//_spriteLibrary->OnEvent(e);

		_meshLibrary->OnEvent(e);

		_prefabLibrary->OnEvent(e);

		_animationClipLibrary->OnEvent(e);

		_animatorControllerLibrary->OnEvent(e);

		_skyboxLibrary->OnEvent(e);

		_scriptLibrary->OnEvent(e);

	    _sceneLibrary->OnEvent(e);
	}

	void LibraryManager::OnGui()
	{
		_shaderLibrary->OnGui();

		_sceneTreeLibrary->OnGui();

		_skeletonLibrary->OnGui();

		_textureLibrary->OnGui();

		_materialLibrary->OnGui();

		//_spriteLibrary->OnGui();

		_meshLibrary->OnGui();

		_prefabLibrary->OnGui();

		_animationClipLibrary->OnGui();

		_animatorControllerLibrary->OnGui();

		_skyboxLibrary->OnGui();

		_scriptLibrary->OnGui();

	    _sceneLibrary->OnGui();
	}

	void LibraryManager::Finalize()
	{
		_meshLibrary->Finalize();

		_sceneTreeLibrary->Finalize();

		_skeletonLibrary->Finalize();

		_materialLibrary->Finalize();

		//_spriteLibrary->Finalize();

		_textureLibrary->Finalize();

		_shaderLibrary->Finalize();

		_prefabLibrary->Finalize();

		_animationClipLibrary->Finalize();

		_animatorControllerLibrary->Finalize();

		_skyboxLibrary->Finalize();

		_scriptLibrary->Finalize();

	    _sceneLibrary->Finalize();
	}

	LibraryManager& LibraryManager::GetInstance()
	{
		static LibraryManager instance;
		return instance;
	}
};
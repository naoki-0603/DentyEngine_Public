#include "stdafx.h"

#include "PostEffect.h"

#include "FullScreenQuad.h"
#include "Graphics/Buffer/FrameBuffer.h"
#include "System/Graphics/GraphicsManager.h"
#include "System/Resource/Library/LibraryManager.h"

#include "System/Resource/Library/ShaderLibrary.h"

namespace DentyEngine
{
	PostEffect::PostEffect() :
		Object(), _sceneBuffer(), _resultBuffer(), _fullScreenQuad(), _fullScreenQuadShader(), _hasCreated()
	{
		_name = "PostEffect";
	}

	void PostEffect::Create(Ref<SceneBuffer> sceneBuffer, const Vector2& screenSize, BlendState::Type blendMode, [[maybe_unused]] bool useDefaultBlendMode)
	{
		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

		const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

		// Create full screen quad.
		{
			_fullScreenQuad = std::make_shared<FullScreenQuad>();
		}

		{
			_resultBuffer = std::make_unique<FrameBuffer>();
			_resultBuffer->Create(device, screenSize, blendMode);
			_fullScreenQuadShader = shaderLibrary->Find(DENTY_FILE_PATH(ShaderLibrary::FULL_SCREEN_QUAD_PATH)).value();
		}

		_sceneBuffer = sceneBuffer;

		_hasCreated = true;
	}
}

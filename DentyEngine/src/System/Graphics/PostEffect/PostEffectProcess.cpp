#include "stdafx.h"

#include "PostEffectProcess.h"

#include "Graphics/PostEffect/PostEffect.h"

namespace DentyEngine
{
	PostEffectProcess::PostEffectProcess(const PostEffects& postEffects, const Vector2& screenSize) :
		_postEffects(postEffects), _screenSize(screenSize)
	{
	}

	void PostEffectProcess::OnPostProcessRender()
	{
		for (const auto& postEffect : _postEffects)
		{
			postEffect->Process();
		}
	}
}

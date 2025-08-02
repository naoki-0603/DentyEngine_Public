#include "stdafx.h"

#include "AnimationState.h"

#include "System/Resource/Animations/AnimationClip.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/AnimationClipLibrary.h"

namespace DentyEngine
{
	AnimationState::AnimationState() :
		BaseState(), _clip(), _clipFilePath(), _oneShot()
	{
	}

    AnimationState::AnimationState(std::string_view name, Ref<AnimatorController> parent) :
        BaseState(name, parent), _clip(), _clipFilePath(), _oneShot()
    {
    }

    void AnimationState::OnGui()
    {
        BaseState::OnGui();

    }

    void AnimationState::OnSerialized() const
    {
	    BaseState::OnSerialized();
    }

    void AnimationState::OnDeserialized()
    {
        BaseState::OnDeserialized();

	    {
	        const AnimationClipLibrary* animationClipLibrary = LibraryManager::GetInstance().GetAnimationClipLibrary();

            const auto clip = animationClipLibrary->Find(_clipFilePath);
            if (clip.has_value())
            {
                _clip = clip.value();
            }
	    }
    }

    void AnimationState::SetAnimationClip(Ref<MeshResource::AnimationClip> clip)
    {
        _clip = clip;

        _clipFilePath = _clip->GetFilePathToSerialized();
    }
}

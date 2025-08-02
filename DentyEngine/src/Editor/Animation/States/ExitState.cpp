#include "stdafx.h"

#include "ExitState.h"

namespace DentyEngine
{
    ExitState::ExitState() :
        BaseState("ExitState", nullptr)
    {
    }

    ExitState::ExitState(Ref<AnimatorController> parent) :
		BaseState("ExitState", parent)
    {
    }

    void ExitState::OnGui()
    {
        BaseState::OnGui();

    }
}

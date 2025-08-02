#include "stdafx.h"

#include "AnyState.h"

namespace DentyEngine
{
    AnyState::AnyState() :
        BaseState("AnyState", nullptr)
    {
    }

    AnyState::AnyState(Ref<AnimatorController> parent) :
		BaseState("AnyState", parent)
    {
    }

    void AnyState::OnGui()
    {
        BaseState::OnGui();

    }
}

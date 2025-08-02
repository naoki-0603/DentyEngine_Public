#include "stdafx.h"

#include "EntryState.h"

namespace DentyEngine
{
    EntryState::EntryState() :
        BaseState("EntryState", nullptr)
    {
    }

    EntryState::EntryState(Ref<AnimatorController> parent) :
		BaseState("EntryState", parent)
    {
    }

    void EntryState::OnGui()
    {
        BaseState::OnGui();
    }
}

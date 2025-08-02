#pragma once

#include "EditorWindow.h"
#include "Editor/Parts/IEditor.h"

namespace DentyEngine
{
    class EditorProfilerWindow final : public EditorWindow, public IEditor
    {
    public:
        EditorProfilerWindow();
        ~EditorProfilerWindow() override = default;

        //
        // public functions.
        //
        void OnGui(Ref<Scene> activeScene) override;
        void OnEvent(Event* e) override;

        //
        // private functions.
        //
    private:
        void OnOpened(Event* e) override;
        void OnClosed(Event* e) override;

    private:
        float _textSize;
    };
}

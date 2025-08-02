#pragma once

namespace DentyEngine
{
    class Event;

    class EditorWindow
    {
    public:
        EditorWindow();
        virtual ~EditorWindow() = default;

        // Deserialize.
        virtual void OnOpened(Event* e) = 0;

        // Serialize.
        virtual void OnClosed(Event* e) = 0;

        // Getter
        bool IsOpen() const { return _open; }
    protected:
        bool _open;
    };
}
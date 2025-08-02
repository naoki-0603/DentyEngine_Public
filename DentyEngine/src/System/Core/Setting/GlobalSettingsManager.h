#pragma once

#include "CollisionFilterSetting.h"
#include "SceneSetting.h"

namespace DentyEngine
{
    struct GlobalSettings
	{
    public:
        GlobalSettings();
        ~GlobalSettings() = default;

    public:
        CollisionFilterSetting collisionFilterSetting;
    };

    class GlobalSettingsManager final
    {
    public:
        GlobalSettingsManager();
        ~GlobalSettingsManager() = default;

        static void Initialize();
        static void Update();
        static void Finalize();

        static void OnGui();

        static void OnEditorSettingsWindowOpened();
        static void OnEditorSettingsWindowClosed();

        [[nodiscard]]
        static CollisionFilterSetting& GetCollisionFilterSetting() { return _settings.collisionFilterSetting; }
    private:
        static GlobalSettings _settings;
    };
}

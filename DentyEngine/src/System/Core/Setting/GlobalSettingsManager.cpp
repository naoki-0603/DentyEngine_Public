#include "stdafx.h"

#include "GlobalSettingsManager.h"

namespace DentyEngine
{
    GlobalSettings::GlobalSettings() :
		collisionFilterSetting()
    {
    }

    GlobalSettings GlobalSettingsManager::_settings = {};

    GlobalSettingsManager::GlobalSettingsManager()
    {
    }

    void GlobalSettingsManager::Initialize()
    {

    }

    void GlobalSettingsManager::Update()
    {
    }

    void GlobalSettingsManager::Finalize()
    {
        //_collisionFilterSetting.Save(true);
    }

    void GlobalSettingsManager::OnGui()
    {
    }

    void GlobalSettingsManager::OnEditorSettingsWindowOpened()
    {
    }

    void GlobalSettingsManager::OnEditorSettingsWindowClosed()
    {
        //_collisionFilterSetting.Save(true);
    }
}

#include "stdafx.h"

#include "SceneSetting.h"

#include "Graphics/PostEffect/ShadowMap/VarianceShadowMap.h"
#include "Graphics/Renderer/LightCamera.h"

namespace DentyEngine
{
	SceneGizmos::SceneGizmos() :
		renderGrid(true)
	{
	}

    ScenePhysics::ScenePhysics() :
        gravity(0.0f, -9.8f, 0.0f), debugDrawerEnable(true)
    {
    }

    SceneShadow::Variance::Variance() :
		targetPosition(), viewSize(), near(), far()
    {
    }

    SceneShadow::Cascade::Cascade() :
        targetPosition(), viewSize(), near(), far(), cascadeAreaTable()
    {
    }

    SceneShadow::SceneShadow() :
		variance(), cascade()
    {
    }

    ScenePostEffects::ScenePostEffects() :
        enableType()
    {

    }

    SceneSetting::SceneSetting() :
        Setting(), physics(), shadow(), postEffects()
    {
        name = "SceneSettings";
    }

    SceneSetting::SceneSetting(const SceneSetting& source) :
        Setting(source)
    {
    }

    void SceneSetting::OnGui()
    {
    }

    void SceneSetting::Save([[maybe_unused]] bool isUpdate) const
    {
        //// If failed
        //if (!SceneSettingSerializer::SerializeAsJson(_filePathToSerialized.Get(), shared_from_this()))
        //{
        //    DENTY_SET_ERR_COLOR

        //    if (isUpdate)
        //    {
        //        DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " scene setting!";
        //    }
        //    else
        //    {
        //        DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " scene setting!";
        //    }

        //    return;
        //}

        //DENTY_SET_INFO_COLOR

        //if (isUpdate)
        //{
        //    DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " scene setting!";
        //}
        //else
        //{
        //    DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " scene setting!";
        //}
    }

    void SceneSetting::Load([[maybe_unused]] const std::filesystem::path& filePath)
    {
        //auto setting = std::make_shared<SceneSetting>();

        //// If failed
        //if (!SceneSettingSerializer::DeserializeFromJson(filePath, setting))
        //{
        //    DENTY_SET_ERR_COLOR
        //    DENTY_ERR_LOG << "Failed to load " << filePath << " scene setting!";

        //    return;
        //}

        //setting->Clone(shared_from_this());

        //DENTY_SET_INFO_COLOR
        //DENTY_INFO_LOG << "Succeed to load " << filePath << " scene setting!";
    }

    void SceneSetting::Clone([[maybe_unused]] const Ref<Setting>& setting)
    {
        /*DENTY_CHECK_CONDITION(setting->GetType() == std::string("SceneSetting"))

        Setting::Clone(setting);

        auto&& target = std::static_pointer_cast<SceneSetting>(setting);
        target->gizmos = gizmos;
        target->physics = physics;
        target->shadow = shadow;
        target->postEffects = postEffects;*/
    }

    void SceneSetting::OnBeforeSave()
    {
        // Variance Shadow
        {
            const LightCameraParameters& parameters = VarianceShadowMap::GetParameters().lightCamera->GetParameters();
            shadow.variance.viewSize = parameters.orthographic.viewSize;
            shadow.variance.far = parameters.orthographic.farZ;
            shadow.variance.near = parameters.orthographic.nearZ;
            shadow.variance.targetPosition = parameters.targetPosition;
        }

        // Cascade shadow
        {
            /*const LightCameraParameters& parameters = CascadeShadowMap::GetParameters().lightCamera->GetParameters();
            shadow.variance.viewSize = parameters.orthographic.viewSize;
            shadow.variance.far = parameters.orthographic.farZ;
            shadow.variance.near = parameters.orthographic.nearZ;
            shadow.variance.targetPosition = parameters.targetPosition;*/
        }
    }
}

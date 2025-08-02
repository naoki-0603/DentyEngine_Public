#pragma once

#include "Camera.h"

namespace DentyEngine
{
    struct ShadowCameraConstants
    {
    public:
        ShadowCameraConstants() :
    		viewProjections(), view(), projection(), cameraPosition(), bias() {}

        std::array<Matrix, 3u> viewProjections;
        Matrix view;
        Matrix projection;

        Vector3 cameraPosition;
        float bias;
    };

    class DirectionalLight;

    // Camera class for directional light shadow map.
    class ShadowCamera final : public Camera
    {
    public:
        ShadowCamera();
        ~ShadowCamera() override = default;

    	DELETE_COPY_MOVE_CONSTRUCTORS(ShadowCamera)

        void Create();
        void OnUpdate();
        void UpdateConstants(const std::array<Matrix, 3u>& lightViewProjections);

        void OnEvent(Event* e) override;

        void OnGui() override;

        void Bind(ID3D11DeviceContext* context);

        //
        // Setter
        //
        void SetDirectionalLight(Ref<DirectionalLight> directionalLight);
        void SetViewSize(float viewSize) { _viewSize = viewSize; }
        void SetDistance(float distance) { _distance = distance; }
        void SetNear(float near) { _near = near; }
        void SetFar(float far) { _far = far; }

        //
        // Getter
        //
        [[nodiscard]]
        float GetViewSize() const { return _viewSize; }

        [[nodiscard]]
        float GetDistance() const { return _distance; }

        [[nodiscard]]
        float GetNear() const { return _near; }

        [[nodiscard]]
        float GetFar() const { return _far; }

        [[nodiscard]]
        bool HasDirectionalLight() const { return (_directionalLight != nullptr); }
    private:
        ShadowCameraConstants _constants;

        Ref<DirectionalLight> _directionalLight;

        Vector3 _direction;
        Vector3 _eyePosition;

        float _viewSize;
        float _distance;
        float _near;
        float _far;
        float _bias;
    };
}

#pragma once

#include "Component/Transform.h"

namespace DentyEngine
{
    class Event;

    class EventBeginRenderingProcess;
    class EventEndRenderingProcess;

    class ConstantBuffer;

    class Camera
    {
    public:
        struct Constants
        {
        public:
            Constants() :
        		viewProjection(), view(),
        		projection(), inverseView(),
        		inverseProjection(), cameraPosition() {}

            Matrix viewProjection;
            Matrix view;
            Matrix projection;
            Matrix inverseView;
            Matrix inverseProjection;

            Vector4 cameraPosition;
        };

        struct Perspective
        {
            Perspective() : fovAngleY(), aspectRatio(), nearZ(), farZ(), distance(), up() {}
        public:
            // degree. (Converted to radian in CalculatePerspectiveProjection)
            float fovAngleY;
            float aspectRatio;
            float nearZ, farZ;
            float distance;

            Vector3 up;
        public:
            [[nodiscard]]
            float GetFovAngleRadian() const;

            [[nodiscard]]
            float GetFovAngle() const;

        private:
            friend class cereal::access;

            template <class Archive>
            void save(Archive& archive, uint32_t version) const
            {
                if (version <= 1u)
                {
                    archive(
                        CEREAL_NVP(fovAngleY),
                        CEREAL_NVP(aspectRatio),
                        CEREAL_NVP(nearZ),
                        CEREAL_NVP(farZ),
                        CEREAL_NVP(distance),
                        CEREAL_NVP(up)
                    );    
                }
                
            }

            template <class Archive>
            void load(Archive& archive, uint32_t version)
            {
                if (version <= 1u)
                {
                    archive(
                        CEREAL_NVP(fovAngleY),
                        CEREAL_NVP(aspectRatio),
                        CEREAL_NVP(nearZ),
                        CEREAL_NVP(farZ),
                        CEREAL_NVP(distance),
                        CEREAL_NVP(up)
                    );    
                }
            }
        };

        struct Orthographic
        {
            Orthographic() : viewSize(), nearZ(), farZ(), distance(), aspectRatio(), up() {}
        public:
            Vector2 viewSize;
            float nearZ, farZ;
            float distance;
            float aspectRatio;

            Vector3 up;
        private:
            friend class cereal::access;

            template <class Archive>
            void save(Archive& archive, uint32_t version) const
            {
                if (version <= 1u)
                {
                    archive(
                        CEREAL_NVP(viewSize),
                        CEREAL_NVP(nearZ),
                        CEREAL_NVP(farZ),
                        CEREAL_NVP(distance),
                        CEREAL_NVP(aspectRatio),
                        CEREAL_NVP(up)
                    );    
                }
                
            }

            template <class Archive>
            void load(Archive& archive, uint32_t version)
            {
                if (version <= 1u)
                {
                    archive(
                        CEREAL_NVP(viewSize),
                        CEREAL_NVP(nearZ),
                        CEREAL_NVP(farZ),
                        CEREAL_NVP(distance),
                        CEREAL_NVP(aspectRatio),
                        CEREAL_NVP(up)
                    );    
                }
                
            }
        };

        enum class ClearFlags
        {
            Skybox,
            SolidColor
        };

        enum class ProjectionType
        {
            Perspective,
            Orthographic
        };
    public:
        // For serialize
        Camera() = default;

        explicit Camera(ID3D11Device* device);

    	Camera(const Camera&);

    	virtual ~Camera() = default;

        virtual void OnEvent(Event* e) = 0;
        virtual void OnGui() = 0;

        void CalculateView(Matrix& world, const Vector3& eyePosition, const Vector3& focusPosition, const Vector3& up);

    	void CalculatePerspectiveProjection(const Perspective& perspective);
        void CalculateOrthographicProjection(const Orthographic& orthographic);

        void OnResize(const Vector2& size);
        void OnProjectionChanged(ProjectionType newType);

        [[nodiscard]]
        const Ref<ConstantBuffer>& GetConstantBuffer() const { return _constantBuffer; }

        [[nodiscard]]
        const Matrix& GetView() const { return _view; }

        [[nodiscard]]
        const Matrix& GetProjection() const { return _projection; }

        [[nodiscard]]
        Matrix GetInverseView() const { return _view.Inverse(); }

        [[nodiscard]]
        Matrix GetInverseProjection() const { return _projection.Inverse(); }

        [[nodiscard]]
        const Matrix& GetViewProjection() const { return _viewProjection; }

        [[nodiscard]]
        const Vector3& GetFocusPosition() const { return _focusPosition; }

        [[nodiscard]]
        Vector3 GetRight() const { return GetInverseView().GetRightVec3().Normalize(); }

        [[nodiscard]]
        Vector3 GetUp() const { return GetInverseView().GetUpVec3().Normalize(); }

        [[nodiscard]]
        Vector3 GetFront() const { return GetInverseView().GetFrontVec3().Normalize(); }

        [[nodiscard]]
        Vector3 GetPosition() const { return GetInverseView().GetOffsetVec3(); }

        [[nodiscard]]
        const Perspective& GetPerspective() const { return _perspective; }

        [[nodiscard]]
        const Orthographic& GetOrthographic() const { return _orthographic; }

        //
        // Static function.
        //

        // http://marupeke296.com/DXG_No59_TakeModelsInCamera.html
        [[nodiscard]]
        static Matrix CalculateViewMatrixTakingSphereIn(
            const Vector3& sphereCenter,
            float sphereRadius,
            float fovY,
            float aspectRatio,
            const Vector3& direction,
            const Vector3& up
        );
    protected:
        ProjectionType _projectionType;

        Perspective _perspective;
        Orthographic _orthographic;

        Matrix _view;
        Matrix _projection;
        Matrix _viewProjection;

        Vector3 _focusPosition;

        Ref<ConstantBuffer> _constantBuffer;
        Constants _constants;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(_projectionType),
                    CEREAL_NVP(_perspective),
                    CEREAL_NVP(_orthographic),
                    CEREAL_NVP(_view),
                    CEREAL_NVP(_projection),
                    CEREAL_NVP(_viewProjection),
                    CEREAL_NVP(_focusPosition)
                );    
            }
        }

        template <class Archive>
        void load(Archive& archive, uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(_projectionType),
                    CEREAL_NVP(_perspective),
                    CEREAL_NVP(_orthographic),
                    CEREAL_NVP(_view),
                    CEREAL_NVP(_projection),
                    CEREAL_NVP(_viewProjection),
                    CEREAL_NVP(_focusPosition)
                );    
            }
        }
    };
}

CEREAL_CLASS_VERSION(DentyEngine::Camera, 1u)
CEREAL_CLASS_VERSION(DentyEngine::Camera::Perspective, 1u)
CEREAL_CLASS_VERSION(DentyEngine::Camera::Orthographic, 1u)
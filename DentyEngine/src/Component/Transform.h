#pragma once

#include "Component.h"

namespace DentyEngine
{
    class RigidBody;

    // Class specification
    //--------------------------
    // �e�����Ȃ��ꍇ�� world local �͓����l�ɂȂ�
    // ��{�I�Ƀ��[�J���̒l�𑀍삷�邱�ƂɂȂ�
    // GUI�ł̓��[�J���̒l��\������
    //
	// position->���[���h��Ԃł̍��W
    // localPosition->�e�̋�Ԃ��猩�����W
    //
    // scale->���[���h��Ԃł̃X�P�[���l
    // localScale->�e�̋�Ԃł̃X�P�[���l
    //
    // eulerAngles->���[���h��Ԃł̉�]�l(�P�ʂ� ���W�A��)
    // localEulerAngles->�e�̋�Ԃł̉�]�l(�P�ʂ� ���W�A��)
    //
    // rotation->���[���h��Ԃł̎p��
    // localRotation->�e�̋�Ԃł̎p��
    //
    // world->���[���h��Ԃł̎p�� + �ʒu
    // local->�e�̋�Ԃł̎p�� + �ʒu
    // 
    // ���t���[�����Ƃ� false �ɐݒ肳���B
    // guizmoChanged->�}�j�s�����[�^�[�ŕύX���ꂽ�Ƃ��� true �ɂȂ�
    class Transform final : public Component, public std::enable_shared_from_this<Transform>
    {
        friend class ManipulateGizmo;
    public:
        Transform();
        Transform(const Transform&);
        ~Transform() override = default;

        void Awake() override;
        void Start() override;
        void OnUpdate([[maybe_unused]] const float deltaTime);
        void OnGui() override;
        void OnPrefabGui() override;

        void OnEvent(Event* e) override;
        void OnSerialized() const override;
        void OnDeserialized() override;

        void OnGameObjectDeserialized() override;

        //
        // Make copy of this.
        //
        [[nodiscard]]
    	Ref<Object> MakeCopy() const override;

        //
        // Converter
        //
        [[nodiscard]]
        Vector3 ConvertToLocal(const Vector3& positionInWorld) const;

        [[nodiscard]]
        Vector3 ConvertToLocalRecursive(const Vector3& positionInWorld) const;

        //
        // Setter
        //
        void SetRotation(const Vector3& paramEulerAngles);

        void SetUpdateEnabled() { _update = true; }
        void SetUpdateDisabled() { _update = false; }
        void SetUpdate(bool update) { _update = update; }

        [[nodiscard]]
        Vector3 CalculateDeltaAngles() const;

        //
        // Getter
        //
        [[nodiscard]]
        bool GetUpdate() const { return _update; }
    public:
        // Positions.

        // World space.
        Vector3 position;

        // Local space.
        Vector3 localPosition;

        // Scales.

        // World space.
        Vector3 scale;

        // Local space.
    	Vector3 localScale;

        // Rotations. (unit is radian)
        Vector3 eulerAngles;
        Vector3 localEulerAngles;

        Quaternion rotation;
        Quaternion localRotation;

        // Transform Local space and World space.
    	Matrix world;
        Matrix local;
    private:
        [[nodiscard]]
        bool IsRotationChanged() const;

    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 2u)
            {
                archive(
                    cereal::base_class<Component>(this),
                    CEREAL_NVP(position),
                    CEREAL_NVP(localPosition),
                    CEREAL_NVP(scale),
                    CEREAL_NVP(localScale),
                    CEREAL_NVP(localEulerAngles),
                    CEREAL_NVP(localRotation),
                    CEREAL_NVP(local),
                    CEREAL_NVP(world),
                    CEREAL_NVP(_update),
                    CEREAL_NVP(_disableParentYPosition)
                );
            }
            

            OnSerialized();
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Component>(this),
                    CEREAL_NVP(position),
                    CEREAL_NVP(localPosition),
                    CEREAL_NVP(scale),
                    CEREAL_NVP(localScale),
                    CEREAL_NVP(localEulerAngles),
                    CEREAL_NVP(localRotation),
                    CEREAL_NVP(local),
                    CEREAL_NVP(world),
                    CEREAL_NVP(_update)
                );
            }
            else if (version == 2u)
            {
                archive(
                    cereal::base_class<Component>(this),
                    CEREAL_NVP(position),
                    CEREAL_NVP(localPosition),
                    CEREAL_NVP(scale),
                    CEREAL_NVP(localScale),
                    CEREAL_NVP(localEulerAngles),
                    CEREAL_NVP(localRotation),
                    CEREAL_NVP(local),
                    CEREAL_NVP(world),
                    CEREAL_NVP(_update),
                    CEREAL_NVP(_disableParentYPosition)
                );
            }

            OnDeserialized();
        }

        Matrix CalculateParentRelation(GameObject* parent);
    private:
        Vector3 _oldPositionForCommand;
        Vector3 _oldScaleForCommand;
        Vector3 _oldEulerAnglesForCommand;

        Vector3 _oldEulerAngles;

        bool _inputRotation;
        bool _inputPosition;
        bool _inputScale;

        bool _disableParentYPosition;

    	bool _update;
    };

};

CEREAL_CLASS_VERSION(DentyEngine::Transform, 2u)

CEREAL_REGISTER_TYPE(DentyEngine::Transform)
#pragma once

#include "Component/Behaviour.h"

namespace DentyEngine
{
    //
    // Unused class. (2024/01/09)
    //
	class PhysicalMovement : public Object, public std::enable_shared_from_this<PhysicalMovement>
	{
	public:
		PhysicalMovement();
        PhysicalMovement(const PhysicalMovement&);
		~PhysicalMovement() override = default;

        virtual void OnUpdate(float deltaTime);

		void OnEvent(Event* e) override;

        virtual void OnGui();
        virtual void OnPrefabGui();

        [[nodiscard]]
        Ref<Object> MakeCopy() const override;

        //
        // Serialize/Deserialize functions.
        //
		void OnSerialized() const override;
		void OnDeserialized() override;

        void OnDestroy() override;

        //
        // Class member functions.
        //
        void AddForce(const Vector3& force);

        [[nodiscard]]
        float GetMass() const { return _mass; }

        [[nodiscard]]
        const Vector3& GetVelocity() const { return _velocity; }

        [[nodiscard]]
        const Vector3& GetPosition() const { return _position; }

        [[nodiscard]]
        static const Vector3& GetGravity() { return GRAVITY; }
	protected:
        // Call by OnUpdate
        virtual void Integrate(float deltaTime);

        virtual void Gravity(float deltaTime);
    protected:
        Vector3 _velocity;
        Vector3 _acceleration;
        Vector3 _position;

        float _mass;

        bool _gravity;

        // Doesn't serialize.
        Vector3 _resultant;
	protected:
        static constexpr float FRICTION = 10.0f;
	private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive) const
        {
            archive(
                cereal::base_class<Object>(this),
                CEREAL_NVP(_velocity),
                CEREAL_NVP(_acceleration),
                CEREAL_NVP(_mass),
                CEREAL_NVP(_gravity)
            );
        }

        template <class Archive>
        void load(Archive& archive)
        {
            archive(
                cereal::base_class<Object>(this),
                CEREAL_NVP(_velocity),
                CEREAL_NVP(_acceleration),
                CEREAL_NVP(_mass),
                CEREAL_NVP(_gravity)
            );
        }
	private:
        static Vector3 GRAVITY;
	};
};

CEREAL_REGISTER_TYPE(DentyEngine::PhysicalMovement)
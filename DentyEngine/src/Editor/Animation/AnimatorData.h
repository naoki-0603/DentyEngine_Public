#pragma once

namespace DentyEngine
{
    struct AnimatorValue
    {
    public:
        enum class Type : int32_t
        {
            None = -1,
            Int = 0,
            Float,
            Bool,
            Trigger,

            Max
        };
    public:
        AnimatorValue();
        ~AnimatorValue() = default;

        [[nodiscard]]
        std::string GetValueTypeAsString() const { return std::string(NAMEOF_ENUM(valueType)).c_str();}

        [[nodiscard]]
        std::optional<std::string> GetValueAsString() const;
    public:
        Type valueType;

        int32_t valueInt;
        float valueFloat;
        bool valueBool;
        bool valueTrigger;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            archive(
                CEREAL_NVP(valueType),
                CEREAL_NVP(valueInt),
                CEREAL_NVP(valueFloat),
                CEREAL_NVP(valueBool),
                CEREAL_NVP(valueTrigger)
            );
        }

        template <class Archive>
        void load(Archive& archive, [[maybe_unused]] const uint32_t version)
        {
            archive(
                CEREAL_NVP(valueType),
                CEREAL_NVP(valueInt),
                CEREAL_NVP(valueFloat),
                CEREAL_NVP(valueBool),
                CEREAL_NVP(valueTrigger)
            );
        }
    };

	struct AnimatorParameter
	{
	public:
		AnimatorParameter();
		~AnimatorParameter() = default;

        std::string name;
        AnimatorValue value;
	private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            archive(
                CEREAL_NVP(name),
                CEREAL_NVP(value)
            );
        }

        template <class Archive>
        void load(Archive& archive, [[maybe_unused]] const uint32_t version)
        {
            archive(
                CEREAL_NVP(name),
                CEREAL_NVP(value)
            );
        }
	};
}

CEREAL_CLASS_VERSION(DentyEngine::AnimatorParameter, 1u)
CEREAL_CLASS_VERSION(DentyEngine::AnimatorValue, 1u)
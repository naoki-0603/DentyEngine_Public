#pragma once

namespace DentyEngine
{
    class AnimatorControllerID final
    {
    public:
        AnimatorControllerID();
        AnimatorControllerID(const uint64_t id);
        AnimatorControllerID(const AnimatorControllerID& id) = default;
        ~AnimatorControllerID() = default;

        void ReCalculate();

        operator uint64_t() const { return _animatorControllerID; }
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            archive(
                CEREAL_NVP(_animatorControllerID)
            );
        }

        template <class Archive>
        void load(Archive& archive, [[maybe_unused]] const uint32_t version)
        {
            archive(
                CEREAL_NVP(_animatorControllerID)
            );
        }
    private:
        uint64_t _animatorControllerID;
    };
};

namespace std
{
    template <typename T> struct hash;

    template <>
    struct hash<DentyEngine::AnimatorControllerID>
    {
        std::size_t operator()(const DentyEngine::AnimatorControllerID& animatorControllerID) const
        {
            return static_cast<size_t>(animatorControllerID);
        }
    };
};

CEREAL_CLASS_VERSION(DentyEngine::AnimatorControllerID, 1u)
#pragma once

namespace DentyEngine
{
    class AnimationClipID final
    {
    public:
        AnimationClipID();
        AnimationClipID(const uint64_t id);
        AnimationClipID(const AnimationClipID& id) = default;
        ~AnimationClipID() = default;

        void ReCalculate();

        operator uint64_t() const { return _animationClipID; }
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            archive(
                CEREAL_NVP(_animationClipID)
            );
        }

        template <class Archive>
        void load(Archive& archive, [[maybe_unused]] const uint32_t version)
        {
            archive(
                CEREAL_NVP(_animationClipID)
            );
        }
    private:
        uint64_t _animationClipID;
    };
};

namespace std
{
    template <typename T> struct hash;

    template <>
    struct hash<DentyEngine::AnimationClipID>
    {
        std::size_t operator()(const DentyEngine::AnimationClipID& animationClipID) const
        {
            return static_cast<size_t>(animationClipID);
        }
    };
};

CEREAL_CLASS_VERSION(DentyEngine::AnimationClipID, 1u)
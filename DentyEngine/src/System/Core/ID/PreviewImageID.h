#pragma once

namespace DentyEngine
{
    class PreviewImageID final
    {
    public:
        PreviewImageID();
        PreviewImageID(const uint64_t id);
        PreviewImageID(const PreviewImageID&) = default;
        ~PreviewImageID() = default;

        void ReCalculate();

        operator uint64_t() const { return _id; }
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            archive(
                CEREAL_NVP(_id)
            );
        }

        template <class Archive>
        void load(Archive& archive, [[maybe_unused]] const uint32_t version)
        {
            archive(
                CEREAL_NVP(_id)
            );
        }
    private:
        uint64_t _id;
    };
}

namespace std
{
    template <typename T> struct hash;

    template <>
    struct hash<DentyEngine::PreviewImageID>
    {
        std::size_t operator()(const DentyEngine::PreviewImageID& id) const
        {
            return static_cast<size_t>(id);
        }
    };
};

CEREAL_CLASS_VERSION(DentyEngine::PreviewImageID, 1u)
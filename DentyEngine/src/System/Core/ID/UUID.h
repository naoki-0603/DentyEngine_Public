#pragma once

// From https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/UUID.h

namespace DentyEngine
{
    class UUID final
    {
    public:
        UUID();
        explicit UUID(const uint64_t uuid);
        UUID(const UUID& uuid) = default;
        ~UUID() = default;

        void ReCalculate();

        operator uint64_t() const { return _uuid; }
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            archive(
                CEREAL_NVP(_uuid)
            );
        }

        template <class Archive>
        void load(Archive& archive, [[maybe_unused]] const uint32_t version)
        {
            archive(
                CEREAL_NVP(_uuid)
            ); 
        }
    private:
        uint64_t _uuid;
    };
};

namespace std
{
    template <typename T> struct hash;

    template <>
    struct hash<DentyEngine::UUID>
    {
        std::size_t operator()(const DentyEngine::UUID& uuid) const
        {
            return static_cast<size_t>(uuid);
        }
    };
};

CEREAL_CLASS_VERSION(DentyEngine::UUID, 1u)

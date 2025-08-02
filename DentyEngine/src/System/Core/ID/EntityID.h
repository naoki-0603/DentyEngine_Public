#pragma once

namespace DentyEngine
{
    class EntityID final
    {
    public:
        EntityID();
        EntityID(const uint32_t id);
        EntityID(const EntityID& id) = default;
        ~EntityID() = default;

        void ReCalculate();

        operator uint32_t() const { return _entityID; }
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            archive(
                CEREAL_NVP(_entityID)
            );
        }

        template <class Archive>
        void load(Archive& archive, [[maybe_unused]] const uint32_t version)
        {
            archive(
                CEREAL_NVP(_entityID)
            );
        }
    private:
        uint32_t _entityID;
    };
};

namespace std
{
    template <typename T> struct hash;

    template <>
    struct hash<DentyEngine::EntityID>
    {
        std::size_t operator()(const DentyEngine::EntityID& entityID) const
        {
            return static_cast<size_t>(entityID);
        }
    };
};

CEREAL_CLASS_VERSION(DentyEngine::EntityID, 1u)
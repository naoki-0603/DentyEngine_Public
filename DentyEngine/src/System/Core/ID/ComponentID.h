#pragma once

namespace DentyEngine
{
    class ComponentID final
    {
    public:
        ComponentID();
        ComponentID(const uint64_t id);
        ComponentID(const ComponentID& id) = default;
        ~ComponentID() = default;

        void ReCalculate();

        operator uint64_t() const { return _componentID; }
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            archive(
                CEREAL_NVP(_componentID)
            );
        }

        template <class Archive>
        void load(Archive& archive, [[maybe_unused]] const uint32_t version)
        {
            archive(
                CEREAL_NVP(_componentID)
            );
        }
    private:
        uint64_t _componentID;
    };
};

namespace std
{
    template <typename T> struct hash;

    template <>
    struct hash<DentyEngine::ComponentID>
    {
        std::size_t operator()(const DentyEngine::ComponentID& componentID) const
        {
            return static_cast<size_t>(componentID);
        }
    };
};

CEREAL_CLASS_VERSION(DentyEngine::ComponentID, 1u)
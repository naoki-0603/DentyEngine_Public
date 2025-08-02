#pragma once

namespace DentyEngine
{
    struct CollisionFilter
    {
    public:
        CollisionFilter() : name(), group(), mask() {}
        CollisionFilter(const std::string& name, uint32_t group, uint32_t mask) :
            name(name), group(group), mask(mask) {}

        CollisionFilter(const CollisionFilter&) = default;
        ~CollisionFilter() = default;

    public:
        std::string name;

        uint32_t group;
        uint32_t mask;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(name),
                    CEREAL_NVP(group),
                    CEREAL_NVP(mask)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(name),
                    CEREAL_NVP(group),
                    CEREAL_NVP(mask)
                );
            }
        }
    };
}

CEREAL_CLASS_VERSION(DentyEngine::CollisionFilter, 1u)

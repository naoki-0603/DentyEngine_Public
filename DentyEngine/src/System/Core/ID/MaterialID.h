#pragma once

namespace DentyEngine
{
    class MaterialID final
    {
    public:
        MaterialID();
        MaterialID(const uint32_t id);
        MaterialID(const MaterialID& id) = default;
        ~MaterialID() = default;

        void ReCalculate();

        operator uint32_t() const { return _materialID; }
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            archive(
                CEREAL_NVP(_materialID)
            );
        }

        template <class Archive>
        void load(Archive& archive, [[maybe_unused]] const uint32_t version)
        {
            archive(
                CEREAL_NVP(_materialID)
            );
        }
    private:
        uint32_t _materialID;
    };
};

namespace std
{
    template <typename T> struct hash;

    template <>
    struct hash<DentyEngine::MaterialID>
    {
        std::size_t operator()(const DentyEngine::MaterialID& materialID) const
        {
            return static_cast<size_t>(materialID);
        }
    };
};

CEREAL_CLASS_VERSION(DentyEngine::MaterialID, 1u)
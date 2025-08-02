#pragma once

namespace DentyEngine
{
    class ShaderID final
    {
    public:
        ShaderID();
        ShaderID(const uint64_t id);
        ShaderID(const ShaderID& id) = default;
        ~ShaderID() = default;

        void ReCalculate();

        operator uint64_t() const { return _shaderID; }
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            archive(
                CEREAL_NVP(_shaderID)
            );
        }

        template <class Archive>
        void load(Archive& archive, [[maybe_unused]] const uint32_t version)
        {
            archive(
                CEREAL_NVP(_shaderID)
            );
        }
    private:
        uint64_t _shaderID;
    };
};

namespace std
{
    template <typename T> struct hash;

    template <>
    struct hash<DentyEngine::ShaderID>
    {
        std::size_t operator()(const DentyEngine::ShaderID& shaderID) const
        {
            return static_cast<size_t>(shaderID);
        }
    };
};

CEREAL_CLASS_VERSION(DentyEngine::ShaderID, 1u)
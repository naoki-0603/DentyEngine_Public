#pragma once

#include "Setting.h"

namespace DentyEngine
{
    struct SceneGizmos
    {
    public:
        SceneGizmos();

    public:
        bool renderGrid;

    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(renderGrid)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(renderGrid)
                );
            }
        }
    };

    struct ScenePhysics
    {
    public:
        ScenePhysics();
        ~ScenePhysics() = default;

    public:
        Vector3 gravity;

        bool debugDrawerEnable;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(gravity),
                    CEREAL_NVP(debugDrawerEnable)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(gravity),
                    CEREAL_NVP(debugDrawerEnable)
                );
            }
        }
    };

    struct SceneShadow
    {
    public:
        struct Variance
        {
        public:
            Variance();

        public:
            Vector3 targetPosition;

            float viewSize;
            float near;
            float far;
        private:
            friend class cereal::access;

            template <class Archive>
            void save(Archive& archive, const uint32_t version) const
            {
	            if (version <= 1u)
	            {
                    archive(
                        CEREAL_NVP(targetPosition),
                        CEREAL_NVP(viewSize),
                        CEREAL_NVP(near),
                        CEREAL_NVP(far)
                    );
	            }
            }

            template <class Archive>
            void load(Archive& archive, const uint32_t version)
            {
                if (version <= 1u)
                {
                    archive(
                        CEREAL_NVP(targetPosition),
                        CEREAL_NVP(viewSize),
                        CEREAL_NVP(near),
                        CEREAL_NVP(far)
                    );
                }
            }
        };

        struct Cascade
        {
        public:
            Cascade();

        public:
            Vector3 targetPosition;

            float viewSize;
            float near;
            float far;

            std::array<float, 3u> cascadeAreaTable;
        private:
            friend class cereal::access;

            template <class Archive>
            void save(Archive& archive, const uint32_t version) const
            {
                if (version <= 2u)
                {
                    archive(
                        CEREAL_NVP(targetPosition),
                        CEREAL_NVP(cascadeAreaTable)
                    );
                }
            }

            template <class Archive>
            void load(Archive& archive, const uint32_t version)
            {
                if (version <= 1u)
                {
                    archive(
                        CEREAL_NVP(targetPosition),
                        CEREAL_NVP(viewSize),
                        CEREAL_NVP(near),
                        CEREAL_NVP(far)
                    );
                }
                else if (version == 2u)
                {
                    archive(
                        CEREAL_NVP(targetPosition),
                        CEREAL_NVP(cascadeAreaTable)
                    );
                }
            }

        };

    public:
        SceneShadow();
        ~SceneShadow() = default;

    public:
        Variance variance;
        Cascade cascade;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 2u)
            {
                archive(
                    CEREAL_NVP(variance),
                    CEREAL_NVP(cascade)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(variance)
                );
            }
            else if (version == 2u)
            {
                archive(
                    CEREAL_NVP(variance),
                    CEREAL_NVP(cascade)
                );
            }
        }
    };

    struct ScenePostEffects
    {
    public:
        enum class Type : uint16_t
        {
            Bloom = BIT(0),

        };
    public:
        ScenePostEffects();
        ~ScenePostEffects() = default;

    public:
        Type enableType;

    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(enableType)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(enableType)
                );
            }
        }
    };

    class SceneSetting final : public Setting
    {
    public:
        SceneSetting();
        SceneSetting(const SceneSetting& source);
        ~SceneSetting() override = default;

        void OnGui() override;

        void Save(bool isUpdate) const override;
        void Load(const std::filesystem::path& filePath) override;

        void Clone(const Ref<Setting>& setting) override;

        void OnBeforeSave();

        [[nodiscard]]
        const char* GetAddFileNameAtEnd() const override { return "_SceneSettings"; }

        [[nodiscard]]
        const char* GetType() const override { return "SceneSetting"; }
    public:
        SceneGizmos gizmos;
        ScenePhysics physics;
        SceneShadow shadow;
        ScenePostEffects postEffects;

    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Setting>(this),
                    CEREAL_NVP(gizmos),
                    CEREAL_NVP(physics),
                    CEREAL_NVP(shadow),
                    CEREAL_NVP(postEffects)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Setting>(this),
                    CEREAL_NVP(gizmos),
                    CEREAL_NVP(physics),
                    CEREAL_NVP(shadow),
                    CEREAL_NVP(postEffects)
                );
            }
        }
    };
}

CEREAL_CLASS_VERSION(DentyEngine::SceneGizmos, 1u)
CEREAL_CLASS_VERSION(DentyEngine::ScenePhysics, 1u)

CEREAL_CLASS_VERSION(DentyEngine::SceneShadow, 2u)
CEREAL_CLASS_VERSION(DentyEngine::SceneShadow::Variance, 1u)
CEREAL_CLASS_VERSION(DentyEngine::SceneShadow::Cascade, 1u)

CEREAL_CLASS_VERSION(DentyEngine::ScenePostEffects, 1u)
CEREAL_CLASS_VERSION(DentyEngine::SceneSetting, 1u)
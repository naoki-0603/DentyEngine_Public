#pragma once

#include "System/Core/DString.h"

namespace DentyEngine
{
    namespace MeshResource
    {
        struct SystemUnitResource
        {
            SystemUnitResource() : scaleFactor(), scaling(Matrix::Identity()), fileType() {}
            SystemUnitResource(const SystemUnitResource&) = default;

            uint32_t scaleFactor;
            Matrix scaling;

            String fileType;
        private:
            friend class cereal::access;

            template <class Archive>
            void save(Archive& archive, const uint32_t version) const
            {
                if (version <= 1u)
                {
                    archive(
                        CEREAL_NVP(scaleFactor),
                        CEREAL_NVP(scaling),
                        CEREAL_NVP(fileType)
                    );    
                }
            }

            template <class Archive>
            void load(Archive& archive, const uint32_t version)
            {
                if (version <= 1u)
                {
                    archive(
                        CEREAL_NVP(scaleFactor),
                        CEREAL_NVP(scaling),
                        CEREAL_NVP(fileType)
                    );    
                }
            }
        };

        class SystemUnit final
        {
        public:
            // For serialize.
            SystemUnit() noexcept;
            explicit SystemUnit(const SystemUnitResource&) noexcept;

        	SystemUnit(const SystemUnit&) = default;

            ~SystemUnit() = default;

            void CalculateScalingMatrix();

            //
            // Setter
            //
            void SetScaleFactor(uint32_t scaleFactor) { _resource.scaleFactor = scaleFactor; }
            void SetFileType(const String& fileType) { _resource.fileType = fileType; }

            [[nodiscard]]
            const SystemUnitResource& GetResource() const { return _resource; }
        private:
            SystemUnitResource _resource;

        private:
            friend class cereal::access;

            template <class Archive>
            void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
            {
                archive(
                    CEREAL_NVP(_resource)
                );
            }

            template <class Archive>
            void load(Archive& archive, [[maybe_unused]] const uint32_t version)
            {
                archive(
                    CEREAL_NVP(_resource)
                );
            }
        };
    }
}

CEREAL_CLASS_VERSION(DentyEngine::MeshResource::SystemUnitResource, 1u)
CEREAL_CLASS_VERSION(DentyEngine::MeshResource::SystemUnit, 1u)
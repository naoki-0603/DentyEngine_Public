#pragma once

#include "DString.h"
#include "ID/UUID.h"

namespace DentyEngine
{
    class Event;
    class Object
    {
    public:
        Object() : _name(), _uuid() {}
        Object(const Object& source);
        virtual ~Object() = default;

        virtual void OnBeforeSave() {}

        // Does not call base class on serialized.
        virtual void OnSerialized() const {}

        // Does not call base class on deserialized.
        virtual void OnDeserialized() {}

        virtual void OnEvent([[maybe_unused]] Event* e) {}

        virtual void OnDestroy() {}

        virtual void Destroy() {}

        // Call on when asset modified.
        virtual void OnAssetRenamed([[maybe_unused]] std::string_view name) {}

        // Call on when asset modified.
        virtual void OnAssetDeleted() {}

        // Copy component.
        [[nodiscard]]
        virtual Ref<Object> MakeCopy() const { return nullptr; }

        void SetName(std::string_view name) { _name = name; }

        [[nodiscard]]
        const std::string& GetName() const { return _name.Get(); }

        [[nodiscard]]
    	const UUID& GetInstanceID() const { return _uuid; }
    protected:
        String _name;

        UUID _uuid;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            {
                archive(
                    CEREAL_NVP(_name),
                    CEREAL_NVP(_uuid)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, [[maybe_unused]] const uint32_t version)
        {
            archive(
                CEREAL_NVP(_name),
                CEREAL_NVP(_uuid)
            );
        }
    };
};

CEREAL_CLASS_VERSION(DentyEngine::Object, 1u)
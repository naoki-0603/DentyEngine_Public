#pragma once

namespace DentyEngine
{
	class IShape
	{
	public:
		IShape() = default;
		virtual ~IShape() = default;

		virtual void Update(const Vector3& size) = 0;

		virtual void OnGui() = 0;

		virtual void OnSerialized() const = 0;
		virtual void OnDeserialized() = 0;

		//
		// Getter
		//
		[[nodiscard]]
		virtual Ref<btCollisionShape> GetBtCollisionShape() const = 0;

		[[nodiscard]]
		virtual const char* GetShapeType() const = 0;
	};
}
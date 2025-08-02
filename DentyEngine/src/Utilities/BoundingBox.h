#pragma once

namespace DentyEngine
{
	class CollisionMesh;
	class Mesh;
	class StaticMesh;
	class SkinnedMesh;

	class BoundingBox final
	{
	public:
		BoundingBox();
		~BoundingBox() = default;

		void Generate(const Ref<Mesh>& mesh, const Matrix& scaling, bool parentNode);

		void OnGizmoRender();

		//
		// Getter
		//
		[[nodiscard]]
		const Vector3& GetMin() const { return _min; }

		[[nodiscard]]
		const Vector3& GetMax() const { return _max; }

		[[nodiscard]]
		const Vector3& GetCenter() const { return _center; }

		[[nodiscard]]
		const Vector3& GetHalfSize() const { return _halfSize; }
	private:
		void GenerateFromStaticMesh(const Ref<StaticMesh>& staticMesh, bool parentNode);
		void GenerateFromSkinnedMesh(const Ref<SkinnedMesh>& skinnedMesh, const Matrix& scaling, bool parentNode);
		void GenerateFromCollisionMesh(const Ref<CollisionMesh>& collisionMesh);

		void CalculateMinAndMax(const std::vector<Vector3>& positions);
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive) const
		{
			archive(
				CEREAL_NVP(_min),
				CEREAL_NVP(_max),
				CEREAL_NVP(_center),
				CEREAL_NVP(_halfSize)
			);
		}

		template <class Archive>
		void load(Archive& archive)
		{
			archive(
				CEREAL_NVP(_min),
				CEREAL_NVP(_max),
				CEREAL_NVP(_center),
				CEREAL_NVP(_halfSize)
			);
		}

	private:
		Vector3 _min;
		Vector3 _max;

		Vector3 _center;

		Vector3 _halfSize;
	};
}
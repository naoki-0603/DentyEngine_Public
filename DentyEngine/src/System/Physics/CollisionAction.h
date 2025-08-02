#pragma once

namespace DentyEngine
{
	class CollisionAction final : public btActionInterface
	{
	public:
		CollisionAction();
		~CollisionAction() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(CollisionAction)

		void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep) override;
		void debugDraw(btIDebugDraw* debugDrawer) override;
	};
}
#pragma once

namespace DentyEngine
{
	class FilterCallback final : public btOverlapFilterCallback
	{
	public:
		FilterCallback();
		~FilterCallback() override = default;

		[[nodiscard]]
		bool needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const override;
	};
}
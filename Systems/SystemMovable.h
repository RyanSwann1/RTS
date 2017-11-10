#pragma once

#include <Systems\SystemBase.h>

class ComponentPosition;
class ComponentMovable;
class SystemMovable : public SystemBase
{
public:
	SystemMovable(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent);
	SystemMovable(const SystemMovable&) = delete;
	SystemMovable& operator=(const SystemMovable&) = delete;
	SystemMovable(SystemMovable&&) = delete;
	SystemMovable&& operator=(SystemMovable&&) = delete;

	void update() const override;

private:
	void onSystemEvent(SystemEvent systemEvent, std::unique_ptr<Entity>& entity) const override;
	void moveInDirection(SystemEvent systemEvent, std::unique_ptr<Entity>& entity) const;

	void adjustPosition(const ComponentMovable& componentMovable, ComponentPosition& componentPosition) const;
	void resetVelocity(ComponentMovable& componentMovable) const;
};
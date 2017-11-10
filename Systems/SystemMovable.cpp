#include <Systems\SystemMovable.h>
#include <Systems\SystemEvent.h>
#include <Managers\EntityManager.h>
#include <Locators\EntityManagerLocator.h>
#include <Components\ComponentMovable.h>
#include <Components\ComponentPosition.h>
#include <Locators\FrameTimerLocator.h>
#include <Game\FrameTimer.h>

SystemMovable::SystemMovable(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent)
	: SystemBase(systemManager, systemType, requiredComponent)
{}

void SystemMovable::update() const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		if (!SystemBase::entityFitsRequirements(entity) || !SystemBase::entityFitsRequirements(entity, ComponentType::Position))
		{
			continue;
		}

		auto& componentPosition = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity);
		auto& componentMovable = entityManager.getEntityComponent<ComponentMovable>(ComponentType::Movable, entity);
		adjustPosition(componentMovable, componentPosition);
		resetVelocity(componentMovable);
	}
}

void SystemMovable::onSystemEvent(SystemEvent systemEvent, std::unique_ptr<Entity>& entity) const
{
	switch (systemEvent)
	{
	case SystemEvent::MoveLeft :
	{
		moveInDirection(SystemEvent::MoveLeft, entity);
		break;
	}
	case SystemEvent::MoveRight :
	{
		moveInDirection(SystemEvent::MoveRight, entity);
		break;
	}
	case SystemEvent::MoveUp :
	{
		moveInDirection(SystemEvent::MoveUp, entity);
		break;
	}
	case SystemEvent::MoveDown :
	{
		moveInDirection(SystemEvent::MoveDown, entity);
		break;
	}
	}
}

void SystemMovable::moveInDirection(SystemEvent systemEvent, std::unique_ptr<Entity>& entity) const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	auto& componentMovable = entityManager.getEntityComponent<ComponentMovable>(ComponentType::Movable, entity);
	
	switch (systemEvent)
	{
	case SystemEvent::MoveLeft:
	{
		componentMovable.m_velocity.x -= componentMovable.m_speed.x;
		break;
	}
	case SystemEvent::MoveRight:
	{
		componentMovable.m_velocity.x += componentMovable.m_speed.x;
		break;
	}
	case SystemEvent::MoveUp:
	{
		componentMovable.m_velocity.y -= componentMovable.m_speed.y;
		break;
	}
	case SystemEvent::MoveDown:
	{
		componentMovable.m_velocity.y += componentMovable.m_speed.y;
		break;
	}
	}
}

void SystemMovable::adjustPosition(const ComponentMovable & componentMovable, ComponentPosition & componentPosition) const
{
	const float deltaTime = FrameTimerLocator::getFrameTimer().getDeltaTime();
	componentPosition.m_position.x += componentMovable.m_velocity.x * deltaTime;
	componentPosition.m_position.y += componentMovable.m_velocity.y * deltaTime;
}

void SystemMovable::resetVelocity(ComponentMovable & componentMovable) const
{
	componentMovable.m_velocity = sf::Vector2f();
}
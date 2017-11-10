#include <Systems\SystemAIMovement.h>
#include <Components\ComponentAIMovement.h>
#include <Components\ComponentPosition.h>
#include <Managers\EntityManager.h>
#include <Locators\EntityManagerLocator.h>
#include <Managers\SystemManager.h>
#include <Systems\SystemDirectMessagePosition.h>
#include <Game\Direction.h>
#include <math.h>
#include <Game\DebugOverlay.h>

//System AIMovement
SystemAIMovement::SystemAIMovement(SystemManager& systemManager, SystemType systemType, ComponentType requiredComponent)
	: SystemBase(systemManager, systemType, requiredComponent)
{}

void SystemAIMovement::update() const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	for (auto& entity : entityManager.getEntities())
	{
		if (!SystemBase::entityFitsRequirements(entity))
		{
			continue;
		}

		handleEntityMovement(entityManager, entity);
	}
}

void SystemAIMovement::onSystemDirectMessagePosition(const SystemDirectMessagePosition & systemMessage) const
{
	switch (systemMessage.m_systemEvent)
	{
	case SystemEvent::SetTargetPosition :
	{
		DebugOverlay::clearShapes();
		setEntityTargetPosition(systemMessage.m_message, systemMessage.m_entity);
		break;
	}
	}
}

void SystemAIMovement::setEntityTargetPosition(const sf::Vector2f & targetPosition, std::unique_ptr<Entity>& entity) const
{
	auto& entityManager = EntityManagerLocator::getEntityManager();
	auto& componentAIMove = entityManager.getEntityComponent<ComponentAIMovement>(ComponentType::AIMovement, entity);
	componentAIMove.m_targetPosition = targetPosition;
	createMovementGraph(entity, entityManager, componentAIMove);
}

void SystemAIMovement::createMovementGraph(std::unique_ptr<Entity>& entity, EntityManager& entityManager, ComponentAIMovement& componentAIMovement) const
{
	const auto& componentPosition = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity);
	const auto startingPosition = sf::Vector2f(std::floor(componentPosition.m_position.x / 16), std::floor(componentPosition.m_position.y / 16));
	const auto targetPosition = sf::Vector2f(std::floor(componentAIMovement.m_targetPosition.x / 16), std::floor(componentAIMovement.m_targetPosition.y / 16));

	componentAIMovement.m_movementGraph.createGraph(startingPosition, targetPosition);
}

void SystemAIMovement::handleEntityMovement(const EntityManager& entityManager, std::unique_ptr<Entity>& entity) const
{
	const auto& componentPosition = entityManager.getEntityComponent<ComponentPosition>(ComponentType::Position, entity);
	const auto& componentAIMovement = entityManager.getEntityComponent<ComponentAIMovement>(ComponentType::AIMovement, entity);
	const Direction moveDirection = componentAIMovement.m_movementGraph.getMoveDirectionFromPosition(componentPosition.m_position);
	
	switch (moveDirection)
	{
	case Direction::Right :
	{
		m_systemManager.addSystemMessage(SystemMessage(SystemEvent::MoveRight, SystemType::Movable, entity));
		break;
	}
	case Direction::Left :
	{
		m_systemManager.addSystemMessage(SystemMessage(SystemEvent::MoveLeft, SystemType::Movable, entity));
		break;
	}
	case Direction::Up :
	{
		m_systemManager.addSystemMessage(SystemMessage(SystemEvent::MoveUp, SystemType::Movable, entity));
		break;
	}
	case Direction::Down :
	{
		m_systemManager.addSystemMessage(SystemMessage(SystemEvent::MoveDown, SystemType::Movable, entity));
		break;
	}
	}
}
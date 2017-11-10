#include <Managers\EntityManager.h>
#include <Locators\EntityManagerLocator.h>
#include <Components\ComponentPosition.h>
#include <Components\ComponentSelectable.h>
#include <Components\ComponentDrawable.h>
#include <Components\ComponentCollidable.h>
#include <Components\ComponentMovable.h>
#include <Components\ComponentAIMovement.h>
#include <Systems\SystemType.h>

//Entity Initializer
void EntityManager::EntityInitializer::initializeEntityComponents(const sf::Vector2f& startingPosition, std::unique_ptr<Entity>& entity) const
{
	//Initialize Position
	std::unique_ptr<ComponentBase>* componentPosition = &entity->m_components[static_cast<unsigned int>(ComponentType::Position)];
	assert(componentPosition);
	static_cast<ComponentPosition*>(componentPosition->get())->m_position = startingPosition;
}

//Component Factory
EntityManager::ComponentFactory::ComponentFactory()
{
	registerComponent<ComponentPosition>(ComponentType::Position, SystemType::None);
	registerComponent<ComponentDrawable>(ComponentType::Drawable, SystemType::Drawable);
	registerComponent<ComponentSelectable>(ComponentType::Selectable, SystemType::Selectable);
	registerComponent<ComponentCollidable>(ComponentType::Collidable, SystemType::Collidable);
	registerComponent<ComponentMovable>(ComponentType::Movable, SystemType::Movable);
	registerComponent<ComponentAIMovement>(ComponentType::AIMovement, SystemType::AIMovemement);
}

std::unique_ptr<ComponentBase> EntityManager::ComponentFactory::getComponent(ComponentType type) const
{
	auto iter = m_factory.find(type);
	assert(iter != m_factory.cend());
	return iter->second();
}

//Entity Factory
EntityManager::EntityFactory::EntityFactory()
{
	registerEntity("Marine", { ComponentType::Position, ComponentType::Drawable, 
		ComponentType::Selectable, ComponentType::Collidable, ComponentType::Movable, ComponentType::AIMovement });
}

std::unique_ptr<Entity> EntityManager::EntityFactory::getEntity(const ComponentFactory& componentFactory, 
	const EntityInitializer& entityInitializer, std::string&& name, const sf::Vector2f& startingPosition, int entityID) const
{
	auto iter = m_factory.find(name);
	assert(iter != m_factory.cend());
	//Create Entity
	auto entity = std::make_unique<Entity>(std::move(name), entityID);
	//Load In Components
	for (const auto& componentType : iter->second)
	{
		entity->m_components[static_cast<unsigned int>(componentType)] = componentFactory.getComponent(componentType);
	}
	assert(!entity->m_components.empty());

	//Initialize components
	entityInitializer.initializeEntityComponents(startingPosition, entity);
	return entity;
}

void EntityManager::EntityFactory::registerEntity(std::string&& name, std::vector<ComponentType>&& componentTypes)
{
	assert(m_factory.find(name) == m_factory.cend());
	m_factory.emplace(std::move(name), componentTypes);
}

//Entity Manager
EntityManager::EntityManager()
	: m_entityInitializer(),
	m_componentFactory(),
	m_entityFactory(),
	m_entities(),
	m_entityCount(0)
{
	EntityManagerLocator::provide(*this);
}

std::list<std::unique_ptr<Entity>>& EntityManager::getEntities()
{
	return m_entities;
}

void EntityManager::addEntity(std::string&& name, const sf::Vector2f& startingPosition)
{
	auto entity = m_entityFactory.getEntity(m_componentFactory, m_entityInitializer, std::move(name), startingPosition, m_entityCount);
	m_entities.emplace_back(std::move(entity));
	++m_entityCount;
}

std::unique_ptr<Entity>& EntityManager::getEntity(int entityID)
{
	auto entity = std::find_if(m_entities.begin(), m_entities.end(), [entityID](const auto& entity) { return entity->m_ID == entityID; });
	assert(entity != m_entities.end());

	return (*entity);
}

bool EntityManager::entityHasComponent(const std::unique_ptr<Entity>& entity, ComponentType componentType) const
{
	const bool entityHasComponent = entity->m_components[static_cast<unsigned int>(componentType)]->m_type == componentType;
	assert(entityHasComponent);

	return entityHasComponent;
}
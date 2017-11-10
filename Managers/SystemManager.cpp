#include <Managers\SystemManager.h>
#include <Systems\SystemDrawable.h>
#include <Systems\SystemSelectable.h>
#include <Systems\SystemCollidable.h>
#include <Locators\EntityManagerLocator.h>
#include <Managers\EntityManager.h>
#include <Systems\SystemMovable.h>
#include <Systems\SystemAIMovement.h>

SystemManager::SystemManager()
	: m_systems(),
	m_systemMessages()
{
	addSystem<SystemDrawable>(SystemType::Drawable, ComponentType::Drawable);
	addSystem<SystemSelectable>(SystemType::Selectable, ComponentType::Selectable);
	addSystem<SystemCollidable>(SystemType::Collidable, ComponentType::Collidable);
	addSystem<SystemMovable>(SystemType::Movable, ComponentType::Movable);
	addSystem<SystemAIMovement>(SystemType::AIMovemement, ComponentType::AIMovement);
}

void SystemManager::sendSystemDirectMessagePosition(const SystemDirectMessagePosition & systemMessage, SystemType systemType) const
{
	getSystem<SystemAIMovement>(systemType).onSystemDirectMessagePosition(systemMessage);
}

void SystemManager::addSystemMessage(const SystemMessage & systemMessage)
{
	m_systemMessages.push_back(systemMessage);
}

void SystemManager::draw(sf::RenderWindow & window) const
{	
	getSystem<SystemDrawable>(SystemType::Drawable).draw(window);
	getSystem<SystemSelectable>(SystemType::Selectable).draw(window);
}

void SystemManager::update()
{
	for (auto& system : m_systems)
	{
		assert(system.get());
		system->update();
	}

	handleEvents();
}

void SystemManager::handleEvents()
{
	while (!m_systemMessages.empty())
	{
		const auto& systemMessage = m_systemMessages.front();
		sendMessageToSystems(systemMessage);
		m_systemMessages.pop_front();
	} 
}

void SystemManager::sendMessageToSystems(const SystemMessage & systemMessage)
{


	if (systemMessage.m_globalToSystems)
	{
		for (const auto& system : m_systems)
		{
			system->onSystemMessage(systemMessage);
		}
	}
	else
	{
		const SystemType systemType = systemMessage.m_systemType;
		m_systems[static_cast<int>(system)]->onSystemMessage(systemMessage);
	}
}
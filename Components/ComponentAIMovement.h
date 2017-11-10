#pragma once

#include <Components\ComponentBase.h>
#include <SFML\Graphics.hpp>
#include <Entities\MovementGraph.h>

class ComponentAIMovement : public ComponentBase
{
public:
	ComponentAIMovement(ComponentType type, SystemType owningSystem)
		: ComponentBase(type, owningSystem),
		m_targetPosition(96, 96)
	{}

	sf::Vector2f m_targetPosition;
	MovementGraph m_movementGraph;
};
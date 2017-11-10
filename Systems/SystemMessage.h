#pragma once

#include <Systems\SystemEvent.h>
#include <Systems\SystemType.h>
#include <memory>

class SystemMessage
{
public:
	SystemMessage(SystemEvent systemEvent, SystemType systemType, std::unique_ptr<Entity>& entity)
		: m_systemEvent(systemEvent),
		m_systemType(systemType),
		m_entity(entity)
	{}

	const SystemEvent m_systemEvent;
	const SystemType m_systemType;
	std::unique_ptr<Entity>& m_entity;
};
#include <Entities\MovementGraph.h>
#include <Game\MathLibrary.h>
#include <assert.h>
#include <Game\DebugOverlay.h>
#include <Game\Direction.h>

//Point
MovementGraph::Point::Point(const sf::Vector2f & position, int ID, int cameFromID)
	: m_position(position),
	m_ID(ID),
	m_cameFromID(cameFromID)
{
}

//Frontier
MovementGraph::Frontier::Frontier(const sf::Vector2f& startingPosition, const sf::Vector2f& targetPosition, MovementGraph& movementGraph)
{
	std::vector<sf::Vector2f> frontier;
	bool graphComplete = false;
	sf::Vector2f nextPoint = startingPosition;
	while (!graphComplete)
	{
		addToFrontier(nextPoint, frontier, graphComplete);
		nextPoint = getNextPoint(targetPosition, frontier);
		addToGraph(nextPoint, targetPosition, movementGraph, graphComplete);

		//Uneeded points in the frontier
		frontier.clear();
	}
}

void MovementGraph::Frontier::addToFrontier(const sf::Vector2f & position, std::vector<sf::Vector2f>& frontier, bool & graphComplete)
{
	const int searchRadius = 1;
	for (int x = position.x - searchRadius; x <= position.x + searchRadius; x += 2)
	{
		frontier.emplace_back(x, position.y);
	}
		
	for (int y = position.y - searchRadius; y <= position.y + searchRadius; y += 2)
	{
		frontier.emplace_back(position.x, y);
	}
}

sf::Vector2f MovementGraph::Frontier::getNextPoint(const sf::Vector2f & targetPosition, const std::vector<sf::Vector2f>& frontier) const
{
	const sf::Vector2f* nextPoint = &frontier.front();
	int distanceAway = MathLibrary::getDistanceBetweenPoints(frontier.front(), targetPosition);
	for (auto cIter = frontier.cbegin(); cIter != frontier.cend(); ++cIter)
	{
		if (cIter == frontier.cbegin())
		{
			continue;
		}

		const int currentPositionDistanceAway = MathLibrary::getDistanceBetweenPoints((*cIter), targetPosition);
		if (currentPositionDistanceAway < distanceAway)
		{
			nextPoint = &(*cIter);
		}
	}

	return *nextPoint;
}

void MovementGraph::Frontier::addToGraph(const sf::Vector2f & position, const sf::Vector2f& targetPosition, MovementGraph& movementGraph, bool & graphComplete) const
{
	int cameFromID = 0;
	(movementGraph.m_graph.empty() ? cameFromID = 0 : cameFromID = movementGraph.m_graph.back().m_ID);
	++movementGraph.m_ID;

	movementGraph.m_graph.emplace_back(position, movementGraph.m_ID, cameFromID);
	DebugOverlay::addShape(sf::Vector2f(position.x * 16, position.y * 16));
	assert(movementGraph.isOnGraph(position));
	if (position == targetPosition)
	{
		graphComplete = true;
	}
}

//MovementGraph
MovementGraph::MovementGraph()
	: m_tileSize(16),
	m_ID(0),
	m_graph()
{
}

bool MovementGraph::isOnGraph(const sf::Vector2f & position) const
{
	auto cIter = std::find_if(m_graph.cbegin(), m_graph.cend(), [&position](const auto& pointOnGraph) { return pointOnGraph.m_position == position; });
	return cIter != m_graph.cend();
}

void MovementGraph::createGraph(const sf::Vector2f & startingPosition, const sf::Vector2f & targetPosition)
{
	Frontier frontier(startingPosition, targetPosition, *this);
}

Direction MovementGraph::getMoveDirectionFromPosition(const sf::Vector2f & position) const
{
	for (auto cIter = m_graph.cbegin(); cIter != m_graph.cend(); ++cIter)
	{
		//Last Node
		if (position == cIter->m_position && cIter == m_graph.cend() - 1)
		{
			return Direction::None;
			break;
		}
		if (position == cIter->m_position)
		{
			//Calculate next position
			const auto& nextPosition = (++cIter)->m_position;
			return MathLibrary::getDirectionFromBetweenPoints(position, nextPosition);
			break;
		}
	}

	return Direction::None;
}
#pragma once

#include <SFML\Graphics.hpp>
#include <vector>

enum class Direction;
class MovementGraph
{
	friend class Frontier;
	class Point
	{
	public:
		Point(const sf::Vector2f& position, int ID, int cameFromID);

		const sf::Vector2f m_position;
		const int m_ID;
		const int m_cameFromID;
	};

	class Frontier
	{
	public:
		Frontier(const sf::Vector2f& startingPosition, const sf::Vector2f& targetPosition, MovementGraph& movementGraph);
		
	private:
		void addToFrontier(const sf::Vector2f& position, std::vector<sf::Vector2f>& frontier, bool& graphComplete);
		sf::Vector2f getNextPoint(const sf::Vector2f& targetPosition, const std::vector<sf::Vector2f>& frontier) const;
		void addToGraph(const sf::Vector2f& position, const sf::Vector2f& targetPosition, MovementGraph& movementGraph, bool& graphComplete) const;
	};

public:
	MovementGraph();
	MovementGraph(const MovementGraph&) = delete;
	MovementGraph& operator=(const MovementGraph&) = delete;
	MovementGraph(MovementGraph&&) = delete;
	MovementGraph&& operator=(MovementGraph&&) = delete;

	void createGraph(const sf::Vector2f& startingPosition, const sf::Vector2f& targetPosition);
	Direction getMoveDirectionFromPosition(const sf::Vector2f& position) const;

private:
	const int m_tileSize;
	int m_ID;
	std::vector<Point> m_graph;

	bool isOnGraph(const sf::Vector2f& position) const;
};
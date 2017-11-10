#include <Game\MathLibrary.h>
#include <math.h>
#include <Game\Direction.h>
#include <assert.h>

Direction MathLibrary::getDirectionFromBetweenPoints(const sf::Vector2f & pointA, const sf::Vector2f & pointB)
{
	Direction direction;

	if (pointB.x > pointA.x)
	{
		direction = Direction::Right;
	}
	else if (pointB.x < pointA.x)
	{
		direction = Direction::Left;
	}
	else if (pointB.y < pointB.y)
	{
		direction = Direction::Up;
	}
	else if (pointB.y > pointA.y)
	{
		direction = Direction::Down;
	}
	else if (pointA == pointB)
	{
		direction = Direction::None;
	}

	return direction;
}

int MathLibrary::getDistanceBetweenPoints(const sf::Vector2f & pointA, const sf::Vector2f & pointB)
{
	return std::abs(pointA.x - pointB.x) + std::abs(pointA.y - pointB.y);
}
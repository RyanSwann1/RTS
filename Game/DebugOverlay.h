#pragma once

#include <SFML\Graphics.hpp>
namespace DebugOverlay
{
	void draw(sf::RenderWindow& window);
	void addShape(const sf::Vector2f& position);
	void clearShapes();
}
#pragma once
#include <SFML/Graphics.hpp>
void drawTriangle(sf::RenderWindow& window, sf::Vector2u a, sf::Vector2u b, sf::Vector2u c, sf::Color color);
void fillTriangleScan(sf::RenderWindow& window, sf::Vector2u a, sf::Vector2u b, sf::Vector2u c, sf::Color color);
void fillTriangleBound(sf::RenderWindow& window, sf::Vector2u a, sf::Vector2u b, sf::Vector2u c, sf::Color color);
#pragma once

#include <SFML/Graphics.hpp>
void drawPixel(sf::RenderWindow& window, sf::Vector2u position, sf::Color color);
void drawLine(sf::RenderWindow& window, sf::Vector2u start, sf::Vector2u end, sf::Color color);
void drawLineNaive(sf::RenderWindow& window, sf::Vector2u start, sf::Vector2u end, sf::Color color);

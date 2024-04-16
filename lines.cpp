#include "lines.h"
#include <SFML/Graphics.hpp>
typedef sf::Vector2u Vec2;

void drawLineVertical(sf::RenderWindow& window, Vec2 start, Vec2 end, sf::Color color) {
	if (start.y > end.y) {
		std::swap(start, end);
	}
	for (auto i = start.y; i < end.y + 1; i++) {
		drawPixel(window, Vec2{ start.x, i }, color);
	}
}

void drawLineHorizontal(sf::RenderWindow& window, Vec2 start, Vec2 end, sf::Color color) {
	if (start.x > end.x) {
		std::swap(start, end);
	}
	for (auto i = start.x; i < end.x + 1; i++) {
		drawPixel(window, Vec2{ i, start.y }, color);
	}
}

void drawLineLowNaive(sf::RenderWindow& window, Vec2 start, Vec2 end, sf::Color color) {
	if (start.x > end.x) {
		std::swap(start, end);
	}
	double m = (static_cast<double>(end.y) - start.y) / (end.x - start.x);
	double b = start.y - m * start.x;

	for (auto i = start.x; i < end.x + 1; i++) {
		auto y = static_cast<uint32_t>(m * i + b);
		drawPixel(window, Vec2(i, y), color);
	}
}

void drawLineHighNaive(sf::RenderWindow& window, Vec2 start, Vec2 end, sf::Color color) {
	if (start.y > end.y) {
		std::swap(start, end);
	}
	double m = (static_cast<double>(end.y) - start.y) / (end.x - start.x);
	double b = start.y - m * start.x;

	for (int i = start.y; i < end.y + 1; i++) {
		auto x = static_cast<uint32_t>((i - b) / m);
		drawPixel(window, Vec2(x, i), color);
	}
}

void drawLineNaive(sf::RenderWindow& window, Vec2 start, Vec2 end, sf::Color color) {
	if (start.x == end.x) {
		drawLineVertical(window, start, end, color);
	}
	else if (start.y == end.y) {
		drawLineHorizontal(window, start, end, color);
	}
	else {
		double m = (static_cast<double>(end.y) - start.y) / (end.x - start.x);
		if (abs(m) <= 1) {
			drawLineLowNaive(window, start, end, color);
		}
		else {
			drawLineHighNaive(window, start, end, color);
		}
	}
}

void drawLineLow(sf::RenderWindow& window, Vec2 start, Vec2 end, sf::Color color) {
	int32_t dx = end.x - start.x;
	int32_t dy = end.y - start.y;
	int32_t yi = 1;

	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}

	int32_t d = (2 * dy) - dx;
	for (Vec2 p = start; p.x < end.x + 1; p.x++) {
		drawPixel(window, p, color);
		if (d > 0) {
			p.y += yi;
			d = d + 2 * (dy - dx);
		}
		else {
			d = d + 2 * dy;
		}
	}
}

void drawLineHigh(sf::RenderWindow& window, Vec2 start, Vec2 end, sf::Color color) {
	int32_t dx = end.x - start.x;
	int32_t dy = end.y - start.y;
	int32_t xi = 1;

	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}

	int32_t d = (2 * dx) - dy;
	for (Vec2 p = start; p.y < end.y + 1; p.y++) {
		drawPixel(window, p, color);
		if (d > 0) {
			p.x += xi;
			d = d + 2 * (dx - dy);
		}
		else {
			d = d + 2 * dx;
		}
	}
}


void drawLine(sf::RenderWindow& window, Vec2 start, Vec2 end, sf::Color color) {
	sf::Vertex points[2] = {
		{{(float)start.x, (float)start.y}, color},
		{{(float)end.x, (float)end.y}, color}
	};
	window.draw(points, 2, sf::Lines);
	return;

	if (start.x == end.x) {
		drawLineVertical(window, start, end, color);
	}
	else if (start.y == end.y) {
		drawLineHorizontal(window, start, end, color);
	}
	else if (abs((double)(end.y - start.y) / (end.x - start.x)) <= 1) {
		if (start.x <= end.x) {
			drawLineLow(window, start, end, color);
		}
		else {
			drawLineLow(window, end, start, color);
		}
	}
	else {
		if (start.y <= end.y) {
			drawLineHigh(window, start, end, color);
		}
		else {
			drawLineHigh(window, end, start, color);
		}
	}
}

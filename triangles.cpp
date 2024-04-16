#include "triangles.h"
#include "lines.h"
typedef sf::Vector2u Vec2;

void drawTriangle(sf::RenderWindow& window, Vec2 a, Vec2 b, Vec2 c, sf::Color color) {
	 drawLine(window, a, b, color);
	 drawLine(window, a, c, color);
	 drawLine(window, b, c, color);
}

void fillTriangleScan(sf::RenderWindow& window, Vec2 a, Vec2 b, Vec2 c, sf::Color color) {
	// First, sort the vertices by y coordinate.
	if (a.y > b.y) {
		std::swap(a, b);
	}
	if (a.y > c.y) {
		std::swap(a, c);
	}
	if (b.y > c.y) {
		std::swap(b, c);
	}

	// Draw the upper segment of the triangle: the lines connecting edge AB with part of AC.

	// First find the inverse slope of AB and AC.
	double imAB = (static_cast<double>(b.x) - a.x) / (b.y - a.y);
	double imAC = (static_cast<double>(c.x) - a.x) / (c.y - a.y);

	// We will "walk" down the lines by repeatedly adding the inverse slope
	// to an x - coordinate accumulator for each line segment, starting at point A.
	double_t xAB = a.x;
	double_t xAC = a.x;

	drawPixel(window, a, color);
	// For the vertical range of edge AB:
	for (auto y = a.y + 1; y < b.y; y++) {
		// Compute the x coordinate for edge AB and AC by moving their current coordinate
		// by their inverse slope.
		xAB = xAB + imAB;
		xAC = xAC + imAC;

		// Connect the two points with a horizontal line.
		// Make sure the range goes in increasing order from left to right.
		auto l = std::min(static_cast<uint32_t>(xAB), static_cast<uint32_t>(xAC));
		auto r = std::max(static_cast<uint32_t>(xAB), static_cast<uint32_t>(xAC));
		
		for (auto x = l; x < r + 1; x++) {
			drawPixel(window, Vec2{ x, y }, color);
		}
	}

	if (c.y == b.y) {
		double_t xBC = b.x;
		auto l = std::min(static_cast<uint32_t>(xBC), static_cast<uint32_t>(xAC));
		auto r = std::max(static_cast<uint32_t>(xBC), static_cast<uint32_t>(xAC));
		for (auto x = l; x < r + 1; x++) {
			drawPixel(window, Vec2{ x, c.y }, color);
		}
	}
	else {

		double_t imBC = (static_cast<double>(c.x) - b.x) / (c.y - b.y);
		double_t xBC = b.x;
		for (auto y = b.y; y < c.y + 1; y++) {
			xBC = xBC + imBC;
			xAC = xAC + imAC;

			// Connect the two points with a horizontal line.
			// Make sure the range goes in increasing order from left to right.
			auto l = std::min(static_cast<uint32_t>(xBC), static_cast<uint32_t>(xAC));
			auto r = std::max(static_cast<uint32_t>(xBC), static_cast<uint32_t>(xAC));

			for (auto x = l; x < r + 1; x++) {
				drawPixel(window, Vec2{ x, y }, color);
			}
		}
	}
}

int32_t cross(Vec2 a, Vec2 b) {
	return a.x * b.y - b.x * a.y;
}

bool insideCross(Vec2 p, Vec2 a, Vec2 b, Vec2 c) {
	Vec2 ab = b - a;
	Vec2 bc = c - b;
	Vec2 ca = a - c;
	Vec2 ap = p - a;
	Vec2 bp = p - b;
	Vec2 cp = p - c;

	return cross(ab, ap) >= 0 && cross(bc, bp) >= 0 && cross(ca, cp) >= 0;

}

bool insideBary2(Vec2 p, Vec2 a, Vec2 b, Vec2 c) {
	int32_t s1 = c.y - a.y;
	int32_t s2 = c.x - a.x;
	int32_t s3 = b.y - a.y;
	int32_t s4 = p.y - a.y;
	double_t w1 = static_cast<double_t>(a.x * s1 + s4 * s2 - p.x * s1) / (s3 * s2 - (b.x - a.x) * s1);
	double_t w2 = static_cast<double_t>(s4 - w1 * s3) / s1;
	return w1 >= 0 && w2 >= 0 && w1 + w2 <= 1;
}

void fillTriangleBound(sf::RenderWindow& window, Vec2 a, Vec2 b, Vec2 c, sf::Color color) {
	auto minX = std::min({ a.x, b.x, c.x });
	auto minY = std::min({ a.y, b.y, c.y });
	auto maxX = std::max({ a.x, b.x, c.x });
	auto maxY = std::max({ a.y, b.y, c.y });
	for (auto x = minX; x < maxX + 1; x++) {
		for (auto y = minY; y < maxY + 1; y++) {
			if (insideCross(Vec2{ x, y }, a, b, c)) {
				drawPixel(window, Vec2{ x, y }, color);
			}
		}
	}
}
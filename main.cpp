#include <SFML/Graphics.hpp>
#include <iostream>

#include "Mesh3D.h"
#include "Object3D.h"

#include <memory>
#include <glm/ext.hpp>

#include "AssimpImport.h"


void drawPixel(sf::RenderWindow& window, sf::Vector2u position, sf::Color color) {
	sf::Vertex pixel[1] = {
		sf::Vertex(sf::Vector2f(position.x, position.y), color)
	};
	window.draw(pixel, 1, sf::Points);
}

int main() {
	sf::RenderWindow window {sf::VideoMode{1200, 800}, "SFML Demo"};
	sf::Clock c;
	
	//// Default cube example
	//auto square = Object3D(std::make_shared<Mesh3D>(Mesh3D::cube(sf::Color::Red)));
	
	//// Drawing a house object
	//auto square = Object3D(std::make_shared<Mesh3D>(Mesh3D::house(sf::Color::Red)));

	//// Use GLM to create a perspective-projection matrix with a 45deg FOVY.
	auto proj = glm::perspective(glm::radians(45.0), (double)window.getSize().x / window.getSize().y, 0.1, 100.0);

	// Use an identity matrix for the camera, which places it at (0, 0, 0) looking down the negative z axis.
	//auto view = glm::mat4(1);
	// TODO: learn how to use glm::lookAt, to place the camera at (3, 0, 3) looking at the origin.
	// Using documentation at https://learnopengl.com/Getting-started/Camera
	
	glm::vec3 cameraPos = glm::vec3(3, 0, 3);       // Where the camera is placed
	glm::vec3 cameraTarget = glm::vec3(0, 0, 0);    // The camera target's location
	glm::vec3 cameraDirection = glm::vec3(0, 1, 0); // Keeps the camera "upright"

	auto view = glm::lookAt(cameraPos, cameraTarget, cameraDirection);

	// The square in local space is at the origin; move it away from the camera so we can see it.
	//square.move(glm::vec3(0, 0, -3));

	//// TODO
	// Find the line in main() that initializes the Object3D using the square mesh. 
	// Change it so it uses the assimpLoad function defined in AssimpImport.cpp. 
	// This function takes a single parameter: a string path, giving the relative path 
	// for a 3D model file supported by Assimp. You will load bunny.obj, included in the ZIP file.
	// auto square = Object3D(std::make_shared<Mesh3D>(Mesh3D::cube(sf::Color::Red)));
	
	auto square = assimpLoad("bunny.obj");
	
	// Gman
	//auto square = assimpLoad("gman.obj");

	//The bunny is significantly smaller than the cube.I suggest scaling it by a factor of 9, and moving it to(0.2, -1, -3).
	
	auto last = c.getElapsedTime();
	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}
		
		/*
		window.clear();
		square.rotate(glm::vec3(0, 0.001, 0));
		square.render(window, view, proj);
		window.display();
		*/

		
		window.clear();
		square.setPosition(glm::vec3(0.2, -1, -3));
		square.rotate(glm::vec3(0, 0.006, 0));
		square.setScale(glm::vec3(20));
		square.render(window, view, proj);
		window.display();
		

		auto now = c.getElapsedTime();
		auto diff = now - last;
		std::cout << 1 / diff.asSeconds() << " FPS " << std::endl;
		last = now;
	}
	
	return 0;
}



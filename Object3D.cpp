#include "Object3D.h"
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>

int test_count = 0;

void Object3D::rebuildModelMatrix() {
	// Initialize the model matrix as the identity matrix.
	auto model = glm::mat4(1); 

	// TODO: either initialize your own mat4 objects to represent scale/yaw/pitch/roll/translate
	// matrices and then multiply them together; 
	
	// SCALE
	auto scaleMatrix = glm::mat4(
		m_scale.x, 0, 0, 0,
		0, m_scale.y, 0, 0,
		0, 0, m_scale.z, 0,
		0, 0, 0, 1
	);

	// YAW
	float yawRadians = m_orientation.y;
	auto yawMatrix = glm::mat4(
		std::cos(yawRadians), 0, std::sin(yawRadians), 0,
		0, 1, 0, 0,
		-std::sin(yawRadians), 0, std::cos(yawRadians), 0,
		0, 0, 0, 1
	);

	// PITCH
	float pitchRadians = m_orientation.x;
	auto pitchMatrix = glm::mat4(
		1, 0, 0, 0,
		0, std::cos(pitchRadians), -std::sin(pitchRadians), 0,
		0, std::sin(pitchRadians), std::cos(pitchRadians), 0,
		0, 0, 0, 1
	);

	// ROLL
	float rollRadians = m_orientation.z;
	auto rollMatrix = glm::mat4(
		std::cos(rollRadians), -std::sin(rollRadians), 0, 0,
		std::sin(rollRadians), std::cos(rollRadians), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	// TRANSLATE
	auto translateMatrix = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		m_position.x, m_position.y, m_position.z, 1
	);


	// OR learn to use glm::scale, glm::rotate, and glm::translate to do this.
	// Remember that the transformations must be applied in the correct order:
	//    scale, yaw, pitch, roll, translate

	// In the end, assign the computed model matrix to m_modelMatrix.
	//m_modelMatrix = model * scaleMatrix * yawMatrix * pitchMatrix * rollMatrix * translateMatrix;
	m_modelMatrix = translateMatrix * rollMatrix * pitchMatrix * yawMatrix * scaleMatrix * model;


	// ADD TEST HERE 
	/*
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			// Accessing element (i, j) of the matrix
			std::cout << m_modelMatrix[j][i] << " "; // Note: glm uses column-major order, so access is column-row
		}
		std::cout << std::endl; // New line for each row
	}
	std::cout << std::endl;
	test_count++;
	if (test_count == 10)
		exit(0);
	*/
	// END TEST
}

Object3D::Object3D(std::shared_ptr<Mesh3D> &&mesh) : m_mesh(mesh), m_position(), m_orientation(), m_scale(1.0) {
	// After every change to m_position, m_orientation, or m_scale, we must rebuild the model matrix.
	rebuildModelMatrix();
}

const glm::vec3& Object3D::getPosition() const {
	return m_position;
}

const glm::vec3& Object3D::getOrientation() const {
	return m_orientation;
}

const glm::vec3& Object3D::getScale() const {
	return m_scale;
}

void Object3D::setPosition(const glm::vec3& position) {
	m_position = position;
	rebuildModelMatrix();
}
// m_orientation changed
void Object3D::setOrientation(const glm::vec3& orientation) {
	m_orientation = orientation;
	rebuildModelMatrix();
}
// m_scale changed
void Object3D::setScale(const glm::vec3& scale) {
	m_scale = scale;
	rebuildModelMatrix();
}

void Object3D::move(const glm::vec3& offset) {
	m_position = m_position + offset;
	rebuildModelMatrix();
}

void Object3D::rotate(const glm::vec3& rotation) {
	m_orientation = m_orientation + rotation;
	rebuildModelMatrix();
}

void Object3D::grow(const glm::vec3& growth) {
	m_scale = m_scale * growth;
	rebuildModelMatrix();
}

void Object3D::render(sf::RenderWindow& window, const glm::mat4& view, const glm::mat4& proj) const {
	m_mesh->render(window, m_modelMatrix, view, proj);
}

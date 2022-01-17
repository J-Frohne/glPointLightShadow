#pragma once

#include "input.hpp"

#include <glm/vec3.hpp>


class Camera
{
public:
	Camera(Input& inputHandler);

	void update(float deltaTimeSeconds);
	void lookAt(glm::vec3 eye, glm::vec3 center);
	glm::mat4 getViewMatrix() const;

private:
	// Camera position
	glm::vec3 m_position;

	// Camera orientation
	float m_theta; // polar angle, 0° <= theta <= 180°
	float m_phi; // azimuthal angle, 0° <= phi <= 360°

	// Invert camera
	bool m_invertVertically;
	bool m_invertHorizontally;

	// Camera coordinate system
	glm::vec3 m_view;
	glm::vec3 m_up;
	glm::vec3 m_right;

	// Camera speed
	float m_unitsPerSecond; // defines how fast the camera can be moved
	float m_radiansPerPixel; // defines how much the camera orientation will change if the mouse is moved one pixel

	// Input handler
	Input& m_input;
};

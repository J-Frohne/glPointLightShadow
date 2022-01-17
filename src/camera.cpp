#include "camera.hpp"

#define GLM_FORCE_RADIANS 
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include <GLFW/glfw3.h>


Camera::Camera(Input& inputHandler) 
	: m_input(inputHandler)
	, m_unitsPerSecond(3.f)
	, m_radiansPerPixel(0.001f)
	, m_invertHorizontally(false)
	, m_invertVertically(false)
{
	lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.f));
}

void Camera::update(float deltaTimeSeconds)
{
	if (m_input.isHold(GLFW_MOUSE_BUTTON_LEFT) && !m_input.isPushed(GLFW_MOUSE_BUTTON_LEFT))
	{
		// calculate how far the mouse moved
		glm::vec2 delta = m_input.getCursorPosLastFrame() - m_input.getCursorPos();

		// update angles
		m_theta -= delta.y * m_radiansPerPixel * (m_invertVertically   ? -1.f : 1.f);
		m_phi   -= delta.x * m_radiansPerPixel * (m_invertHorizontally ? -1.f : 1.f);

		// clamp (0° < theta < 180°) to prevent camera from flipping
		m_theta = glm::clamp(m_theta, 0.001f, glm::pi<float>() - 0.001f);

		// keep phi within [0°, 360°)
		m_phi = fmod(m_phi + 2.0f * glm::pi<float>(), 2.0f * glm::pi<float>());
	}

	// Update coordinate system of the camera
	m_up = glm::vec3(0, 1, 0);
	m_view = glm::vec3(glm::sin(m_theta) * glm::cos(m_phi), glm::cos(m_theta), glm::sin(m_theta) * glm::sin(m_phi));
	m_right = glm::normalize(glm::cross(m_view, m_up));

	// Horizontal forward direction used for camera movement
	glm::vec3 forward = glm::cross(m_up, m_right);
	
	// Move the camera
	float distance = m_unitsPerSecond * deltaTimeSeconds;
	if (m_input.isHold(GLFW_KEY_W))
		m_position += distance * forward;
	if (m_input.isHold(GLFW_KEY_S))
		m_position += -distance * forward;
	if (m_input.isHold(GLFW_KEY_A))
		m_position += -distance * m_right;
	if (m_input.isHold(GLFW_KEY_D))
		m_position += distance * m_right;
	if (m_input.isHold(GLFW_KEY_SPACE))
		m_position += distance * m_up;
	if (m_input.isHold(GLFW_KEY_X) || m_input.isHold(GLFW_KEY_LEFT_SHIFT) || m_input.isHold(GLFW_KEY_LEFT_CONTROL))
		m_position += -distance * m_up;

	// Inverting the camera
	if (m_input.isPushed(GLFW_KEY_COMMA))
	{
		m_invertHorizontally = !m_invertHorizontally;
	}
	if (m_input.isPushed(GLFW_KEY_PERIOD))
	{
		m_invertVertically = !m_invertVertically;
	}
}

void Camera::lookAt(glm::vec3 eye, glm::vec3 center)
{
	// Update eye position
	m_position = eye;

	// Update coordinate system
	m_view = glm::normalize(center - eye);
	m_up = glm::vec3(0, 1, 0);
	m_right = glm::normalize(glm::cross(m_view, m_up));

	// Update angles
	glm::vec3 horizontalOrientation = glm::cross(m_up, m_right);
	m_theta = glm::acos(m_view.y);
	m_phi = glm::atan(horizontalOrientation.z, horizontalOrientation.x);
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_position, m_position + m_view, m_up);
}

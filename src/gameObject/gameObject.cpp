#include "gameObject/gameObject.hpp"
#include "scene/primitive.hpp"

#define  GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include <GLFW/glfw3.h>

GameObject::GameObject(SceneGraph* sceneGraph, size_t parentNode)
	: m_sceneGraph(sceneGraph)
	, m_parentNode(parentNode)
{

}

GameObject::~GameObject()
{
}

LightSphere::LightSphere(SceneGraph* sceneGraph, size_t parentNode, glm::vec3 position, Input* input, Camera const* camera)
	: GameObject(sceneGraph, parentNode)
	, m_input(input)
	, m_camera(camera)
	, m_spherePosition(position)
	, m_sphereSpeed(2.f)
	, m_animate(true)
	, m_animation(2.f)
	, m_lightSource(
		glm::vec3(10.f, 10.f, 0.f),
		glm::vec4(0.5f),
		glm::vec4(0.8f),
		glm::vec4(0.6f))
{
	// Create the material
	Material* sphereMaterial = sceneGraph->takeMaterial(std::move(std::make_unique<Material>(
		glm::vec4(1000.f),
		glm::vec4(0.f),
		glm::vec4(0.f),
		1.f, 1.f)));

	// Create the geometry
	m_sphereNode = m_sceneGraph->addNode(m_parentNode, false, glm::translate(m_spherePosition));
	std::vector<Vertex> verts;
	std::vector<uint32_t> inds;
	createSphere(0.05f, 40, 40, verts, inds);
	Mesh* sphereMesh = m_sceneGraph->takeMesh(std::make_unique<Mesh>(verts, inds, *sphereMaterial));
	m_sceneGraph->addNodeMesh(m_sphereNode, sphereMesh);

	// Create the animation keyframes
	float zMin = -3.45f;
	float zMid = -0.28f;
	float zMax = 3.17f;
	float xMin = -9.18f;
	float xMax = 8.64f;
	float y = 1.5f;
	m_keyFrames.push_back(glm::vec3(xMax, y, zMin));
	m_keyFrames.push_back(glm::vec3(xMin, y, zMin));
	m_keyFrames.push_back(glm::vec3(xMin, y, zMid));
	m_keyFrames.push_back(glm::vec3(xMax, y, zMid));
	m_keyFrames.push_back(glm::vec3(xMax, y, zMax));
	m_keyFrames.push_back(glm::vec3(xMin, y, zMax));
	m_keyFrames.push_back(glm::vec3(xMin, y, zMid));
	m_keyFrames.push_back(glm::vec3(xMax, y, zMid));

	// Adjust how fast the animation is advancing during the different parts
	// of the animation. This ensures that the sphere is always moving at 
	// constant speed 
	float zSpeed1 = m_sphereSpeed / (zMax - zMid);
	float zSpeed2 = m_sphereSpeed / (zMid - zMin);
	float xSpeed  = m_sphereSpeed / (xMax - xMin);
	m_keyFrameSpeeds.push_back(xSpeed);
	m_keyFrameSpeeds.push_back(zSpeed2);
	m_keyFrameSpeeds.push_back(xSpeed);
	m_keyFrameSpeeds.push_back(zSpeed1);
	m_keyFrameSpeeds.push_back(xSpeed);
	m_keyFrameSpeeds.push_back(zSpeed2);
	m_keyFrameSpeeds.push_back(xSpeed);
	m_keyFrameSpeeds.push_back(zSpeed1);
}

LightSphere::~LightSphere()
{
}

void LightSphere::update(float deltaTimeSeconds)
{
	// Toggle animation
	if (m_input->isPushed(GLFW_KEY_P))
	{
		m_animate = !m_animate;
	}

	if (m_animate)
	{
		// Compute new position
		int keyFrame = static_cast<int>(m_animation);
		float animationMod1 = glm::mod(m_animation, 1.f);
		m_spherePosition = (1.f - animationMod1) * m_keyFrames[ keyFrame      % m_keyFrames.size()] +
								  animationMod1  * m_keyFrames[(keyFrame + 1) % m_keyFrames.size()];
	
		// Advance the animation
		m_animation = glm::mod(m_animation + deltaTimeSeconds * m_keyFrameSpeeds[keyFrame], static_cast<float>(m_keyFrames.size()));

		// Update position
		m_sceneGraph->setNodeTransformation(m_sphereNode, glm::translate(m_spherePosition));
		m_lightSource.position = m_spherePosition;
	}
	else
	{
		// Movement relative to the camera
		glm::mat4 v = m_camera->getViewMatrix();
		glm::vec3 rightDir(v[0].x, v[1].x, v[2].x);
		glm::vec3 upDir = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 forwardDir = glm::cross(upDir, rightDir);

		// Distance traveled in this frame
		float distance = m_sphereSpeed * deltaTimeSeconds;

		if (m_input->isHold(GLFW_KEY_UP) || m_input->isHold(GLFW_KEY_I))
		{
			m_spherePosition += distance * forwardDir;
		}
		if (m_input->isHold(GLFW_KEY_DOWN) || m_input->isHold(GLFW_KEY_K))
		{
			m_spherePosition += -distance * forwardDir;
		}
		if (m_input->isHold(GLFW_KEY_LEFT) || m_input->isHold(GLFW_KEY_J))
		{
			m_spherePosition += -distance * rightDir;
		}
		if (m_input->isHold(GLFW_KEY_RIGHT) || m_input->isHold(GLFW_KEY_L))
		{
			m_spherePosition += distance * rightDir;
		}
		if (m_input->isHold(GLFW_KEY_U))
		{
			m_spherePosition += glm::vec3(0.f, -distance, 0.f);
		}
		if (m_input->isHold(GLFW_KEY_O))
		{
			m_spherePosition += glm::vec3(0.f, distance, 0.f);
		}

		// Update the transformation within the scene graph
		m_sceneGraph->setNodeTransformation(m_sphereNode, glm::translate(m_spherePosition));

		// Update the position of the actual light source
		m_lightSource.position = m_spherePosition;
	}
}

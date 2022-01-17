#pragma once

#include "input.hpp"
#include "camera.hpp"
#include "scene/sceneGraph.hpp"
#include "scene/lightSource.hpp"

class GameObject
{
public:
	GameObject(SceneGraph* sceneGraph, size_t parentNode);
	virtual ~GameObject();

	virtual void update(float deltaTimeSeconds) = 0;

protected:
	// Reference to the scene graph so that the game object is able to add 
	// and update nodes
	SceneGraph* m_sceneGraph;
	size_t m_parentNode;
};

class LightSphere : GameObject
{
public:
	LightSphere(SceneGraph* sceneGraph, size_t parentNode, glm::vec3 position, Input* input, Camera const* camera);
	virtual ~LightSphere();

	virtual void update(float deltaTimeSeconds) override;

public:
	LightSource m_lightSource;

private:
	Input* m_input;
	Camera const* m_camera;

	size_t m_sphereNode;
	glm::vec3 m_spherePosition;
	float m_sphereSpeed;

	bool m_animate;
	float m_animation;
	std::vector<glm::vec3> m_keyFrames;
	std::vector<float> m_keyFrameSpeeds;
};
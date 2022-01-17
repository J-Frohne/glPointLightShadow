#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct LightSource
{
	LightSource();
	LightSource(glm::vec3 position, glm::vec4 Ia, glm::vec4 Id, glm::vec4 Is);

	glm::vec3 position;
	glm::vec4 Ia;
	glm::vec4 Id;
	glm::vec4 Is;
};
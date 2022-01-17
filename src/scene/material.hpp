#pragma once

#include <GL/glew.h>
#include <glm/vec4.hpp>


struct Material
{
	Material();
	Material(glm::vec4 Ka, glm::vec4 Kd, glm::vec4 Ks, float Ns, float d);

	// Move constructor
	Material(Material&& other);

	// Move assignment
	Material& operator=(Material&& other);

	// Destructor
	~Material();

	// Delete copy constructor and copy assignment operator
	Material(Material const&) = delete;
	Material& operator=(Material const&) = delete;

	glm::vec4 Ka; // ambient color
	glm::vec4 Kd; // diffuse color
	glm::vec4 Ks; // specular color
	float Ns; // specular exponent
	float d; // dissolve i.e. transparency (1.0 means fully opaque)

	GLuint textureKa; // ambient color texture
	GLuint textureKd; // diffuse color texture
	GLuint textureKs; // specular color texture
};
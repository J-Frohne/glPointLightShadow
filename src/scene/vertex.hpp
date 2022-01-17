#pragma once

#include <GL/glew.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>


struct Vertex
{
	Vertex();
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates, glm::vec4 color);

	bool operator==(Vertex const& other) const;

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoordinates;
	glm::vec4 color;

	// Creates an OpenGL vertex array from an array of vertices and an array of indices
	static GLuint createVertexArrayObject(std::vector<Vertex> const& vertices, std::vector<uint32_t> const& indices);
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const;
	};
}
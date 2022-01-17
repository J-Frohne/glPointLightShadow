#include "scene/vertex.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>


Vertex::Vertex() 
	: position(glm::vec3(0))
	, normal(glm::vec3(1.f, 0.f, 0.f))
	, textureCoordinates(glm::vec2(0))
	, color(1.f)
{
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates) 
	: Vertex(position, normal, textureCoordinates, glm::vec4(1))
{
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates, glm::vec4 color) 
	: position(position)
	, normal(normal)
	, textureCoordinates(textureCoordinates)
	, color(color)
{
}

bool Vertex::operator==(Vertex const& other) const
{
	return
		position == other.position &&
		normal == other.normal &&
		textureCoordinates == other.textureCoordinates &&
		color == other.color;
}

size_t std::hash<Vertex>::operator()(Vertex const& vertex) const
{
	return
		(
			(
				hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.normal) << 1)
				) >> 1
			)
		^
		(
			hash<glm::vec2>()(vertex.textureCoordinates) ^ (hash<glm::vec4>()(vertex.color) << 1)
			);
}

GLuint Vertex::createVertexArrayObject(std::vector<Vertex> const& vertices, std::vector<uint32_t> const& indices)
{
	// Create a vertex array
	GLuint vertexArrayObject = 0;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Add buffers to the vertex array
	GLuint positionBuffer;
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	GLuint texCoordBuffer;
	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, textureCoordinates));
	glEnableVertexAttribArray(2);

	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	// Unbind the vertex array
	glBindVertexArray(0);

	// Delete the buffers
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &texCoordBuffer);
	glDeleteBuffers(1, &indexBuffer);

	return vertexArrayObject;
}
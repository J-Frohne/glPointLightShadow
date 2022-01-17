#pragma once

#include "scene/vertex.hpp"
#include "scene/material.hpp"

#include <GL/glew.h>

#include <vector>
#include <string>
#include <memory>


struct Mesh
{
	// Creates a vertex buffer and an index buffer and uploads the vertex and index data to device memory
	Mesh(std::vector<Vertex> const& vertices, std::vector<uint32_t> const& indices, Material const& material);
	
	// Move constructor
	Mesh(Mesh&& other);

	// Destructor
	~Mesh();

	// Delete copy constructor and assignment operators
	Mesh(Mesh const&) = delete;
	Mesh& operator=(Mesh const&) = delete;
	Mesh& operator=(Mesh&& other) = delete;

	GLuint vertexArrayObject;
	uint32_t numVertices;
	uint32_t numIndices;

	Material const& material;

	// Reads an obj file. Returns an array of meshes and materials
	static void readObj(
		std::string directory,
		std::string filename,
		std::vector<std::unique_ptr<Mesh>>& outMeshes,
		std::vector<std::unique_ptr<Material>>& outMaterials);
};
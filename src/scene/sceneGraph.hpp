#pragma once

#include "scene/mesh.hpp"
#include "scene/material.hpp"

#include <glm/mat4x4.hpp>

#include <memory>
#include <vector>


class SceneGraph
{
public:
	// A node of the Scene Graph
	struct SceneNode
	{
		size_t index; // Index of this node
		size_t parent; // Index of the parent node
		std::vector<size_t> children; // Indices of the child nodes

		std::vector<Mesh*> meshes; // Meshes which are attached to this node

		glm::mat4 nodeMatrix; // Transformation of this node
		glm::mat4 modelMatrix; // Combined transformation of this node and all its parents

		bool castsShadow; // If false, this node is not rendered in the shadow pass
	};

private:
	std::vector<SceneNode> m_nodes; // Stores the nodes of the scene graph

	std::vector<std::unique_ptr<Mesh>> m_meshes; // Stores all the meshes used by the scene graph
	std::vector<std::unique_ptr<Material>> m_materials; // Stores all materials used within the scene

public:
	SceneGraph();
	~SceneGraph();

	// Takes ownership of a mesh
	Mesh* takeMesh(std::unique_ptr<Mesh> mesh);
	std::vector<Mesh*> takeMeshes(std::vector<std::unique_ptr<Mesh>>& meshes);

	// Takes ownership of a material
	Material* takeMaterial(std::unique_ptr<Material> material);
	std::vector<Material*> takeMaterials(std::vector<std::unique_ptr<Material>>& materials);

	// Adds a new node to the scene graph
	size_t addNode(size_t parent, bool castsShadow, glm::mat4 initialTransformation = glm::mat4(1));

	// Adds a mesh to the specified node
	void addNodeMesh(size_t nodeIdx, Mesh* mesh);
	void addNodeMeshes(size_t nodeIdx, std::vector<Mesh*>& meshes);

	// Sets the (local) transformation of the specified node. The new model matrix 
	// of  this node is available  only after calling update().
	void setNodeTransformation(size_t nodeIdx, glm::mat4 transformation);

	// Returns the nodes vector
	std::vector<SceneNode> const& getNodes() const;

	// Recomputes the model matrix of each node i.e. multiplies the local transformation of 
	// each nodes with the transformations of all its parents.
	void update();

private:
	// Recursively updates the model matrices of all child nodes
	void updateModelMatrix(size_t nodeIdx);
};
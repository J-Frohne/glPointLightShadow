#include "scene/sceneGraph.hpp"


SceneGraph::SceneGraph()
{
	// Create the root node
	SceneNode node = {};
	node.index = 0;
	node.parent = 0;
	node.nodeMatrix = glm::mat4(1.f);
	node.modelMatrix = glm::mat4(1.f);
	node.castsShadow = true;

	// Add the root node
	m_nodes.push_back(node);
}

SceneGraph::~SceneGraph()
{
}

Mesh* SceneGraph::takeMesh(std::unique_ptr<Mesh> mesh)
{
	m_meshes.push_back(std::move(mesh));

	return m_meshes.back().get();
}

std::vector<Mesh*> SceneGraph::takeMeshes(std::vector<std::unique_ptr<Mesh>>& meshes)
{
	std::vector<Mesh*> references;
	references.reserve(meshes.size());

	for (auto& mesh : meshes)
	{
		m_meshes.push_back(std::move(mesh));
		references.push_back(m_meshes.back().get());
	}

	return references;
}

Material* SceneGraph::takeMaterial(std::unique_ptr<Material> material)
{
	m_materials.push_back(std::move(material));

	return m_materials.back().get();
}

std::vector<Material*> SceneGraph::takeMaterials(std::vector<std::unique_ptr<Material>>& materials)
{
	std::vector<Material*> references;
	references.reserve(materials.size());

	for (auto& material : materials)
	{
		m_materials.push_back(std::move(material));
		references.push_back(m_materials.back().get());
	}

	return references;
}

size_t SceneGraph::addNode(size_t parent, bool castsShadow, glm::mat4 initialTransformation)
{
	// Create the new node
	SceneNode node = {};
	node.index = m_nodes.size();
	node.parent = parent;
	node.nodeMatrix = initialTransformation;
	node.modelMatrix = glm::mat4(1.f);
	node.castsShadow = castsShadow;

	// Add the node to the parents childes
	m_nodes[parent].children.push_back(node.index);

	// Add the new node to the array
	m_nodes.push_back(node);

	return node.index;
}

void SceneGraph::addNodeMesh(size_t nodeIdx, Mesh* mesh)
{
	m_nodes[nodeIdx].meshes.push_back(mesh);
}

void SceneGraph::addNodeMeshes(size_t nodeIdx, std::vector<Mesh*>& meshes)
{
	for (auto mesh : meshes)
	{
		m_nodes[nodeIdx].meshes.push_back(mesh);
	}
}

void SceneGraph::setNodeTransformation(size_t nodeIdx, glm::mat4 transformation)
{
	m_nodes[nodeIdx].nodeMatrix = transformation;
}

std::vector<SceneGraph::SceneNode> const& SceneGraph::getNodes() const
{
	return m_nodes;
}

void SceneGraph::update()
{
	// Update root node
	SceneNode& root = m_nodes[0];
	root.modelMatrix = root.nodeMatrix;

	// Update children recursively
	for (size_t child : root.children)
	{
		updateModelMatrix(child);
	}
}

void SceneGraph::updateModelMatrix(size_t nodeIdx)
{
	// Update the transformation of this node
	SceneNode& node = m_nodes[nodeIdx];
	SceneNode& parent = m_nodes[node.parent];
	node.modelMatrix = parent.modelMatrix * node.nodeMatrix;

	// Update the transformation of all its children
	for (size_t childrenIdx : node.children)
	{
		updateModelMatrix(childrenIdx);
	}
}
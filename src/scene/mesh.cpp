#include "log.hpp"
#include "glUtil.hpp"
#include "scene/mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

Mesh::Mesh(std::vector<Vertex> const& vertices, std::vector<uint32_t> const& indices, Material const& material)
	: numVertices(static_cast<uint32_t>(vertices.size()))
	, numIndices(static_cast<uint32_t>(indices.size()))
	, material(material)
	, vertexArrayObject(Vertex::createVertexArrayObject(vertices, indices))
{
}

Mesh::Mesh(Mesh&& other)
	: numVertices(other.numVertices)
	, numIndices(other.numIndices)
	, material(other.material)
	, vertexArrayObject(other.vertexArrayObject)
{
	other.vertexArrayObject = 0;
}

Mesh::~Mesh()
{
	if (vertexArrayObject != 0)
	{
		glDeleteVertexArrays(1, &vertexArrayObject);
	}
}

void Mesh::readObj(
	std::string directory, 
	std::string filename, 
	std::vector<std::unique_ptr<Mesh>>& outMeshes, 
	std::vector<std::unique_ptr<Material>>& outMaterials)
{
	// Container for tinyObjLoader to write to
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warning, error;
	
	// Path of the obj file
	std::string path = directory + "/" + filename;
	SPDLOG_INFO("Reading scene from file \"{}\"...", path);

	// Load the obj with tinyObjLoader
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, path.c_str() , directory.c_str()))
	{
		throw std::runtime_error(warning + error);
	}

	// Check for errors
	if (error != "")
	{
		SPDLOG_WARN("TinyObjLoader error: {}", error);
	}
	if (warning != "")
	{
		SPDLOG_WARN("TinyObjLoader warning: {}", warning);
	}

	// Create Materials
	for (int i = 0; i < materials.size(); i++) 
	{
		glm::vec4  Ka = glm::vec4(materials[i].ambient [0], materials[i].ambient [1], materials[i].ambient [2], materials[i].ambient [2]);
		glm::vec4  Kd = glm::vec4(materials[i].diffuse [0], materials[i].diffuse [1], materials[i].diffuse [2], materials[i].diffuse [2]);
		glm::vec4  Ks = glm::vec4(materials[i].specular[0], materials[i].specular[1], materials[i].specular[2], materials[i].specular[2]);
		float      Ns = materials[i].shininess;
		float      d  = materials[i].dissolve;

		// Create the material
		auto mat = std::make_unique<Material>(Ka, Kd, Ks, Ns, d);

		// Create textures if present
		if (!materials[i].ambient_texname.empty()) 
		{
			std::string texturePath = directory + "/" + materials[i].ambient_texname;
			mat->textureKa = glUtil::createTexture(texturePath);
		}

		if (!materials[i].diffuse_texname.empty())
		{
			std::string texturePath = directory + "/" + materials[i].diffuse_texname;
			mat->textureKd = glUtil::createTexture(texturePath);
		}

		if (!materials[i].specular_texname.empty())
		{
			std::string texturePath = directory + "/" + materials[i].specular_texname;
			mat->textureKs = glUtil::createTexture(texturePath);
		}

		// Push the material into the vector
		outMaterials.push_back(std::move(mat));
	}
	
	// Iterate trough all the shapes/meshes
	SPDLOG_TRACE("Processing vertex data... ");
	for (const tinyobj::shape_t &shape : shapes)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned> indices;
	
		// Stores the index of each unique vertex. Used to fill the indices array
		std::unordered_map<Vertex, unsigned> uniqueVertices;
	
		// Iterate through the faces within the mesh
		size_t numVerticesInPreviousFaces = 0;
		for (size_t faceIndex = 0; faceIndex < shape.mesh.num_face_vertices.size(); ++faceIndex)
		{
			// Iterate through all the vertices inside the face
			int faceVertices = shape.mesh.num_face_vertices[faceIndex];
			for (size_t vertexIndex = 0; vertexIndex < faceVertices; ++vertexIndex)
			{
				// Get the index into the attrib array
				tinyobj::index_t index = shape.mesh.indices[numVerticesInPreviousFaces + vertexIndex];
	
				// Read the vertex attributes
				tinyobj::real_t vx =    attrib.vertices[3 * index.vertex_index + 0];
				tinyobj::real_t vy =    attrib.vertices[3 * index.vertex_index + 1];
				tinyobj::real_t vz =    attrib.vertices[3 * index.vertex_index + 2];
				tinyobj::real_t nx =    attrib.normals[3 * index.normal_index + 0];
				tinyobj::real_t ny =    attrib.normals[3 * index.normal_index + 1];
				tinyobj::real_t nz =    attrib.normals[3 * index.normal_index + 2];
				tinyobj::real_t tx =    attrib.texcoords[2 * index.texcoord_index + 0];
				tinyobj::real_t ty =    1.f - attrib.texcoords[2 * index.texcoord_index + 1];
				tinyobj::real_t red =   attrib.colors[3 * index.vertex_index + 0];
				tinyobj::real_t green = attrib.colors[3 * index.vertex_index + 1];
				tinyobj::real_t blue =  attrib.colors[3 * index.vertex_index + 2];
	
				// Create the vertex
				Vertex vertex(glm::vec3(vx, vy, vz), glm::vec3(nx, ny, nz), glm::vec2(tx, ty), glm::vec4(red, green, blue, 1.f));
	
				// Check if this vertex was used before
				if (uniqueVertices.count(vertex) == 0)
				{
					// If not push it into the vector and store its index in uniqueVertices
					uniqueVertices[vertex] = static_cast<unsigned>(vertices.size());
					vertices.push_back(vertex);
				}
	
				// Push the index of this vertex into the indices vector
				indices.push_back(uniqueVertices[vertex]);
			}
			numVerticesInPreviousFaces += faceVertices;
		}
	
		// Obj stores per face materials. We will only use one material per mesh.
		// Use the material of the first face.
		int materialIndex = shape.mesh.material_ids[0];

		// Create the mesh
		auto mesh = std::make_unique<Mesh>(vertices, indices, *outMaterials[materialIndex]);
		outMeshes.push_back(std::move(mesh));
	}
}
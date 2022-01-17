#include "scene/primitive.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


void createPlane(
	float width,
	float depth,
	std::vector<Vertex>& vertices,
	std::vector<unsigned>& indices)
{
	vertices.reserve(4);
	vertices.emplace_back(glm::vec3(-width / 2, 0, -depth / 2), glm::vec3(0 ,1, 0), glm::vec2(0, 0));
	vertices.emplace_back(glm::vec3(-width / 2, 0,  depth / 2), glm::vec3(0, 1, 0), glm::vec2(0, 1));
	vertices.emplace_back(glm::vec3( width / 2, 0, -depth / 2), glm::vec3(0, 1, 0), glm::vec2(1, 0));
	vertices.emplace_back(glm::vec3( width / 2, 0,  depth / 2), glm::vec3(0, 1, 0), glm::vec2(1, 1));

	indices.reserve(6);
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);
}

void createCube(
	float width,
	std::vector<Vertex>& vertices,
	std::vector<unsigned>& indices)
{
	// Reserve memory to prevent reallocating
	vertices.reserve(4 * 6);
	indices.reserve(6 * 6);

	// bottom
	vertices.emplace_back(glm::vec3(-width / 2, -width / 2, -width / 2), glm::vec3(0, -1, 0), glm::vec2(1, 1));
	vertices.emplace_back(glm::vec3( width / 2, -width / 2, -width / 2), glm::vec3(0, -1, 0), glm::vec2(0, 1));
	vertices.emplace_back(glm::vec3(-width / 2, -width / 2,  width / 2), glm::vec3(0, -1, 0), glm::vec2(1, 0));
	vertices.emplace_back(glm::vec3( width / 2, -width / 2,  width / 2), glm::vec3(0, -1, 0), glm::vec2(0, 0));

	// top
	vertices.emplace_back(glm::vec3(-width / 2,  width / 2, -width / 2), glm::vec3(0,  1, 0), glm::vec2(1, 0));
	vertices.emplace_back(glm::vec3( width / 2,  width / 2, -width / 2), glm::vec3(0,  1, 0), glm::vec2(0, 0));
	vertices.emplace_back(glm::vec3(-width / 2,  width / 2,  width / 2), glm::vec3(0,  1, 0), glm::vec2(1, 1));
	vertices.emplace_back(glm::vec3( width / 2,  width / 2,  width / 2), glm::vec3(0,  1, 0), glm::vec2(0, 1));

	// front
	vertices.emplace_back(glm::vec3(-width / 2, -width / 2,  width / 2), glm::vec3(0, 0,  1), glm::vec2(0, 1));
	vertices.emplace_back(glm::vec3( width / 2, -width / 2,  width / 2), glm::vec3(0, 0,  1), glm::vec2(1, 1));
	vertices.emplace_back(glm::vec3(-width / 2,  width / 2,  width / 2), glm::vec3(0, 0,  1), glm::vec2(0, 0));
	vertices.emplace_back(glm::vec3( width / 2,  width / 2,  width / 2), glm::vec3(0, 0,  1), glm::vec2(1, 0));

	// back
	vertices.emplace_back(glm::vec3(-width / 2, -width / 2, -width / 2), glm::vec3(0, 0, -1), glm::vec2(1, 1));
	vertices.emplace_back(glm::vec3( width / 2, -width / 2, -width / 2), glm::vec3(0, 0, -1), glm::vec2(0, 1));
	vertices.emplace_back(glm::vec3(-width / 2,  width / 2, -width / 2), glm::vec3(0, 0, -1), glm::vec2(1, 0));
	vertices.emplace_back(glm::vec3( width / 2,  width / 2, -width / 2), glm::vec3(0, 0, -1), glm::vec2(0, 0));

	// left
	vertices.emplace_back(glm::vec3(-width / 2, -width / 2, -width / 2), glm::vec3(-1, 0, 0), glm::vec2(0, 1));
	vertices.emplace_back(glm::vec3(-width / 2, -width / 2,  width / 2), glm::vec3(-1, 0, 0), glm::vec2(1, 1));
	vertices.emplace_back(glm::vec3(-width / 2,  width / 2, -width / 2), glm::vec3(-1, 0, 0), glm::vec2(0, 0));
	vertices.emplace_back(glm::vec3(-width / 2,  width / 2,  width / 2), glm::vec3(-1, 0, 0), glm::vec2(1, 0));
	
	// right
	vertices.emplace_back(glm::vec3( width / 2, -width / 2, -width / 2), glm::vec3( 1, 0, 0), glm::vec2(1, 1));
	vertices.emplace_back(glm::vec3( width / 2, -width / 2,  width / 2), glm::vec3( 1, 0, 0), glm::vec2(0, 1));
	vertices.emplace_back(glm::vec3( width / 2,  width / 2, -width / 2), glm::vec3( 1, 0, 0), glm::vec2(1, 0));
	vertices.emplace_back(glm::vec3( width / 2,  width / 2,  width / 2), glm::vec3( 1, 0, 0), glm::vec2(0, 0));
	
	for (unsigned i = 0; i < 6; i += 2)
	{
		indices.push_back(i * 4 + 0);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 3);
		indices.push_back(i * 4 + 2);

		indices.push_back((i + 1) * 4 + 2);
		indices.push_back((i + 1) * 4 + 1);
		indices.push_back((i + 1) * 4 + 0);
		indices.push_back((i + 1) * 4 + 2);
		indices.push_back((i + 1) * 4 + 3);
		indices.push_back((i + 1) * 4 + 1);
	}	
}

void createSphere (
	float radius, 
	unsigned subdivisions_height, 
	unsigned subdivisions_axis, 
	std::vector<Vertex>& vertices, 
	std::vector<unsigned>& indices)
{
	// Texture map looks like this: https://upload.wikimedia.org/wikipedia/commons/0/04/UVMapping.png
	
	// Reserve memory to prevent reallocating
	vertices.reserve(subdivisions_height * (subdivisions_axis + 1) - 2);
	indices.reserve(6 * subdivisions_axis * subdivisions_height + subdivisions_height - 2);

	// Create vertices of the top pole
	for (unsigned int i = 0; i < subdivisions_axis; ++i)
	{
		// All vertices added in this loop have the same position and normal. 
		// Only the texture coordinate varies.
		
		// Vertex attributes
		glm::vec3 normal = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 pos = radius * normal;
		glm::vec2 texCoords = glm::vec2(
			(static_cast<float>(i) + 0.5f) / static_cast<float>(subdivisions_axis),
			0.f
		);
		vertices.emplace_back(pos, normal, texCoords);

		// Indices
		unsigned i1 = i; // Index to the vertex created in this iteration
		unsigned i2 = i + subdivisions_axis;	 // Index to an inner vertex
		unsigned i3 = i + 1 + subdivisions_axis; // Index to the following inner vertex
		indices.push_back(i1);
		indices.push_back(i2);
		indices.push_back(i3);
	}

	// Create all inner vertices
	for (unsigned int v = 1; v <= subdivisions_height - 1; ++v)
	{
		// Polar angle 0° < theta < 180°
		float theta = v * glm::pi<float>() / subdivisions_height;
		
		for (unsigned int h = 0; h <= subdivisions_axis; ++h)
		{
			// Azimuth angle 0° <= phi <= 360°
			float phi = h * 2 * glm::pi<float>() / subdivisions_axis; 
			
			// Note: phi = 0° and phi = 360° correspond to the same vertex 
			// position but different texture coordinates

			// Vertex attributes
			glm::vec3 normal = glm::vec3(
				glm::sin(theta) * glm::sin(phi),
				glm::cos(theta), 
				glm::sin(theta) * glm::cos(phi)
			);
			glm::vec3 pos = radius * normal;
			glm::vec2 texCoords = glm::vec2(
				static_cast<float>(h) / static_cast<float>(subdivisions_axis),
				static_cast<float>(v) / static_cast<float>(subdivisions_height)
			);
			vertices.emplace_back(pos, normal, texCoords);

			// Indices
			if (v != subdivisions_height - 1 && h != subdivisions_axis) // skip adding indices in the last iteration of either loop
			{
				// Notes:
				// - The offset (= subdivisions_axis) comes form the number of vertices added for the top pole
				// - h starts at 0 and v starts at 1
				
				//           |Azimuth| |-------------Polar---------------| |------Offset-----|
				unsigned i1 = (h + 0) + (v - 1) * (subdivisions_axis + 1) + subdivisions_axis;
				unsigned i2 = (h + 1) + (v - 1) * (subdivisions_axis + 1) + subdivisions_axis;
				unsigned i3 = (h + 0) + (v - 0) * (subdivisions_axis + 1) + subdivisions_axis;
				unsigned i4 = (h + 1) + (v - 0) * (subdivisions_axis + 1) + subdivisions_axis;

				// Push two triangles
				indices.push_back(i1);
				indices.push_back(i4);
				indices.push_back(i2);

				indices.push_back(i1);
				indices.push_back(i3);
				indices.push_back(i4);
			}
		}
	}

	// Create vertices of the bottom pole
	for (unsigned int i = 0; i < subdivisions_axis; ++i)
	{
		// All vertices added in this loop have the same position and normal. 
		// Only the texture coordinate varies.

		// Vertex attributes
		glm::vec3 normal = glm::vec3(0.f, -1.f, 0.f);
		glm::vec3 pos = radius * normal;
		glm::vec2 texCoords = glm::vec2(
			(static_cast<float>(i) + 0.5f) / static_cast<float>(subdivisions_axis),
			1.0f
		);
		vertices.emplace_back(pos, normal, texCoords);

		// Indices
		unsigned i1 = i + (subdivisions_axis + 1) * (subdivisions_height - 1) - 1;
		unsigned i2 = i + (subdivisions_axis + 1) * (subdivisions_height - 1);
		unsigned i3 = i + (subdivisions_axis + 1) * (subdivisions_height - 1) + subdivisions_axis;
		indices.push_back(i1);
		indices.push_back(i3);
		indices.push_back(i2);
	}
}

void createTorus(
	float radius,
	float sectionRadius,
	float twist,
	unsigned subdivisions_axis,
	unsigned subdivisions_height,
	std::vector<Vertex>& vertices,
	std::vector<unsigned>& indices)
{
	// Reserve memory to prevent reallocating
	vertices.reserve((subdivisions_axis + 1) * (subdivisions_height + 1));
	indices.reserve(6 * subdivisions_axis * subdivisions_height);

	for (unsigned i = 0; i <= subdivisions_axis; ++i)
	{
		// Iterate through angles 0° <= phi <= 360°
		float phi = i * 2.f * glm::pi<float>() / subdivisions_axis;

		// Direction from the origin to a point on the central circle within the
		// torus
		glm::vec3 vecCircle(
			glm::cos(phi), 
			0.0f, 
			glm::sin(phi)
		);

		for (unsigned j = 0; j <= subdivisions_height; ++j)
		{
			// Iterate through angles 0° <= theta <= 360°
			float theta = twist + j * 2.f * glm::pi<float>() / subdivisions_axis;
			
			// Direction from the point on the central circle within the torus 
			// to a point on the surface of the torus
			glm::vec3 vecSurface(
				glm::sin(theta) * glm::cos(phi), 
				glm::cos(theta), 
				glm::sin(theta) * glm::sin(phi));

			// Vertex attributes
			glm::vec3 vertexPosition = radius * vecCircle + sectionRadius * vecSurface;
			glm::vec3 vertexNormal = vecSurface;
			glm::vec2 vertexTexCoords = glm::vec2(
				static_cast<float>(i) / static_cast<float>(subdivisions_axis),
				static_cast<float>(j) / static_cast<float>(subdivisions_height)
			);
			vertices.emplace_back(vertexPosition, vertexNormal, vertexTexCoords);

			if (i == subdivisions_axis || j == subdivisions_height)
			{
				continue;
			}

			// Indices
			unsigned i0 = (i + 0) * (subdivisions_axis + 1) + (j + 0);
			unsigned i1 = (i + 0) * (subdivisions_axis + 1) + (j + 1);
			unsigned i2 = (i + 1) * (subdivisions_axis + 1) + (j + 0);
			unsigned i3 = (i + 1) * (subdivisions_axis + 1) + (j + 1);

			// Push two triangles
			indices.push_back(i0);
			indices.push_back(i2);
			indices.push_back(i1);

			indices.push_back(i1);
			indices.push_back(i2);
			indices.push_back(i3);
		}
	}
}

void createRing(
	float innerRadius,
	float outerRadius,
	unsigned subdivisions_axis,
	std::vector<Vertex>& vertices,
	std::vector<unsigned>& indices)
{
	// Reserve memory to prevent reallocating
	vertices.reserve(2 * (subdivisions_axis + 1));
	indices.reserve(6 * subdivisions_axis);

	for (unsigned i = 0; i <= subdivisions_axis; ++i)
	{
		// Iterate through angles 0° <= phi <= 360°
		float phi = i * 2.f * glm::pi<float>() / subdivisions_axis;

		// Vertex attributes
		glm::vec3 posUnitCircle(
			glm::cos(phi), 
			0.0f, 
			glm::sin(phi)
		);
		glm::vec3 normal(0.0f, 1.0f, 0.0f);
		float texCoordT = static_cast<float>(i) / static_cast<float>(subdivisions_axis);

		// Create one vertex on the inner edge and one on the outer edge
		vertices.emplace_back(innerRadius * posUnitCircle, normal, glm::vec2(0.0f, texCoordT));
		vertices.emplace_back(outerRadius * posUnitCircle, normal, glm::vec2(1.0f, texCoordT));

		// Indices
		if (i != subdivisions_axis) // skip adding indices in the last iteration
		{
			unsigned i0 = 2 * (i + 0) + 0; // index to a vertex an the inner edge of the ring
			unsigned i1 = 2 * (i + 0) + 1; // index to a vertex on the outer edge of the ring
			unsigned i2 = 2 * (i + 1) + 0; // index to the following vertex on the inner edge
			unsigned i3 = 2 * (i + 1) + 1; // index to the following vertex on the outer edge

			// Push two triangles
			indices.push_back(i0);
			indices.push_back(i2);
			indices.push_back(i1);

			indices.push_back(i1);
			indices.push_back(i2);
			indices.push_back(i3);
		}
	}
}

void createCone (
	float angle,
	float height,
	unsigned subdivisions_axis,
	std::vector<Vertex>& vertices,
	std::vector<unsigned>& indices)
{
	// Reserve memory to prevent reallocating
	vertices.reserve(subdivisions_axis + 3);
	indices.reserve(subdivisions_axis * 6);

	// Calculate radius of the base from the apex angle
	float radius = glm::tan(angle / 2.f) * height;

	// Bottom pole vertex
	vertices.emplace_back(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f)
	);

	// Edge of the base 
	for (unsigned i = 0; i <= subdivisions_axis; ++i)
	{
		// Iterate through angles 0° <= phi <= 360°
		float phi = i * 2 * glm::pi<float>() / subdivisions_axis;

		// Vertex attributes
		glm::vec3 pos(
			radius * glm::cos(phi), 
			height, 
			radius * glm::sin(phi)
		);
		glm::vec3 normal(0.0f, 1.0f, 0.0f);
		glm::vec2 texCoord(0.0f, 0.0f);
		vertices.emplace_back(pos, normal, texCoord);

		// Indices
		if (i != subdivisions_axis) // skip adding indices in the last iteration
		{
			unsigned i0 = 0; // index of base/bottom pole
			unsigned i1 = i + 1; // index of a vertex on the edge of the base
			unsigned i2 = i + 2; // and to the following one
			unsigned i3 = subdivisions_axis + 2; // index of apex/top pole

			// Push two triangles
			indices.push_back(i0);
			indices.push_back(i1);
			indices.push_back(i2);

			indices.push_back(i3);
			indices.push_back(i2);
			indices.push_back(i1);
		}
	}

	// Top pole vertex
	vertices.emplace_back(
		glm::vec3(0.0f, height, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f)
	);
}
#pragma once

#include "scene/vertex.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>


void createPlane(
	float width,
	float depth,
	std::vector<Vertex>& vertices,
	std::vector<unsigned>& indices
);

void createCube(
	float width,
	std::vector<Vertex>& vertices,
	std::vector<unsigned>& indices
);

void createSphere (
	float radius,
	unsigned subdivisions_height,
	unsigned subdivisions_axis,
	std::vector<Vertex>& vertices,
	std::vector<unsigned>& indices
);

void createTorus(
	float radius,
	float sectionRadius,
	float twist,
	unsigned subdivisions_axis,
	unsigned subdivisions_height,
	std::vector<Vertex>& vertices,
	std::vector<unsigned>& indices
);

void createRing (
	float innerRadius,
	float outerRadius,
	unsigned subdivisions_axis,
	std::vector<Vertex>& vertices,
	std::vector<unsigned>& indices
);

 // Currently does not create proper normals nor texture coordinates.
 // All texture coordinates are set to (0,0).
void createCone (
	float angle,
	float height,
	unsigned subdivisions_axis,
	std::vector<Vertex>& vertices,
	std::vector<unsigned>& indices
);
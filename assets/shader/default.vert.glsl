#version 400

uniform mat4 projectionMatrix; // eye space -> clip coordinates
uniform mat4 viewMatrix; // world space -> eye space
uniform mat4 modelMatrix; // model space -> world space
uniform mat4 normalMatrix; // model space normal -> eye space normal

uniform vec3 lightPositionWorld; // position of the light in world space

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

smooth out vec4 vpos; // position in eye space
smooth out vec4 vposLightSpace; // position in light space
smooth out vec3 vnormal; // normal in eye space, not normalized
smooth out vec2 vtexCoords; // texture coordinates

void main(void)
{
	// The position in eye space.
	vpos = viewMatrix * modelMatrix * vec4(position, 1);
	
	// The normal in eye space.
	vnormal = mat3(normalMatrix) * normal;

	// The texture coordinates
	vtexCoords = texCoords;
	
	// The position in clip space
	gl_Position = projectionMatrix * vpos;
	
	// The position in light space
	vposLightSpace = modelMatrix * vec4(position, 1) - vec4(lightPositionWorld, 0);
}
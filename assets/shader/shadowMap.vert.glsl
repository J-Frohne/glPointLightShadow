#version 400

uniform mat4 modelViewProjection; // model space -> clip space

layout(location = 0) in vec3 position;

void main(void)
{
	// The position in clip space
    gl_Position = modelViewProjection * vec4(position, 1.0);
}
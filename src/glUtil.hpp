#pragma once

#include <GL/glew.h>

#include <glm/mat4x4.hpp>

#include <string>


namespace glUtil
{
	GLuint loadShader(char const *path, GLenum shaderType);
	GLuint linkShaders(GLuint vertexShader, GLuint fragmentShader);

	GLuint createTexture(std::string texturePath);
	GLuint createCubeMapDepth(GLsizei size);
	void createFramebufferDepth(GLsizei width, GLsizei height, GLuint& framebuffer, GLuint& depthBuffer);

	class ShadowMap
	{
	public:
		~ShadowMap();

		void init(GLsizei size);
		void recreate(GLsizei size);

		// Attaches the specified face of the cube map to the current framebuffer
		void setCubeMapFace(GLenum face) const;

		glm::mat4 getViewMatrix(GLenum face, glm::vec3 position) const;
		glm::mat4 getProjectionMatrix(float near, float far) const;

		GLuint m_framebuffer;
		GLuint m_depthBuffer;
		GLuint m_cubeMap;
		GLsizei m_size;

	private:
		glm::vec3 getViewDir(GLenum face) const;
		glm::vec3 getUpDir(GLenum face) const;
	};
}
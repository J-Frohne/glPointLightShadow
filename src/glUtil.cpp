#include "glUtil.hpp"

#include "log.hpp"

#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>


GLuint glUtil::loadShader(char const *path, GLenum shaderType)
{
	// Create a shader object
	GLuint shader = glCreateShader(shaderType);

	// Open shader file
	std::ifstream inputStream(path, std::ios::in);
	if (!inputStream.is_open()) 
	{
		SPDLOG_ERROR("Cannot open shader file \"{0}\"", path);

		return 0;
	}

	// Read shader code
	std::stringstream sstr;
	sstr << inputStream.rdbuf();
	std::string shaderCode = sstr.str();
	inputStream.close();

	// Compile shader
	SPDLOG_DEBUG("Compiling shader \"{0}\"", path);
	char const *pShaderCode = shaderCode.c_str();
	glShaderSource(shader, 1, &pShaderCode, NULL);
	glCompileShader(shader);

	// Check if compilation was successful
	GLint compileStatus = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	int errorLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLength);

	if (errorLength > 0) 
	{
		std::vector<char> errorMessage(errorLength + 1);
		glGetShaderInfoLog(shader, errorLength, NULL, &errorMessage[0]);

		SPDLOG_ERROR("Compilation of shader file \"{0}\" failed! \n{1}", path, &errorMessage[0]);
		std::cin.get();
	}

	return shader;
}

GLuint glUtil::linkShaders(GLuint vertexShader, GLuint fragmentShader)
{
	// Link the program
	SPDLOG_TRACE("Linking shader program. vertexShader={}, fragmentShader={}", vertexShader, fragmentShader);
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Check the program
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	int infoLogLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(program, infoLogLength, NULL, &ProgramErrorMessage[0]);
		
		SPDLOG_ERROR("Linking failed! \n{}", &ProgramErrorMessage[0]);
		std::cin.get();
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

GLuint glUtil::createTexture(std::string texturePath)
{
	// Read texture file
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(texturePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

	if (pixels == nullptr)
	{
		SPDLOG_ERROR("Cannot open texture file \"{0}\"", texturePath);
		return 0;
	}

	// Generate texture object
	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set the texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
	// Generate mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	// Set wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set filtering method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

GLuint glUtil::createCubeMapDepth(GLsizei size)
{
	// Generate and bind a new cube map texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	// Create empty 2D image for every face
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	// Set texture parameters
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

	// Set comparison mode
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	// Unbind cube map
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

void glUtil::createFramebufferDepth(GLsizei width, GLsizei height, GLuint& framebuffer, GLuint &depthBuffer)
{
	// Create and bind a framebuffer
	framebuffer = 0;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Set the color buffer to none
	glDrawBuffer(GL_NONE);

	// Generate and bind a new render buffer
	depthBuffer = 0;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);

	// Set up the render buffer as depth buffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	// Attach the depth buffer to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	// Check for errors
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		SPDLOG_ERROR("OpenGL FBO error {0:x}", status);
	}
}

glUtil::ShadowMap::~ShadowMap()
{
	if (m_framebuffer != 0)
	{
		glDeleteFramebuffers(1, &m_framebuffer);
	}

	if (m_depthBuffer != 0)
	{
		glDeleteRenderbuffers(1, &m_depthBuffer);
	}

	if (m_cubeMap != 0)
	{
		glDeleteTextures(1, &m_cubeMap);
	}
}

void glUtil::ShadowMap::init(GLsizei size)
{
	m_size = size;
	m_cubeMap = createCubeMapDepth(size);
	createFramebufferDepth(size, size, m_framebuffer, m_depthBuffer);
}

void glUtil::ShadowMap::recreate(GLsizei size)
{
	if (m_size == size)
	{
		return;
	}

	if (m_framebuffer != 0)
	{
		glDeleteFramebuffers(1, &m_framebuffer);
	}

	if (m_depthBuffer != 0)
	{
		glDeleteRenderbuffers(1, &m_depthBuffer);
	}

	if (m_cubeMap != 0)
	{
		glDeleteTextures(1, &m_cubeMap);
	}

	m_size = size;
	m_cubeMap = createCubeMapDepth(size);
	createFramebufferDepth(size, size, m_framebuffer, m_depthBuffer);
}

void glUtil::ShadowMap::setCubeMapFace(GLenum face) const
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, face, m_cubeMap, 0);
}

glm::mat4 glUtil::ShadowMap::getViewMatrix(GLenum face, glm::vec3 position) const
{
	glm::vec3 viewDirection = getViewDir(face);
	glm::vec3 upDirection = -getUpDir(face);

	glm::mat4 viewMatrix = glm::lookAt(position, position + viewDirection, upDirection);

	return viewMatrix;
}

glm::mat4 glUtil::ShadowMap::getProjectionMatrix(float near, float far) const
{
	return glm::perspective(glm::radians(90.0f), 1.0f, near, far);
}

glm::vec3 glUtil::ShadowMap::getViewDir(GLenum face) const
{
	switch (face)
	{
	default:
	case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
		return glm::vec3(1, 0, 0);

	case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
		return glm::vec3(-1, 0, 0);

	case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
		return glm::vec3(0, 1, 0);

	case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
		return glm::vec3(0, -1, 0);

	case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
		return glm::vec3(0, 0, 1);

	case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
		return glm::vec3(0, 0, -1);
	}
}

glm::vec3 glUtil::ShadowMap::getUpDir(GLenum face) const
{
	switch (face)
	{
	default:
	case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
		return glm::vec3(0, 1, 0);

	case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
		return glm::vec3(0, 1, 0);

	case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
		return glm::vec3(0, 0, -1);

	case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
		return glm::vec3(0, 0, 1);

	case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
		return glm::vec3(0, 1, 0);

	case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
		return glm::vec3(0, 1, 0);
	}
}

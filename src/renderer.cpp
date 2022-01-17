#include "renderer.hpp"

#include "log.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h> // Key definitions for input handling


Renderer::Renderer() 
	: m_shaderDefault(0)
	, m_shaderDefaultNoShadow(0)
	, m_shaderShadowMap(0)
	, m_shadowMap()
	, m_useShadowMap(true)
	, m_shadowCullFront(false)
	, m_shadowUsePolygonOffset(true)
	, m_shadowPolygonOffsetUnits(500.f)
	, m_shadowPolygonOffsetFactor(1.f)
	, m_input(nullptr)
{
}

Renderer::~Renderer()
{
	glDeleteShader(m_shaderDefault);
	glDeleteShader(m_shaderDefaultNoShadow);
	glDeleteShader(m_shaderShadowMap);
}

void Renderer::init(Input* input)
{
	m_input = input;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0, 0, 0, 1);

	GLuint vsDefault = glUtil::loadShader("assets/shader/default.vert.glsl", GL_VERTEX_SHADER);
	GLuint fsDefault = glUtil::loadShader("assets/shader/default.frag.glsl", GL_FRAGMENT_SHADER);
	m_shaderDefault = glUtil::linkShaders(vsDefault, fsDefault);

	GLuint vsDefaultNoShadow = glUtil::loadShader("assets/shader/defaultNoShadow.vert.glsl", GL_VERTEX_SHADER);
	GLuint fsDefaultNoShadow = glUtil::loadShader("assets/shader/defaultNoShadow.frag.glsl", GL_FRAGMENT_SHADER);
	m_shaderDefaultNoShadow = glUtil::linkShaders(vsDefaultNoShadow, fsDefaultNoShadow);

	GLuint vsShadow = glUtil::loadShader("assets/shader/shadowMap.vert.glsl", GL_VERTEX_SHADER);
	GLuint fsShadow = glUtil::loadShader("assets/shader/shadowMap.frag.glsl", GL_FRAGMENT_SHADER);
	m_shaderShadowMap = glUtil::linkShaders(vsShadow, fsShadow);

	m_shadowMap.init(2048);
}

void Renderer::update()
{
	// Input handling for shadow map debugging
	if (m_input->isPushed(GLFW_KEY_0))
	{
		m_useShadowMap = true;
		m_shadowCullFront = false;
		m_shadowUsePolygonOffset = true;
		m_shadowPolygonOffsetUnits = 500.f;
		m_shadowPolygonOffsetFactor = 1.f;
		m_shadowMap.recreate(2048);

		SPDLOG_DEBUG("Shadow map enabled");
		SPDLOG_DEBUG("Culling back face during shadow pass");
		SPDLOG_DEBUG("glPolygonOffset enabled");
		SPDLOG_DEBUG("uglPolygonOffset parameter: units = {}", m_shadowPolygonOffsetUnits);
		SPDLOG_DEBUG("glPolygonOffset parameter: factor = {}", m_shadowPolygonOffsetFactor);
		SPDLOG_DEBUG("Shadow map size = {}", m_shadowMap.m_size);
	}
	if (m_input->isPushed(GLFW_KEY_V))
	{
		m_useShadowMap = !m_useShadowMap;
		if (m_useShadowMap)
		{
			SPDLOG_DEBUG("Shadow map enabled");
		}
		else
		{
			SPDLOG_DEBUG("Shadow map disabled");
		}
	}
	if (m_input->isPushed(GLFW_KEY_B))
	{
		m_shadowCullFront = !m_shadowCullFront;
		if (m_shadowCullFront) 
		{ 
			SPDLOG_DEBUG("Culling front face during shadow pass"); 
		}
		else                   
		{ 
			SPDLOG_DEBUG("Culling back face during shadow pass");  
		}
	}
	if (m_input->isPushed(GLFW_KEY_N))
	{
		m_shadowUsePolygonOffset = !m_shadowUsePolygonOffset;
		if (m_shadowUsePolygonOffset)
		{
			SPDLOG_DEBUG("glPolygonOffset enabled");
		}
		else
		{
			SPDLOG_DEBUG("glPolygonOffset disabled");
		}
	}
	if (m_input->isPushed(GLFW_KEY_1))
	{
		m_shadowPolygonOffsetUnits -= 100.f;
		SPDLOG_DEBUG("glPolygonOffset parameter: units = {}", m_shadowPolygonOffsetUnits);
	}
	if (m_input->isPushed(GLFW_KEY_2))
	{
		m_shadowPolygonOffsetUnits += 100.f;
		SPDLOG_DEBUG("glPolygonOffset parameter: units = {}", m_shadowPolygonOffsetUnits);
	}
	if (m_input->isPushed(GLFW_KEY_3))
	{
		m_shadowPolygonOffsetFactor -= 1.0f;
		SPDLOG_DEBUG("glPolygonOffset parameter: factor = {}", m_shadowPolygonOffsetFactor);
	}
	if (m_input->isPushed(GLFW_KEY_4))
	{
		m_shadowPolygonOffsetFactor += 1.0f;
		SPDLOG_DEBUG("glPolygonOffset parameter: factor = {}", m_shadowPolygonOffsetFactor);
	}
	if (m_input->isPushed(GLFW_KEY_5))
	{
		if (m_shadowMap.m_size > 256)
		{
			m_shadowMap.recreate(m_shadowMap.m_size / 2);
		}
		SPDLOG_DEBUG("Shadow map size = {}", m_shadowMap.m_size);
	}
	if (m_input->isPushed(GLFW_KEY_6))
	{
		if (m_shadowMap.m_size < 4096)
		{
			m_shadowMap.recreate(m_shadowMap.m_size * 2);
		}
		SPDLOG_DEBUG("Shadow map size = {}", m_shadowMap.m_size);
	}
}

void Renderer::render(
	SceneGraph const& scene,
	LightSource const& light,
	glm::mat4 viewMatrix,
	float vfov,
	int width,
	int height,
	float near,
	float far
) const
{
	renderShadowPass(scene, light, near, far);
	renderLightPass(scene, light, viewMatrix, vfov, width, height, near, far);
}

void Renderer::renderShadowPass(
	SceneGraph const& scene, 
	LightSource const& light, 
	float near,
	float far
) const
{
	if (!m_useShadowMap)
	{
		return;
	}

	// Load program
	glUseProgram(m_shaderShadowMap);

	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMap.m_framebuffer);
	glViewport(0, 0, m_shadowMap.m_size, m_shadowMap.m_size);

	if (m_shadowUsePolygonOffset)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(m_shadowPolygonOffsetFactor, m_shadowPolygonOffsetUnits);
	}

	if (m_shadowCullFront)
	{
		glCullFace(GL_FRONT);
	}
	else
	{
		glCullFace(GL_BACK);
	}

	for (GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X; face <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; ++face)
	{
		// Bind texture and clear depth buffer
		m_shadowMap.setCubeMapFace(face);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Render the scene
		glm::mat4 viewProjection = m_shadowMap.getProjectionMatrix(near, far) * 
			                       m_shadowMap.getViewMatrix(face, light.position);

		for (SceneGraph::SceneNode const& node : scene.getNodes())
		{
			if (!node.castsShadow)
			{
				continue;
			}

			for (Mesh const* mesh : node.meshes)
			{
				// Bind vertex array object
				glBindVertexArray(mesh->vertexArrayObject);

				// Pass uniforms
				glm::mat4 modelViewProjection = viewProjection * node.modelMatrix;
				glUniformMatrix4fv(glGetUniformLocation(m_shaderShadowMap, "modelViewProjection"), 1, GL_FALSE, glm::value_ptr(modelViewProjection));

				// Draw the mesh
				glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, 0);
			}
		}
	}

	if (m_shadowUsePolygonOffset)
	{
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
}

void Renderer::renderLightPass(
	SceneGraph const& scene, 
	LightSource const& light, 
	glm::mat4 viewMatrix, 
	float vfov,
	int width,
	int height,
	float near,
	float far
) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Set GL state
	glCullFace(GL_BACK);
	glEnable(GL_MULTISAMPLE);

	// Load program
	GLuint currentShader = m_useShadowMap ? m_shaderDefault : m_shaderDefaultNoShadow;
	glUseProgram(currentShader);

	if (m_useShadowMap)
	{
		// Shadow map texture (unit 0)
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap.m_cubeMap);
		glUniform1i(glGetUniformLocation(currentShader, "shadowMap"), 0);

		// Shadow map uniforms
		glUniform3fv(glGetUniformLocation(currentShader, "lightPositionWorld"), 1, glm::value_ptr(light.position));
		glUniformMatrix4fv(glGetUniformLocation(currentShader, "shadowMapProjection"), 1, GL_FALSE, glm::value_ptr(m_shadowMap.getProjectionMatrix(near, far)));
	}

	// Projection matrix uniform
	glm::mat4 projectionMatrix = glm::perspective(
		vfov,
		static_cast<float>(width) / static_cast<float>(height),
		near, far
	);
	glUniformMatrix4fv(glGetUniformLocation(currentShader, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Light uniforms
	glUniform3fv(glGetUniformLocation(currentShader, "lightPosition"), 1, glm::value_ptr(glm::vec3(viewMatrix * glm::vec4(light.position, 1.0f))));
	glUniform4fv(glGetUniformLocation(currentShader, "Ia"), 1, glm::value_ptr(light.Ia));
	glUniform4fv(glGetUniformLocation(currentShader, "Id"), 1, glm::value_ptr(light.Id));
	glUniform4fv(glGetUniformLocation(currentShader, "Is"), 1, glm::value_ptr(light.Is));

	for (SceneGraph::SceneNode const& node : scene.getNodes())
	{
		for (Mesh const* mesh : node.meshes)
		{
			glBindVertexArray(mesh->vertexArrayObject);

			// Transformation matrices
			glm::mat4 const& modelMatrix = node.modelMatrix;
			glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
			glm::mat4 normalMatrix = glm::inverse(glm::transpose(modelViewMatrix));
			glUniformMatrix4fv(glGetUniformLocation(currentShader, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
			glUniformMatrix4fv(glGetUniformLocation(currentShader, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(glGetUniformLocation(currentShader, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

			// Material uniforms
			Material const& material = mesh->material;
			glUniform4fv(glGetUniformLocation(currentShader, "ka"), 1, glm::value_ptr(material.Ka));
			glUniform4fv(glGetUniformLocation(currentShader, "kd"), 1, glm::value_ptr(material.Kd));
			glUniform4fv(glGetUniformLocation(currentShader, "ks"), 1, glm::value_ptr(material.Ks));
			glUniform1f(glGetUniformLocation(currentShader, "hasTexKa"), material.textureKa != 0);
			glUniform1f(glGetUniformLocation(currentShader, "hasTexKd"), material.textureKd != 0);
			glUniform1f(glGetUniformLocation(currentShader, "hasTexKs"), material.textureKs != 0);
			glUniform1f(glGetUniformLocation(currentShader, "shininess"), material.Ns);

			// Material textures
			glUniform1i(glGetUniformLocation(currentShader, "texKa"), 1);
			glUniform1i(glGetUniformLocation(currentShader, "texKd"), 2);
			glUniform1i(glGetUniformLocation(currentShader, "texKs"), 3);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, material.textureKa);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, material.textureKd);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, material.textureKs);

			// Draw the mesh
			glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, 0);
		}
	}

	glDisable(GL_MULTISAMPLE);
}

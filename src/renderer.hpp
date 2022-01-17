#pragma once

#include "input.hpp"
#include "glUtil.hpp"

#include "scene/sceneGraph.hpp"
#include "scene/lightSource.hpp"


class Renderer
{
public:
	Renderer();
	~Renderer();

	void init(Input* input);
	void update();
	void render(
		SceneGraph const& scene, 
		LightSource const& light, 
		glm::mat4 viewMatrix, 
		float vfov,
		int width, 
		int height,
		float near, 
		float far
	) const;

private:
	void renderShadowPass(
		SceneGraph const& scene,
		LightSource const& light,
		float near,
		float far
	) const;

	void renderLightPass(
		SceneGraph const& scene,
		LightSource const& light,
		glm::mat4 viewMatrix,
		float vfov,
		int width,
		int height,
		float near,
		float far
	) const;


private:
	GLuint m_shaderDefault;
	GLuint m_shaderDefaultNoShadow;
	GLuint m_shaderShadowMap;

	glUtil::ShadowMap m_shadowMap;
	bool m_useShadowMap;
	bool m_shadowCullFront;
	bool m_shadowUsePolygonOffset;
	GLfloat m_shadowPolygonOffsetFactor;
	GLfloat m_shadowPolygonOffsetUnits;

	Input* m_input;
};
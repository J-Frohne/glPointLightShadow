#pragma once

#include "input.hpp"
#include "camera.hpp"
#include "renderer.hpp"
#include "scene/sceneGraph.hpp"
#include "gameObject/gameObject.hpp"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <string>
#include <memory>


class MainApplication
{
public:
	MainApplication();
	~MainApplication();

	void init();
	void run();

private:
	void createScene();

	void update(float deltaTimeSeconds);
	void render() const;

	void setFullscreen(bool fullscreen);
	void meassueFPS(float deltaTimeSeconds);
	void printControls() const;

	// Window attributes
	GLFWwindow* m_window; // handle to the GLFW window
	std::string m_windowTitle;
	int m_windowWidth;
	int m_windowHeight;
	bool m_windowIsFullscreen;
	bool m_windowIsIconified;
	bool m_windowVSync;

	// Information to restore window size and position after exiting fullscreen
	int m_windowOldWidth;
	int m_windowOldHeight;
	int m_windowOldPosX;
	int m_windowOldPosY;

	// Anti aliasing
	int const m_msaaSamples;

	// Frame rate measurement
	float m_timeSinceLastFpsMeassure;
	uint64_t m_framesSinceLastFpsMeassure;

	// Input handler for this window
	Input m_input;

	// Renderer
	Renderer m_renderer;

	// Scene graph
	SceneGraph m_sceneGraph;

	// Camera and projection settings
	Camera m_camera;
	float m_near;
	float m_far;
	float m_vfov;

	// Game objects
	std::unique_ptr<LightSphere> m_lightSphere;

public:
	// Callback functions for GLFW
	static void callbackGlfwError(int errorCode, const char* errorDescription);
	static void callbackGlfwKey(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void callbackGlfwCursorPosition(GLFWwindow* window, double xpos, double ypos);
	static void callbackGlfwMouseButton(GLFWwindow* window, int button, int action, int mods);
	static void callbackGlfwWindowFocus(GLFWwindow * window, int focus);
	static void callbackGlfwWindowSize(GLFWwindow* window, int width, int height);
	static void callbackGlfwFramebufferSize(GLFWwindow* window, int width, int height);
	static void callbackGlfwWindowIconify(GLFWwindow* window, int iconified);

	// Callback functions for OpenGL
	static void callbackOpenGlError(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam
	);
};
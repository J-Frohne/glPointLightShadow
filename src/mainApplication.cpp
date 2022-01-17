#include "mainApplication.hpp"

#include "log.hpp"
#include "scene/vertex.hpp"
#include "scene/mesh.hpp"
#include "scene/primitive.hpp"

#include <GL/glew.h>

#define  GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>


MainApplication::MainApplication()
	: m_window(nullptr)
	, m_windowTitle("Point Light Shadow Demo")
	, m_windowWidth(1280)
	, m_windowHeight(720)
	, m_windowOldWidth(m_windowWidth)
	, m_windowOldHeight(m_windowHeight)
	, m_windowOldPosX(100)
	, m_windowOldPosY(100)
	, m_windowIsFullscreen(false)
	, m_windowIsIconified(false)
	, m_windowVSync(true)
	, m_msaaSamples(4)
	, m_camera(m_input)
	, m_near(0.1f)
	, m_far(25.f)
	, m_vfov(glm::radians(30.f))
	, m_timeSinceLastFpsMeassure(0.f)
	, m_framesSinceLastFpsMeassure(0)
{
}

MainApplication::~MainApplication()
{
	glfwDestroyWindow(m_window);

	// Terminate GLFW
	glfwTerminate();
}

void MainApplication::init()
{
	// Initialize GLFW
	if (!glfwInit()) // Initializing GLFW multiple times is unproblematic
	{
		SPDLOG_ERROR("Initializing GLFW failed!");
		exit(EXIT_FAILURE);
	}

	// Set the GLFW error callback
	glfwSetErrorCallback(callbackGlfwError);

	// Minimum OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// OpenGL Profile and Forward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	// Multi sample anti aliasing
	glfwWindowHint(GLFW_SAMPLES, m_msaaSamples);

	// Hide the window until the first frame can be rendered
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	// Create GLFW window
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_windowTitle.c_str(), nullptr, nullptr);

	// Check whether the window creation succeeded
	if (!m_window)
	{
		SPDLOG_ERROR("Window creation failed!");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Set the user pointer to this instance of MainApplication
	glfwSetWindowUserPointer(m_window, this);

	// Set GLFW callback functions
	glfwSetKeyCallback(m_window, callbackGlfwKey);
	glfwSetWindowSizeCallback(m_window, callbackGlfwWindowSize);
	glfwSetFramebufferSizeCallback(m_window, callbackGlfwFramebufferSize);
	glfwSetWindowFocusCallback(m_window, callbackGlfwWindowFocus);
	glfwSetCursorPosCallback(m_window, callbackGlfwCursorPosition);
	glfwSetMouseButtonCallback(m_window, callbackGlfwMouseButton);
	glfwSetWindowIconifyCallback(m_window, callbackGlfwWindowIconify);

	// Make the context of the window the current OpenGL context
	glfwMakeContextCurrent(m_window);

	// Disable VSYNC
	if (!m_windowVSync)
	{
		glfwSwapInterval(0);
	}

	// Initialize GLEW
	glewExperimental = GL_TRUE; // forces GLEW to use a modern OpenGL method for checking if a function is available
	GLenum err = glewInit(); // Initializing GLEW multiple times is unproblematic
	if (GLEW_OK != err)
	{
		SPDLOG_ERROR("Initializing GLEW failed: \"{}.\"", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	// Set OpenGL error callback
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(callbackOpenGlError, 0);

	// Print the GLFW, GLEW and OpenGL version
	SPDLOG_INFO("Using GLFW {}", glfwGetVersionString());
	SPDLOG_INFO("Using GLEW {}", glewGetString(GLEW_VERSION));
	SPDLOG_INFO("Using OpenGL {}", glGetString(GL_VERSION));

	// Initialize the renderer
	m_renderer.init(&m_input);

	// Print controls
	printControls();

	// Create the scene
	createScene();
}

void MainApplication::createScene()
{
	// Set initial camera location
	m_camera.lookAt(glm::vec3(-10, 0.7, 0), glm::vec3(10, 2.0, 0));

	// Light source
	m_lightSphere = std::make_unique<LightSphere>(&m_sceneGraph, 0, glm::vec3(0.f, 1.5f, -0.28f), &m_input, &m_camera);

	// CrytekSponza
	if (true)
	{
		// Read the obj
		std::vector<std::unique_ptr<Mesh>> meshes;
		std::vector<std::unique_ptr<Material>> materials;
		Mesh::readObj("assets/scenes/CrytekSponza", "sponzaNoCurtain.obj", meshes, materials);

		// Add meshed and textures to the scene graph
		m_sceneGraph.takeMaterials(materials);
		auto sponza = m_sceneGraph.takeMeshes(meshes);
		m_sceneGraph.addNodeMeshes(0, sponza);
	}

	// Simple scene using primitives
	if (false)
	{
		Material* defaultMaterial = m_sceneGraph.takeMaterial(std::move(std::make_unique<Material>()));

		std::vector<Vertex> verts;
		std::vector<uint32_t> inds;

		float roomWidth = 20.f;
		float roomDepth = 10.f;
		float roomHeight = 5.f;

		// Floor
		if (true)
		{
			verts.clear();
			inds.clear();
			createPlane(roomWidth, roomDepth, verts, inds);
			Mesh* planeMesh = m_sceneGraph.takeMesh(std::make_unique<Mesh>(verts, inds, *defaultMaterial));
			m_sceneGraph.addNodeMesh(0, planeMesh);
		}

		// Walls
		if (true)
		{
			verts.clear();
			inds.clear();
			createCube(1.f, verts, inds);
			Mesh* wallMesh = m_sceneGraph.takeMesh(std::make_unique<Mesh>(verts, inds, *defaultMaterial));

			float wallTickness = 1.f;

			glm::vec3 wallPosL(-(roomWidth + wallTickness) / 2.f , roomHeight / 2.f, 0.f);
			glm::vec3 wallPosR((roomWidth + wallTickness) / 2.f , roomHeight / 2.f, 0.f);
			glm::vec3 wallPosF(0.f, roomHeight / 2.f, -(roomDepth + wallTickness) / 2.f);
			glm::vec3 wallPosB(0.f, roomHeight / 2.f, (roomDepth + wallTickness) / 2.f);

			glm::vec3 wallSacleFB(roomWidth, roomHeight, wallTickness);
			glm::vec3 wallSacleLR(wallTickness, roomHeight, roomDepth);

			
			size_t wallNodeL = m_sceneGraph.addNode(0, true, glm::translate(wallPosL) * glm::scale(wallSacleLR));
			size_t wallNodeR = m_sceneGraph.addNode(0, true, glm::translate(wallPosR) * glm::scale(wallSacleLR));
			size_t wallNodeF = m_sceneGraph.addNode(0, true, glm::translate(wallPosF) * glm::scale(wallSacleFB));
			size_t wallNodeB = m_sceneGraph.addNode(0, true, glm::translate(wallPosB) * glm::scale(wallSacleFB));
			m_sceneGraph.addNodeMesh(wallNodeL, wallMesh);
			m_sceneGraph.addNodeMesh(wallNodeR, wallMesh);
			m_sceneGraph.addNodeMesh(wallNodeF, wallMesh);
			m_sceneGraph.addNodeMesh(wallNodeB, wallMesh);
		}


		// Sphere
		if (true)
		{
			Material* sphereMaterial = m_sceneGraph.takeMaterial(std::move(std::make_unique<Material>(
				glm::vec4(0.5f * glm::vec3(0.2f, 0.2f, 1.0f), 1.f),
				glm::vec4(0.3f * glm::vec3(0.2f, 0.2f, 1.0f), 1.f),
				glm::vec4(0.7f * glm::vec3(0.2f, 0.2f, 1.0f), 1.f),
				100.f,
				1.f
			)));

			verts.clear();
			inds.clear();
			createSphere(0.5f, 40, 40, verts, inds);
			Mesh* sphereMesh = m_sceneGraph.takeMesh(std::make_unique<Mesh>(verts, inds, *sphereMaterial));
			size_t sphereNode = m_sceneGraph.addNode(0, true, glm::translate(glm::vec3(1.f, 0.5f, 1.f)));
			m_sceneGraph.addNodeMesh(sphereNode, sphereMesh);
		}

		// Torus
		if (true)
		{
			Material* torusMaterial = m_sceneGraph.takeMaterial(std::move(std::make_unique<Material>(
				glm::vec4(0.5f * glm::vec3(0.2f, 1.0f, 0.2f), 1.f),
				glm::vec4(0.7f * glm::vec3(0.2f, 1.0f, 0.2f), 1.f),
				glm::vec4(0.3f * glm::vec3(0.2f, 1.0f, 0.2f), 1.f),
				10.f,
				1.f
			)));

			verts.clear();
			inds.clear();
			createTorus(roomDepth / 20.f, roomDepth / 100.f, 0.f, 40, 40, verts, inds);
			Mesh* torusMesh = m_sceneGraph.takeMesh(std::make_unique<Mesh>(verts, inds, *torusMaterial));
			size_t torusNode = m_sceneGraph.addNode(0, true, glm::translate(glm::vec3(0.f, 1.5f, -0.28f)) * glm::rotate(glm::radians(90.f), glm::vec3(0,0,1)));
			m_sceneGraph.addNodeMesh(torusNode, torusMesh);
		}
		
		// Cubes
		if (true)
		{
			Material* cubeMaterial = m_sceneGraph.takeMaterial(std::move(std::make_unique<Material>(
				glm::vec4(0.5f * glm::vec3(1.0f, 0.2f, 0.2f), 1.f),
				glm::vec4(0.8f * glm::vec3(1.0f, 0.2f, 0.2f), 1.f),
				glm::vec4(0.2f * glm::vec3(1.0f, 0.2f, 0.2f), 1.f),
				1.f,
				1.f
			)));

			verts.clear();
			inds.clear();
			createCube(1.f, verts, inds);
			Mesh* cubeMesh = m_sceneGraph.takeMesh(std::make_unique<Mesh>(verts, inds, *cubeMaterial));

			int numCubes = 6;
			glm::vec3 cubeSize(roomWidth / 50.f, 5.f, roomWidth / 50.f);
			float xDistance = roomWidth / static_cast<float>(numCubes + 1);
			float yDistance = roomDepth / 4.f;

			for (int i = 1; i <= numCubes; ++i)
			{
				float xPos = -roomWidth / 2.f + static_cast<float>(i) * xDistance;
				float zPos1 = -roomDepth / 2.f + yDistance;
				float zPos2 = -zPos1;
				glm::mat4 scale = glm::scale(cubeSize);

				size_t cubeNode1 = m_sceneGraph.addNode(0, true, glm::translate(glm::vec3(xPos, 0.5f * cubeSize.y, zPos1)) * scale);
				size_t cubeNode2 = m_sceneGraph.addNode(0, true, glm::translate(glm::vec3(xPos, 0.5f * cubeSize.y, zPos2)) * scale);
				m_sceneGraph.addNodeMesh(cubeNode1, cubeMesh);
				m_sceneGraph.addNodeMesh(cubeNode2, cubeMesh);
			}
		}
		
		// Coordinate system
		if (false)
		{
			Material* xMaterial = m_sceneGraph.takeMaterial(std::make_unique<Material>(glm::vec4(1000, 0, 0, 1), glm::vec4(0), glm::vec4(0), 1.f, 1.f));
			Material* yMaterial = m_sceneGraph.takeMaterial(std::make_unique<Material>(glm::vec4(0, 1000, 0, 1), glm::vec4(0), glm::vec4(0), 1.f, 1.f));
			Material* zMaterial = m_sceneGraph.takeMaterial(std::make_unique<Material>(glm::vec4(0, 0, 1000, 1), glm::vec4(0), glm::vec4(0), 1.f, 1.f));
		
			verts.clear();
			inds.clear();
			createSphere(0.05f, 10, 10, verts, inds);
			Mesh* xMesh = m_sceneGraph.takeMesh(std::move(std::make_unique<Mesh>(verts, inds, *xMaterial)));
			Mesh* yMesh = m_sceneGraph.takeMesh(std::move(std::make_unique<Mesh>(verts, inds, *yMaterial)));
			Mesh* zMesh = m_sceneGraph.takeMesh(std::move(std::make_unique<Mesh>(verts, inds, *zMaterial)));
		
			size_t xParent = 0, yParent = 0, zParent = 0;
			for (int i = 0; i < 20; ++i)
			{
				xParent = m_sceneGraph.addNode(xParent, false, glm::translate(glm::vec3(0.1f, 0.0f, 0.0f)));
				yParent = m_sceneGraph.addNode(yParent, false, glm::translate(glm::vec3(0.0f, 0.1f, 0.0f)));
				zParent = m_sceneGraph.addNode(zParent, false, glm::translate(glm::vec3(0.0f, 0.0f, 0.1f)));
		
				m_sceneGraph.addNodeMesh(xParent, xMesh);
				m_sceneGraph.addNodeMesh(yParent, yMesh);
				m_sceneGraph.addNodeMesh(zParent, zMesh);
			}
		}
	}
}

void MainApplication::run()
{
	glfwShowWindow(m_window);
	glfwSetTime(0.0);

	// Main loop
	while (!glfwWindowShouldClose(m_window))
	{
		// Get the time delta and restart the timer for the next update
		float deltaTimeSeconds = static_cast<float>(glfwGetTime());
		glfwSetTime(0.0);

		meassueFPS(deltaTimeSeconds);
		update(deltaTimeSeconds);
		render();
	}
}

void MainApplication::update(float deltaTimeSeconds)
{
	// Store key-state of the last frame
	m_input.update();

	// Poll events
	glfwPollEvents();

	// Update actors
	m_lightSphere->update(deltaTimeSeconds);
	m_camera.update(deltaTimeSeconds);

	// Update other members
	m_sceneGraph.update();
	m_renderer.update();

	// Handle input
	if (m_input.isPushed(GLFW_KEY_F11) || m_input.isPushed(GLFW_KEY_F))
	{
		setFullscreen(!m_windowIsFullscreen);
	}
	if (m_input.isPushed(GLFW_KEY_ESCAPE))
	{
		setFullscreen(false);
	}
	if (m_input.isPushed(GLFW_KEY_T))
	{
		m_windowVSync = !m_windowVSync;
		if (m_windowVSync)
		{
			SPDLOG_INFO("VSync enabled");
			glfwSwapInterval(1);
		}
		else
		{
			SPDLOG_INFO("VSync disabled");
			glfwSwapInterval(0);
		}

	}
	if (m_input.isPushed(GLFW_KEY_H))
	{
		printControls();
	}
}

void MainApplication::render() const
{
	if (m_windowIsIconified)
	{
		return;
	}
	
	m_renderer.render(
		m_sceneGraph, 
		m_lightSphere->m_lightSource, 
		m_camera.getViewMatrix(), 
		m_vfov, 
		m_windowWidth, m_windowHeight, 
		m_near, m_far
	);
	glfwSwapBuffers(m_window);
}

void MainApplication::meassueFPS(float deltaTimeSeconds)
{
	m_timeSinceLastFpsMeassure += deltaTimeSeconds;
	m_framesSinceLastFpsMeassure++;

	if (m_timeSinceLastFpsMeassure > 1.f)
	{
		float fps = static_cast<float>(m_framesSinceLastFpsMeassure) / m_timeSinceLastFpsMeassure;
		std::string title = m_windowTitle + " (FPS: " + std::to_string(static_cast<int>(fps + 0.5)) + ")";
		glfwSetWindowTitle(m_window, title.c_str());

		m_timeSinceLastFpsMeassure = 0.f;
		m_framesSinceLastFpsMeassure = 0;
	}
}

void MainApplication::setFullscreen(bool fullscreen)
{
	if (m_windowIsFullscreen == fullscreen)
	{
		return;
	}
	
	m_windowIsFullscreen = fullscreen;
	if (m_windowIsFullscreen)
	{
		// Get the primary monitor and its video mode
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

		// Store the old size and position of the window
		m_windowOldWidth = m_windowWidth;
		m_windowOldHeight = m_windowHeight;
		glfwGetWindowPos(m_window, &m_windowOldPosX, &m_windowOldPosY);

		// Set to borderless fullscreen
		glfwSetWindowMonitor(m_window, monitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
	}
	else
	{
		// Set to windowed with the previous size and position
		glfwSetWindowMonitor(m_window, nullptr, m_windowOldPosX, m_windowOldPosY, m_windowOldWidth, m_windowOldHeight, GLFW_DONT_CARE);
	}
}

void MainApplication::printControls() const
{
	// General controls
	SPDLOG_INFO("Controls:");
	SPDLOG_INFO("  W, A, S, D - move camera horizontally");
	SPDLOG_INFO("  Space      - move camera up");
	SPDLOG_INFO("  Shift, X   - move camera down");
	SPDLOG_INFO("  Mouse drag - rotate camera");
	SPDLOG_INFO("  ','        - invert camera horizontally");
	SPDLOG_INFO("  '.'        - invert camera vertically");
	SPDLOG_INFO("  P          - toggle light animation");
	SPDLOG_INFO("  Arrow Keys - move light source horizontally");
	SPDLOG_INFO("  I, J, K, L - move light source horizontally");
	SPDLOG_INFO("  U, O       - move light source vertically");
	SPDLOG_INFO("  F, F11     - toggle fullscreen");
	SPDLOG_INFO("  ESC        - exit fullscreen");
	SPDLOG_INFO("  T          - toggle VSync");
	SPDLOG_INFO("  H          - print controls");

	// Shadow map debug controls
	SPDLOG_DEBUG("Shadow map debug controls:");
	SPDLOG_DEBUG(" 0          - reset shadow map settings");
	SPDLOG_DEBUG(" V          - toggle shadows");
	SPDLOG_DEBUG(" B          - toggle between front- and back-face culling");
	SPDLOG_DEBUG(" N          - toggle glPolygonOffset (shadow map bias)");
	SPDLOG_DEBUG(" 1, 2       - adjust polygon offset: units (constant bias)");
	SPDLOG_DEBUG(" 3, 4       - adjust polygon offset: factor (angle dependent bias)");
	SPDLOG_DEBUG(" 5, 6       - adjust shadow map resolution");
}

void MainApplication::callbackGlfwError(int errorCode, const char* errorDescription)
{
	SPDLOG_ERROR("GLFW encountered an error: {} (Error Code {})", errorDescription, errorCode);
}

void MainApplication::callbackGlfwKey(GLFWwindow* window, int key, int, int action, int)
{
	MainApplication* app = static_cast<MainApplication*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS)
	{
		app->m_input.setKey(key, true);
	}
	else if (action == GLFW_RELEASE)
	{
		app->m_input.setKey(key, false);
	}
}

void MainApplication::callbackGlfwCursorPosition(GLFWwindow* window, double xpos, double ypos)
{
	MainApplication* app = static_cast<MainApplication*>(glfwGetWindowUserPointer(window));
	app->m_input.setCursorPos(glm::dvec2(xpos, ypos));
}

void MainApplication::callbackGlfwMouseButton(GLFWwindow* window, int button, int action, int)
{
	MainApplication* app = static_cast<MainApplication*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS)
	{
		app->m_input.setKey(button, true);
	}
	else if (action == GLFW_RELEASE)
	{
		app->m_input.setKey(button, false);
	}
}

void MainApplication::callbackGlfwWindowFocus(GLFWwindow*, int)
{

}

void MainApplication::callbackGlfwWindowSize(GLFWwindow* window, int width, int height)
{
	SPDLOG_TRACE("Window resized to {}x{}", width, height);

	MainApplication* app = static_cast<MainApplication*>(glfwGetWindowUserPointer(window));
	app->m_windowWidth = width;
	app->m_windowHeight = height;
}

void MainApplication::callbackGlfwFramebufferSize(GLFWwindow*, int, int)
{
}

void MainApplication::callbackGlfwWindowIconify(GLFWwindow* window, int iconified)
{

	MainApplication* app = static_cast<MainApplication*>(glfwGetWindowUserPointer(window));

	if (iconified)
	{
		SPDLOG_TRACE("Window iconified");
		app->m_windowIsIconified = true;
	}
	else
	{
		SPDLOG_TRACE("Window restored");
		app->m_windowIsIconified = false;
	}
}

void MainApplication::callbackOpenGlError(
	GLenum,
	GLenum,
	GLuint,
	GLenum severity,
	GLsizei,
	const GLchar* message,
	const void*
)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		SPDLOG_ERROR("OpenGL Message: {}", message);
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		SPDLOG_WARN("OpenGL Message: {}", message);
		break;

	case GL_DEBUG_SEVERITY_LOW:
		SPDLOG_DEBUG("OpenGL Message: {}", message);
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
	default:
		// Messages of this severity are not really helpful. Also the Nvidia 
		// driver is extremely verbose. Uncomment if needed.
		//SPDLOG_TRACE("OpenGL Message: {}", message);
		break;
	}
}
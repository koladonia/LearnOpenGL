#include "Application.h"

#ifdef NDEBUG
	#define DEBUG 0
#else
	#define DEBUG 1
#endif

void Application::glfwErrorCallback(int error, const char* description)
{
	std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}

void Application::glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (width > 0 && height > 0)
	{
		app->camera->updateP(width, height);
		glViewport(0, 0, width, height);
		app->renderer->rebuildScreenFramebuffers(width, height);
		app->draw();
	}
}

void Application::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (action != GLFW_REPEAT)
		if (app->input->keyData.find(key) != app->input->keyData.end())
			app->input->keyEvents.push({ key, action, std::chrono::high_resolution_clock::now() });
}

void Application::glfwMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	float fxpos = static_cast<float>(xpos);
	float fypos = static_cast<float>(ypos);

	if (app->focus)
	{
		if (app->firstCallbackInFocus)
		{
			app->previousMouseX = fxpos;
			app->previousMouseY = fypos;
			app->firstCallbackInFocus = false;
		}
		else
		{
			float xOffset = (fxpos - app->previousMouseX) * app->sensitivity;
			float yOffset = (app->previousMouseY - fypos) * app->sensitivity;
			app->previousMouseX = fxpos;
			app->previousMouseY = fypos;
			if (glm::degrees(app->camera->getPitch()) + yOffset > 89.9f)
				yOffset = 89.9f - glm::degrees(app->camera->getPitch());
			else if (glm::degrees(app->camera->getPitch()) + yOffset < -89.9f)
				yOffset = -89.9f - glm::degrees(app->camera->getPitch());

			glm::quat pitchRotation = glm::angleAxis(glm::radians(yOffset), app->camera->initialRight);
			glm::quat yawRotation = glm::angleAxis(glm::radians(-xOffset), app->camera->initialUp);
			app->camera->orientation = yawRotation * app->camera->orientation * pitchRotation;
			app->camera->needUpdateV = true;
		} 
	}
}

void Application::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
		if (action == GLFW_PRESS)
		{
			app->focus = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			app->firstCallbackInFocus = true;
		}
		else if (action == GLFW_RELEASE)
		{
			app->focus = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
}

void Application::glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	auto const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		default: return "UNKNOWN";
		}
	}();

	auto const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		default: return "UNKNOWN";
		}
	}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		default: return "UNKNOWN";
		}
	}();

	std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << std::endl;
	if (strcmp(severity_str, "NOTIFICATION") != 0)
		std::cin.get();
}

Application::Application()
{
	if (!glfwInit())
		std::cerr << "Failed to initialize GLFW" << std::endl;
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	window = glfwCreateWindow(1280, 720, "Application", /*glfwGetPrimaryMonitor()*/nullptr, nullptr);
	if (!window)
		std::cerr << "Failed to create window" << std::endl;
	glfwSetWindowSizeLimits(window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwMakeContextCurrent(window);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cerr << "Failed to load GL functions" << std::endl;
	glfwSetWindowUserPointer(window, this);
	glfwSetErrorCallback(glfwErrorCallback);
	glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
	glfwSetKeyCallback(window, glfwKeyCallback);
	glfwSetCursorPosCallback(window, glfwMouseCallback);
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
#if DEBUG == 1
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glMessageCallback, nullptr);
#endif
	glfwSwapInterval(0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_STENCIL_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	renderer = new Renderer(width, height);
	input = new Input();
	ui = new UI(window);
	camera = new Camera(window);
}

Application::~Application()
{
	delete input;
	delete ui;
	delete camera;
	delete renderer;
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Application::executeLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		input->resolveKeyEvents();

		handleInput();
		input->frameFinished();

		ImGuiPerformanceBox();
		
		draw();
	}
}

void Application::draw()
{	
	camera->updateV();
	renderer->render(*camera, false);
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
}

void Application::handleInput()
{
	if (input->getKeySinglePressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
	
	//Has negative signs in case direction is opposite
	double forwardDuration = input->getKeyDuration(GLFW_KEY_W) - input->getKeyDuration(GLFW_KEY_S);
	double rightDuration = input->getKeyDuration(GLFW_KEY_D) - input->getKeyDuration(GLFW_KEY_A);
	double absForwardDuration = glm::abs(forwardDuration);
	double absRightDuration = glm::abs(rightDuration);

	if (absForwardDuration > 0.05 || absRightDuration > 0.05)
	{
		//Diagonal and axis movement should be calculated separately using normalized direction vectors
		glm::vec3 front = glm::rotate(camera->orientation, camera->initialFront);
		glm::vec3 up = glm::rotate(camera->orientation, camera->initialUp);

		glm::vec3 forwardDirection = static_cast<float>(glm::sign(forwardDuration)) * glm::normalize(front);
		glm::vec3 rightDirection = static_cast<float>(glm::sign(rightDuration)) * glm::normalize(glm::cross(front, up));

		glm::vec3 diagonalMovement = glm::vec3(0.0f);
		double diagonalDuration = 0.0;
		if ((absForwardDuration > 0.0) && (absRightDuration > 0.0))
		{
			diagonalDuration = glm::min(absForwardDuration, absRightDuration);
			glm::vec3 diagonalDirection = forwardDirection + rightDirection;
			if (glm::length(diagonalDirection) > 0.0f)
				diagonalDirection = glm::normalize(diagonalDirection);
			diagonalMovement = static_cast<float>(diagonalDuration)* diagonalDirection;
		}

		double axisDuration = glm::max(absForwardDuration, absRightDuration) - diagonalDuration;
		glm::vec3 axisDirection = absForwardDuration > absRightDuration ? forwardDirection : rightDirection;
		glm::vec3 axisMovement = static_cast<float>(axisDuration) * axisDirection;

		camera->pos += (diagonalMovement + axisMovement) * camera->speed;
		camera->needUpdateV = true;
	}
}

void Application::ImGuiPerformanceBox()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> frameDuration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(currentTime - previousTime);
	double timePassed = frameDuration.count();
	timeSinceStart += timePassed;
	numberOfFrames++;
	if (timeSinceStart > 500.0 || avgNumberOfFrames == 0)
	{
		avgTimeSinceStart = timeSinceStart;
		avgNumberOfFrames = numberOfFrames;
		timeSinceStart = 0.0;
		numberOfFrames = 0;
	}
	
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::Begin("FPS overlay", (bool*)0, overlayBox);
		ImGui::Text("%f AVG ms/Frame", avgTimeSinceStart / avgNumberOfFrames);
		ImGui::Text("%d AVG FPS", static_cast<unsigned>(1000.0 / (avgTimeSinceStart / avgNumberOfFrames)));
	ImGui::End();
	previousTime = currentTime;
}

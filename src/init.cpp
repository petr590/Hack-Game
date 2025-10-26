#include "init.h"
#include "main.h"
#include "model/models.h"
#include "shader/shader_manager.h"
#include "entity/simple_entity.h"
#include "entity/player.h"
#include "entity/enemy.h"
#include "entity/block.h"
#include "entity/minion.h"

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "nowarn_imgui.h"
#include "nowarn_imgui_backends.h"

namespace hack_game {
	using std::cerr;
	using std::endl;


	static const GLsizei SAMPLES = 4;
	static WindowData* windowData = nullptr;

	const WindowData& WindowData::getInstance() {
		static WindowData instance;
		windowData = &instance;
		return instance;
	}


	static const GLFWvidmode* initGLFW();
	static GLFWwindow* initWindow(int width, int height);
	static void initGLEW();
	static void initImGui(GLFWwindow*);

	static void shutdownImGui();
	static void shutdownGLFW(GLFWwindow*);
	
	static FramebufferInfo initGL(GLFWwindow* window, int windowWidth, int windowHeight);
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);


	WindowData::WindowData() {
		const GLFWvidmode* mode = initGLFW();
		windowWidth = mode->width;
		windowHeight = mode->height;
		refreshRate = mode->refreshRate;

		window = initWindow(windowWidth, windowHeight);
		initGLEW();
		initImGui(window);

		fbInfo = initGL(window, windowWidth, windowHeight);
	}

	WindowData::~WindowData() {
		shutdownImGui();
		shutdownGLFW(window);
	}


	// ------------------------------------------- init -------------------------------------------

	static const GLFWvidmode* initGLFW() {
		if (!glfwInit()) {
			cerr << "Failed to create initialize GLFW" << endl;
			exit(EXIT_FAILURE);
		}
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		return glfwGetVideoMode(glfwGetPrimaryMonitor());
	}


	static GLFWwindow* initWindow(int width, int height) {
		
		GLFWwindow* window = glfwCreateWindow(width, height, "Hacking Game", nullptr, nullptr);
		if (window == nullptr) {
			cerr << "Failed to create GLFW window" << endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

		return window;
	}

	static void initGLEW() {
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			cerr << "Failed to initialize GLEW" << endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
	}
	
	static void initImGui(GLFWwindow* window) {

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // TODO?

		ImGui::StyleColorsDark();

		const float mainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes(mainScale);
		style.FontScaleDpi = mainScale;
		style.FontSizeBase = 35.0f;
		ImFont* font = io.Fonts->AddFontFromFileTTF("resources/fonts/TikTok_Sans_Regular.ttf");
    	IM_ASSERT(font != nullptr);

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");
	}
	

	static void shutdownImGui() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	static void shutdownGLFW(GLFWwindow* window) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}


	static void generateMultisampleBuffer(FramebufferInfo& fbInfo, int windowWidth, int windowHeight) {
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, SAMPLES, GL_RGBA, windowWidth, windowHeight, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture, 0);

		GLuint renderbuffer;
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, SAMPLES, GL_DEPTH_COMPONENT24, windowWidth, windowHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		fbInfo.msFramebuffer = framebuffer;
		fbInfo.msTexture = texture;
		fbInfo.msRenderbuffer = renderbuffer;
	}


	static void generateFramebuffer(FramebufferInfo& fbInfo, int windowWidth, int windowHeight) {
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		fbInfo.framebuffer = framebuffer;
		fbInfo.texture = texture;
	}


	static FramebufferInfo initGL(GLFWwindow* window, int windowWidth, int windowHeight) {
		for (Model* model : Model::getModels()) {
			model->generateVertexArray();
		}

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		GLint width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		
		FramebufferInfo fbInfo;
		generateMultisampleBuffer(fbInfo, windowWidth, windowHeight);
		generateFramebuffer(fbInfo, windowWidth, windowHeight);
		return fbInfo;
	}


	static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
		windowData->setWindowWidth(width);
		windowData->setWindowHeight(height);

		const FramebufferInfo& fbInfo = windowData->getFbInfo();

		GLint fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		glViewport(0, 0, fbWidth, fbHeight);
		
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fbInfo.msTexture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, SAMPLES, GL_RGBA, width, height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, fbInfo.msRenderbuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, SAMPLES, GL_DEPTH_COMPONENT24, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, fbInfo.texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);

		onChangeWindowSize(width, height);
	}
}

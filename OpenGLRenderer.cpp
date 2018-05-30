#include "stdafx.h"
#include "OpenGLRenderer.h"
#include "GLFW/glfw3.h"
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Controls.h"
#include "Chunk.h"
#include "SectionBuilder.h"
#include "AsyncVboBuildingManager.h"
#include "Loader.h"
#include "World.h"
#include <ctime>
#include "Fbo.h"
#include "PostProcessing.h"
#include "Skybox.h"
#include "Settings.h"
#include "Raycast.h"
#include "BBRenderer.h"
#include "Font.h"
#include "C01PacketChat.h"
#include "MinecraftSocket.h"
#include "IPacket.h"
#include "C07PlayerDigging.h"
#include "CrosshairRenderer.h"
#include "VboBuilder.h"
#include "Title.h"

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "glfw3.lib")

using namespace std;
using namespace glm;

AsyncVboBuildingManager* OpenGLRenderer::manager;
World* OpenGLRenderer::world;
Frustum* OpenGLRenderer::frustum;
Controls* OpenGLRenderer::controls;
vector<CHATMESSAGE>* OpenGLRenderer::chatMessages;

int OpenGLRenderer::width;
int OpenGLRenderer::height;
bool OpenGLRenderer::chatOpen;
glm::mat4 projection;

Fbo fbo;

bool lastPressed;
string chatInput = string();
OpenGLRenderer::OpenGLRenderer()
{
	world = new World();
}

OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::sendPacket(IPacket * packet)
{
	if (MinecraftSocket::connected)
		MinecraftSocket::instance->sendPacket(packet);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE && key == GLFW_KEY_T && !OpenGLRenderer::chatOpen) {
		OpenGLRenderer::chatOpen = true;
		chatInput = "";
	}
	if (OpenGLRenderer::chatOpen && key != GLFW_KEY_LEFT_SHIFT) {
		if (action == GLFW_RELEASE) return;
		if (key == GLFW_KEY_BACKSPACE && chatInput.length() > 0) {
			chatInput = chatInput.substr(0, chatInput.length() - 1);
		}
		else {
			char x = (char)key;
			if (!iscntrl(x)) {
				if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS) x = tolower(x);
				chatInput += x;
			}
		}
	}
	if (OpenGLRenderer::chatOpen && key == GLFW_KEY_ESCAPE) {
		Controls::first = true;
		OpenGLRenderer::chatOpen = false;
	}
	if (OpenGLRenderer::chatOpen && key == GLFW_KEY_ENTER) {
		Controls::first = true;
		MinecraftSocket::instance->sendPacket(new C01PacketChat(chatInput.c_str()));
		OpenGLRenderer::chatOpen = false;
	}


}
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void OpenGLRenderer::start() {
	width = 854;
	height = 480;
	GLFWwindow* window;
	controls = new Controls();
	frustum = new Frustum();
	chatMessages = new vector<CHATMESSAGE>;

	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, GAME_TITLE, NULL, NULL);
	if (!window)
	{
		cout << "Window could not be created" << endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize OpenGL context" << endl;
		return;
	}

	glClearColor(0.72f, 0.83f, 0.996f, 0.0f);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);


	Loader loader;

	cout << "Loading fonts..." << endl;
	Font roboto = Font("fonts\\roboto.ttf", 18);

	// Creating vertex array
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);

	// Loading textures
	GLuint texture = loader.loadTexture("textures\\atlas_blocks.png");
	GLuint crosshair = loader.loadTexture("textures\\hud\\crosshair.png");

	manager = new AsyncVboBuildingManager();
	manager->initialize();

	GLuint simpleShader = loader.loadShaders("simple");
	GLint mvMatrixLocation = glGetUniformLocation(simpleShader, "mvMatrix");
	GLint prMatrixLocation = glGetUniformLocation(simpleShader, "prMatrix");
	GLint skyColorLocation = glGetUniformLocation(simpleShader, "skyColor");

	GLuint fontShader = loader.loadShaders("font");
	GLint fontMatrixLocation = glGetUniformLocation(fontShader, "projection");
	GLint colorLocation = glGetUniformLocation(fontShader, "textColor");

	GLuint postProcShader = loader.loadShaders("postproc");
	GLint fluidModeLocation = glGetUniformLocation(postProcShader, "fluidMode");

	GLuint crosshairShader = loader.loadShaders("crosshair");
	GLint crosshairMatrixLocation = glGetUniformLocation(crosshairShader, "projection");
	GLint crosshairOffsetLocation = glGetUniformLocation(crosshairShader, "offset");

	GLuint waterShader = loader.loadShaders("water");
	GLint mvMatrixLocationW = glGetUniformLocation(waterShader, "mvMatrix");
	GLint prMatrixLocationW = glGetUniformLocation(waterShader, "prMatrix");
	GLint skyColorLocationW = glGetUniformLocation(waterShader, "skyColor");
	GLint timeLocation = glGetUniformLocation(waterShader, "time");

	GLuint skyboxShader = loader.loadShaders("skybox");
	GLint loc_projMatSkybox = glGetUniformLocation(skyboxShader, "projectionMatrix");
	GLint loc_viewMatSkybox = glGetUniformLocation(skyboxShader, "viewMatrix");

	GLuint wireframeShader = loader.loadShaders("wireframe");
	GLint loc_projMatWire = glGetUniformLocation(wireframeShader, "projectionMatrix");
	GLint loc_mdvwMatWire = glGetUniformLocation(wireframeShader, "modelviewMatrix");
	GLint loc_wire_offset = glGetUniformLocation(wireframeShader, "offset");

	GLuint guiShader = loader.loadShaders("gui");
	GLint loc_gui_projMat = glGetUniformLocation(guiShader, "projection");

	MATRICES matrices;
	projection = glm::ortho(0.0f, (float)OpenGLRenderer::width, 0.0f, (float)OpenGLRenderer::height);

	Raycast raycast;
	BBRenderer bbRenderer;

	Skybox skybox;
	skybox.initialize(loader);

	int frames = 0;
	int fps = 0;
	int lastReset = clock();

	fbo = Fbo(width, height, DEPTH_RENDER_BUFFER);
	CrosshairRenderer crosshairRenderer = CrosshairRenderer(crosshair);
	crosshairRenderer.init();

	PostProcessing postProc = PostProcessing(postProcShader);
	postProc.init();

	vec3 skyColor = vec3(0.72f, 0.83f, 0.996f);

	glUniform3f(skyColorLocation, skyColor.x, skyColor.y, skyColor.z);
	while (!glfwWindowShouldClose(window))
	{
		int current_time = clock();

		fbo.bindFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(simpleShader);

		matrices = controls->computeMatrices(window);
		glUniformMatrix4fv(mvMatrixLocation, 1, false, &matrices.modelviewMatrix[0][0]);
		glUniformMatrix4fv(prMatrixLocation, 1, false, &matrices.projectionMatrix[0][0]);


		glBindVertexArray(VertexArrayID);
		glBindTexture(GL_TEXTURE_2D, texture);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		world->render(false); // Render pass 1: Solid and transparent blocks

		if (Settings::ADVANCED_WATER) {
			glUseProgram(waterShader);
			glUniform1i(timeLocation, current_time);
			glUniformMatrix4fv(mvMatrixLocationW, 1, false, &matrices.modelviewMatrix[0][0]);
			glUniformMatrix4fv(prMatrixLocationW, 1, false, &matrices.projectionMatrix[0][0]);
			glUniform3f(skyColorLocationW, skyColor.x, skyColor.y, skyColor.z);
		}
		glDisable(GL_CULL_FACE);
		world->render(true); // Render pass 2: Translucent blocks (water, etc.)
		glEnable(GL_CULL_FACE);

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		if (Settings::SKYBOX) {
			glUseProgram(skyboxShader);
			matrices.viewMatrix[3][0] = 0;
			matrices.viewMatrix[3][1] = 0;
			matrices.viewMatrix[3][2] = 0;
			glUniformMatrix4fv(loc_projMatSkybox, 1, false, &matrices.projectionMatrix[0][0]);
			glUniformMatrix4fv(loc_viewMatSkybox, 1, false, &matrices.viewMatrix[0][0]);
			skybox.render();
		}

		glDisable(GL_CULL_FACE);
		vec2 rot = controls->getRotation();
		RAYCAST_RESULT result = raycast.cast(rot.x, rot.y);
		glUseProgram(wireframeShader);
		glUniformMatrix4fv(loc_mdvwMatWire, 1, false, &matrices.modelviewMatrix[0][0]);
		glUniformMatrix4fv(loc_projMatWire, 1, false, &matrices.projectionMatrix[0][0]);
		vec3 renderOffset = vec3(result.blockX, result.blockY, result.blockZ);
		glUniform3fv(loc_wire_offset, 1, &renderOffset[0]);
		bbRenderer.render();
		glEnable(GL_CULL_FACE);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
			if (!lastPressed) {
				sendPacket(new C07PlayerDigging(START_DESTROY_BLOCK, POSITION(result.blockX, result.blockY, result.blockZ), result.face));
				sendPacket(new C07PlayerDigging(STOP_DESTROY_BLOCK, POSITION(result.blockX, result.blockY, result.blockZ), result.face));
				world->setBlock(result.blockX, result.blockY, result.blockZ, 0);
			}
			lastPressed = true;
		}
		else lastPressed = false;

		fbo.unbindFrameBuffer();

		glDisable(GL_DEPTH_TEST);


		Section::resetData();

		/* Post Processing */
		vec3 playerPos = controls->getPosition();
		vec3 eyePos = controls->getEyePosition();

		char blockInEyes = world->getBlock(floor(eyePos.x), floor(eyePos.y), floor(eyePos.z));
		int fluidMode = 0;
		if (blockInEyes == 8 || blockInEyes == 9) fluidMode = 1;
		else if (blockInEyes == 10 || blockInEyes == 11) fluidMode = 2;
		postProc.doPostProc(fbo.getColorTexture(), fluidModeLocation, fluidMode);

		/* GUI */
		glUseProgram(guiShader);
		glUniformMatrix4fv(loc_gui_projMat, 1, false, &projection[0][0]);
		float offset = 100;
		for (int i = chatMessages->size() - 1; i >= 0; i--) {
			CHATMESSAGE msg = chatMessages->at(i);
			if (chatOpen || current_time - msg.timeCreated < 10000) {
				VboBuilder builder = VboBuilder(2);
				builder.drawRect(15, offset - 7, 350, 25, COLORDATA(64, 64, 64, 128));
				builder.buildAndRender();
				offset += 25;
				if (!chatOpen && height - offset < height / 3) break;
			}
		}
		if (chatOpen) {
			VboBuilder builder = VboBuilder(2);
			builder.drawRect(15, 15, width - 30, 25, COLORDATA(64, 64, 64, 128));
			builder.buildAndRender();
		}

		/* Crosshair */
		glUseProgram(crosshairShader);
		vec2 chOffset = vec2(width / 2, height / 2);
		glUniform2fv(crosshairOffsetLocation, 1, &chOffset[0]);
		glUniformMatrix4fv(crosshairMatrixLocation, 1, false, &projection[0][0]);
		crosshairRenderer.render();

		/* Font */
		glClear(GL_DEPTH_BUFFER_BIT);
		glUseProgram(fontShader);
		glUniformMatrix4fv(fontMatrixLocation, 1, false, &projection[0][0]);

		offset = height - 100;
		for (int i = chatMessages->size() - 1; i >= 0; i--) {
			CHATMESSAGE msg = chatMessages->at(i);
			if (chatOpen || current_time - msg.timeCreated < 10000) {
				roboto.renderTextWithShadow(*msg.content, colorLocation, 25, 1.0f + offset, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
				offset -= 25;
				if (!chatOpen && offset < height / 3) break;
			}
		}

		/* Debug Information */
		roboto.renderTextWithShadow("Debug Information", colorLocation, 25, 25, 1.0, vec3(1.0, 1.0, 1.0));
		roboto.renderTextWithShadow(" FPS: " + to_string(fps), colorLocation, 25, 40, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
		roboto.renderTextWithShadow(" XYZ: " + to_string(playerPos.x) + " " + to_string(playerPos.y) + " " + to_string(playerPos.z), colorLocation, 25, 55, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));

		if (chatOpen) {
			string append = current_time % 1000 < 500 ? "|" : "";
			roboto.renderTextWithShadow(chatInput + append, colorLocation, 20, height - 22, 1.0f, vec3(1, 1, 1));
		}
		glEnable(GL_DEPTH_TEST);

		/* FPS counter */
		frames++;
		if (current_time - lastReset > 1000) {
			lastReset = current_time;
			fps = frames;
			frames = 0;
		}

		/* Swap buffers */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	fbo.cleanUp();

	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(simpleShader);
	glfwTerminate();
	return;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	OpenGLRenderer::width = width;
	OpenGLRenderer::height = height;
	projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
	fbo = Fbo(width, height, DEPTH_RENDER_BUFFER);
}
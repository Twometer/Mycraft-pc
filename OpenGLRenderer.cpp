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
#include "GuiChat.h"
#include "GuiPause.h"
#include "TextureIds.h"
#include "GuiRespawn.h"
#include "ShadowMapRenderer.h"

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "glfw3.lib")

using namespace std;
using namespace glm;

AsyncVboBuildingManager* OpenGLRenderer::manager;
GuiRenderer* OpenGLRenderer::guiRenderer;
World* OpenGLRenderer::world;
Frustum* OpenGLRenderer::frustum;
Controls* OpenGLRenderer::controls;
vector<CHATMESSAGE>* OpenGLRenderer::chatMessages;

int OpenGLRenderer::width;
int OpenGLRenderer::height;
glm::mat4 projection;

Fbo fbo;
PostProcessing postProc;

bool lastPressed;

GLFWwindow* window;

bool show_debug = false;
OpenGLRenderer::OpenGLRenderer()
{
	world = new World();
	guiRenderer = new GuiRenderer();
}

OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::sendPacket(IPacket * packet)
{
	if (MinecraftSocket::connected)
		MinecraftSocket::instance->sendPacket(packet);
}
void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	OpenGLRenderer::guiRenderer->onCharPress(codepoint);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bool guiOpen = OpenGLRenderer::guiRenderer->isGuiOpen();
	OpenGLRenderer::guiRenderer->onKeyPress(key, action);
	if (action == GLFW_RELEASE && !guiOpen)
	{
		if (key == GLFW_KEY_T)
			OpenGLRenderer::guiRenderer->displayGui(new GuiChat());
		else if (key == GLFW_KEY_ESCAPE)
			OpenGLRenderer::guiRenderer->displayGui(new GuiPause());
		else if (key == GLFW_KEY_F3)
			show_debug = !show_debug;
	}
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	OpenGLRenderer::guiRenderer->onMouseClick(button, action);
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void OpenGLRenderer::setCursorVisibility(bool visible)
{
	if (visible)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void OpenGLRenderer::onKilled()
{
	guiRenderer->displayGui(new GuiRespawn());
}
void OpenGLRenderer::start()
{
	width = 854;
	height = 480;

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
	window = glfwCreateWindow(width, height, GAME_TITLE, nullptr, nullptr);
	if (!window)
	{
		cout << "Window could not be created" << endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, character_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
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
	Font::roboto = Font("fonts\\Roboto.ttf", 18);
	Font::robotoTitle = Font("fonts\\Roboto.ttf", 36);

	// Creating vertex array
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);

	// Loading textures
	GLuint texture = loader.loadTexture("textures\\atlas_blocks.png", false);
	GLuint crosshair = loader.loadTexture("textures\\hud\\common\\crosshair.png", false);
	TextureIds::tex_guipause_back = loader.loadTexture("textures\\hud\\common\\back.png", true);
	TextureIds::tex_guipause_settings = loader.loadTexture("textures\\hud\\common\\settings.png", true);
	TextureIds::tex_guipause_leave = loader.loadTexture("textures\\hud\\common\\leave.png", true);
	TextureIds::tex_guirespawn_respawn = loader.loadTexture("textures\\hud\\common\\respawn.png", true);
	TextureIds::tex_guisettings_advanced_water = loader.loadTexture("textures\\hud\\settings\\advanced_water.png", true);
	TextureIds::tex_guisettings_ambient_occlusion = loader.loadTexture("textures\\hud\\settings\\ambient_occlusion.png", true);
	TextureIds::tex_guisettings_bloom = loader.loadTexture("textures\\hud\\settings\\bloom.png", true);
	TextureIds::tex_guisettings_downscaling = loader.loadTexture("textures\\hud\\settings\\downscaling.png", true);
	TextureIds::tex_guisettings_fov = loader.loadTexture("textures\\hud\\settings\\fov.png", true);
	TextureIds::tex_guisettings_render_distance = loader.loadTexture("textures\\hud\\settings\\render_distance.png", true);
	TextureIds::tex_guisettings_skybox = loader.loadTexture("textures\\hud\\settings\\skybox.png", true);

	manager = new AsyncVboBuildingManager();
	manager->initialize();

	GLuint simpleShader = loader.loadShaders("simple");
	GLint mvMatrixLocation = glGetUniformLocation(simpleShader, "mvMatrix");
	GLint prMatrixLocation = glGetUniformLocation(simpleShader, "prMatrix");
	GLint skyColorLocation = glGetUniformLocation(simpleShader, "skyColor");
	GLint toShadowMapSpaceLocation = glGetUniformLocation(simpleShader, "toShadowMapSpace");
	GLint texSamplerLocation = glGetUniformLocation(simpleShader, "texSampler");
	GLint shadowMapLocation = glGetUniformLocation(simpleShader, "shadowMap");

	glUseProgram(simpleShader);
	glUniform1i(texSamplerLocation, 0);
	glUniform1i(shadowMapLocation, 1);
	glUseProgram(0);

	GLuint fontShader = loader.loadShaders("font");
	GLint fontMatrixLocation = glGetUniformLocation(fontShader, "projection");
	GLint colorLocation = glGetUniformLocation(fontShader, "textColor");

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

	GLuint guiTexShader = loader.loadShaders("guitex");
	GLint loc_guit_projMat = glGetUniformLocation(guiTexShader, "projection");

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

	postProc = PostProcessing();
	postProc.init(loader);
	postProc.resize(width, height);

	ShadowMapRenderer shadowMapRenderer(loader);

	vec3 skyColor = vec3(0.72f, 0.83f, 0.996f);

	glUniform3f(skyColorLocation, skyColor.x, skyColor.y, skyColor.z);
	while (!glfwWindowShouldClose(window))
	{
		int current_time = clock();

		/* Shadow Map */
		glBindVertexArray(VertexArrayID);
		glEnableVertexAttribArray(0);
		shadowMapRenderer.render();
		glDisableVertexAttribArray(0);

		/* World renderer */
		fbo.bindFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(simpleShader);

		matrices = controls->computeMatrices(window);
		glUniformMatrix4fv(mvMatrixLocation, 1, false, &matrices.modelviewMatrix[0][0]);
		glUniformMatrix4fv(prMatrixLocation, 1, false, &matrices.projectionMatrix[0][0]);
		glUniformMatrix4fv(toShadowMapSpaceLocation, 1, false, &shadowMapRenderer.get_to_shadow_map_space_matrix()[0][0]);
		
		glBindVertexArray(VertexArrayID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowMapRenderer.get_shadow_map());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		world->render(false, false); // Render pass 1: Solid and transparent blocks

		if (Settings::ADVANCED_WATER) {
			glUseProgram(waterShader);
			glUniform1i(timeLocation, current_time);
			glUniformMatrix4fv(mvMatrixLocationW, 1, false, &matrices.modelviewMatrix[0][0]);
			glUniformMatrix4fv(prMatrixLocationW, 1, false, &matrices.projectionMatrix[0][0]);
			glUniform3f(skyColorLocationW, skyColor.x, skyColor.y, skyColor.z);
		}
		glDisable(GL_CULL_FACE);
		world->render(true, false); // Render pass 2: Translucent blocks (water, etc.)
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

		if (!guiRenderer->isGuiOpen() && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
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

		postProc.doPostProc(fbo.getColorTexture());

		/* GUI */
		glUseProgram(guiShader);
		glUniformMatrix4fv(loc_gui_projMat, 1, false, &projection[0][0]);

		bool chatOpen = guiRenderer->isGuiOpen(GuiChat::GUI_IDENTIFIER);

		float offset = 100;
		for (int i = chatMessages->size() - 1; i >= 0; i--) {
			CHATMESSAGE msg = chatMessages->at(i);
			if (chatOpen || current_time - msg.timeCreated < 10000) {
				VboBuilder* builder = new VboBuilder(2);
				builder->drawRect(15, offset - 7, 500, 25, COLORDATA(0, 0, 0, 128));
				builder->buildAndRender();
				delete builder;
				offset += 25;
				if (!chatOpen && height - offset < height / 3) break;
			}
		}

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		guiRenderer->onRender(xpos, ypos, FLAT, 0);

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

		offset = height - 99.0f;
		for (int i = chatMessages->size() - 1; i >= 0; i--) {
			CHATMESSAGE msg = chatMessages->at(i);
			if (chatOpen || current_time - msg.timeCreated < 10000) {
				Font::roboto.renderTextWithShadow(*msg.content, colorLocation, 22, 1.0f + offset, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
				offset -= 25.0f;
				if (!chatOpen && offset < height / 3) break;
			}
		}

		vec3 playerPos = controls->getPosition();


		/* Debug Information */
		if (show_debug) {
			Font::roboto.renderTextWithShadow("Debug Information", colorLocation, 25, 25, 1.0, vec3(1.0, 1.0, 1.0));
			Font::roboto.renderTextWithShadow(" FPS: " + to_string(fps), colorLocation, 25, 43, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
			Font::roboto.renderTextWithShadow(" XYZ: " + to_string(playerPos.x) + " " + to_string(playerPos.y) + " " + to_string(playerPos.z), colorLocation, 25, 61, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
		}
		guiRenderer->onRender(xpos, ypos, FONT, colorLocation);
		glEnable(GL_DEPTH_TEST);

		/* Textured GUIs */
		glUseProgram(guiTexShader);
		glUniformMatrix4fv(loc_guit_projMat, 1, false, &projection[0][0]);
		guiRenderer->onRender(xpos, ypos, TEX, 0);

		glBindTexture(GL_TEXTURE_2D, shadowMapRenderer.get_shadow_map());
		VboBuilder builder = VboBuilder(2);
		builder.drawRect(0, 0, 640, 480, COLORDATA(255, 255, 255, 255), true);
		builder.buildAndRender();

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
	postProc.resize(width, height);
}
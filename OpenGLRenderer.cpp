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

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "glfw3.lib")

using namespace std;
using namespace glm;

static const int WIDTH = 640;
static const int HEIGHT = 480;

AsyncVboBuildingManager* OpenGLRenderer::manager;
World* OpenGLRenderer::world;
Frustum* OpenGLRenderer::frustum;
Controls* OpenGLRenderer::controls;
vector<string*>* OpenGLRenderer::chatMessages;

OpenGLRenderer::OpenGLRenderer()
{
	world = new World();
}


OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::start() {
	GLFWwindow* window;
	controls = new Controls();
	frustum = new Frustum();
	chatMessages = new vector<string*>;

	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Mycraft v1.0-beta", NULL, NULL);
	if (!window)
	{
		cout << "Window could not be created" << endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

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
	loader.loadFonts();

	// Creating vertex array
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);

	// Loading textures
	GLuint texture = loader.loadPng("atlas_blocks.png");

	manager = new AsyncVboBuildingManager();
	manager->initialize();

	GLuint simpleShader = loader.loadShaders("shaders\\simple.v.glsl", "shaders\\simple.f.glsl");
	GLint mvMatrixLocation = glGetUniformLocation(simpleShader, "mvMatrix");
	GLint prMatrixLocation = glGetUniformLocation(simpleShader, "prMatrix");
	GLint skyColorLocation = glGetUniformLocation(simpleShader, "skyColor");

	GLuint fontShader = loader.loadShaders("shaders\\font.v.glsl", "shaders\\font.f.glsl");
	GLint fontMatrixLocation = glGetUniformLocation(fontShader, "projection");
	GLint colorLocation = glGetUniformLocation(fontShader, "textColor");

	GLuint postProcShader = loader.loadShaders("shaders\\postproc.v.glsl", "shaders\\postproc.f.glsl");

	MATRICES matrices;
	glm::mat4 projection = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f);
	
	int frames = 0;
	int fps = 0;
	int lastReset = clock();

	Fbo fbo = Fbo(640, 480, DEPTH_RENDER_BUFFER);
	PostProcessing postProc = PostProcessing(postProcShader);
	postProc.init();

	vec3 skyColor = vec3(0.72f, 0.83f, 0.996f);

	glUniform3f(skyColorLocation, skyColor.x, skyColor.y,skyColor.z);

	while (!glfwWindowShouldClose(window))
	{
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

		glDisable(GL_CULL_FACE);
		world->render(true); // Render pass 2: Translucent blocks (water, etc.)
		glEnable(GL_CULL_FACE);

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		fbo.unbindFrameBuffer();

		Section::resetData();

		postProc.doPostProc(fbo.getColorTexture());

		glClear(GL_DEPTH_BUFFER_BIT);
		glUseProgram(fontShader);
		glUniformMatrix4fv(fontMatrixLocation, 1, false, &projection[0][0]);
		float offset = 0;
		for (unsigned int i = 0; i < chatMessages->size(); i++) {
			loader.renderText(*chatMessages->at(i), colorLocation, 1.0f, 1.0f + offset, 1.0f, glm::vec3(0.0, 0.0f, 0.0f));
			offset += 21;
		}
		loader.renderText("FPS: " + to_string(fps), colorLocation, 1.0f, 100.0f, 1.0f, glm::vec3(0.0, 0.0f, 0.0f));
		glm::vec3 pos = controls->getPosition();
		loader.renderText("Pos: " + to_string(pos.x) + " " + to_string(pos.y) + " " + to_string(pos.z), colorLocation, 1.0f, 150.0f, 1.0f, glm::vec3(0.0, 0.0f, 0.0f));

		frames++;

		if (clock() - lastReset > 1000) {
			lastReset = clock();
			fps = frames;
			frames = 0;
		}


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	fbo.cleanUp();

	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(simpleShader);
	glfwTerminate();
	return;
}
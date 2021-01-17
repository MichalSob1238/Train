#define GLEW_STATIC
#include <GL/glew.h>
#include "shprogram.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Camera.h"
#include "Scene.h"
#include "Wagon.h"
#include "Locomotive.h"
#include "Tracks.h"
#include "floor.h"

const GLuint WIDTH = 1920, HEIGHT = 1080;

static Camera camera(glm::vec3(1.f,1.f,1.f), glm::vec3(0.f, 1.f, 0.f));
float current_time = 0.0, delta_time = 0.0f, last_frame = 0.0f, directional_speed = 0.0f, light_intensity = 0.3f;
double lastX = WIDTH/2;
double lastY = HEIGHT/2;
bool firstMouse = true;



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void processInput(GLFWwindow* window) {
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		directional_speed += 0.01 * delta_time;
		if (directional_speed > 1.0)
			directional_speed = 1;
	}
		
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		directional_speed -= 0.01f * delta_time;
		if (directional_speed < -1.0)
			directional_speed = -1;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{

		directional_speed += delta_time * (directional_speed > 0 ? -1.0 : 1.0) * 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

		if ((light_intensity += 1.f * delta_time) > 1.0f)
			light_intensity = 1.0f;
	}
		
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		
		if ((light_intensity -= 1.f * delta_time) < 0.3f)
			light_intensity = 0.3f;
	}
}

void scroll_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.ProcessMouseScroll(ypos);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

int main()
{
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	try
	{
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GKOM - OpenGL 206.z Lokomotywa", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");
		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");

		glViewport(0, 0, WIDTH, HEIGHT);
		glEnable(GL_DEPTH_TEST);

		Cuboid LightCube({ 10, 99, 10 }, { 0.1, 0.1, 0.1 }, glm::vec3( 1, 1, 1 ), true);
		Cuboid SkyBox({ 0,0,0 }, { 1, 1, 1 }, "skybox2.png");
		SkyBox.setShader(Scene::getScene().skybox_shader);
		Floor Floor({ 0,-.5,0 }, { 100,1,10000 }, "floor2.png");
		Tracks TrainTracks;
			
		Wagon Wagon1; 
		Wagon1.move({ 0,1.02,3 });
		Locomotive Loc1;
		Loc1.move({ 0,1.02,0 });

		// main event loop
		float num = 1;
		while (!glfwWindowShouldClose(window))
		{
			processInput(window);
			current_time = glfwGetTime();
			delta_time = current_time - last_frame;
			last_frame = current_time;

			// resistance on rolling
			if(directional_speed != 0.0)
				directional_speed += delta_time * (directional_speed > 0 ? -1. : 1.) * 0.001;

			glfwPollEvents();
			// Clear the colorbuffer
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			camera.adjustPosition(Loc1.getPosition());

			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
			Scene::getScene().setMatrix4fvInShaders("projection", projection);
			Scene::getScene().setMatrix4fvInShaders("view", camera.GetViewMatrix());
			Scene::getScene().setVec3InShaders("viewPos", camera.getPosition());

			//movement
			Scene::getScene().updateLights();

			SkyBox.setPosition(camera.getPosition());
			LightCube.setPosition({ -10,99,10 + Loc1.getPosition().z });
			LightCube.draw();
			glDepthMask(GL_FALSE);
			SkyBox.draw();
			glDepthMask(GL_TRUE);
			Loc1.move({ 0,0,directional_speed });
			Loc1.set_light_intensity(light_intensity);
			Loc1.draw();
			Wagon1.move({ 0,0,directional_speed });
			Wagon1.draw();
			Floor.adjustPosition(Loc1.getPosition()); // TODO
			Floor.draw();
			TrainTracks.adjustPosition(Loc1.getPosition());
			TrainTracks.draw();

			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			// Swap the screen buffers
			glfwSwapBuffers(window);
		}

	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	glfwTerminate();

	return 0;
}

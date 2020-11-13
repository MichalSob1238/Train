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

#include "Cuboid.h"
#include "Composite.h"
#include "Cylinder.h"
#include "Camera.h"
const GLuint WIDTH = 800, HEIGHT = 800;

static Camera camera(glm::vec3(0.f,0.f,3.f), glm::vec3(0.f, 1.f, 0.f));
float current_time = 0.0, delta_time = 0.0f, last_frame = 0.0f;
double lastX = WIDTH/2;
double lastY = HEIGHT/2;
bool firstMouse = true;



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::FORWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::LEFT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::RIGHT, delta_time);
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

GLuint LoadMipmapTexture(GLuint texId, const char* fname)
{
	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
		throw runtime_error("Failed to load texture file");

	GLuint texture;
	glGenTextures(1, &texture);

	glActiveTexture(texId);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
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
			throw runtime_error("GLFW window not created");
		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw runtime_error("GLEW Initialization failed");

		glViewport(0, 0, WIDTH, HEIGHT);
		glEnable(GL_DEPTH_TEST);

		Cuboid Cube1({ 0, 0, 0 }, { 0.5, 1.5, 0.5 }, glm::vec3( 1, 0, 0 ));
		Cuboid Cube2({ 1, 0, 0 }, { 0.5, 0.5, 0.5 }, glm::vec3( 0, 0, 1 ));
		Cuboid Cube3({ 0.5, 0, 0 }, { 0.5, 0.5, 0.5 }, glm::vec3( 0, 1, 0 ));

		Cylinder Cylinder1({ 0, 0, 0.5 }, { 0.5, 0.5, 0.5 }, glm::vec3(0.4f, 0.3f, 1.0f));
		Cylinder1.rotate({ 90, 0, 0 });
		Cylinder Cylinder2({ -0.5, 0, 0 }, { 0.5, 0.5, 0.5 }, glm::vec3(0.2f, 0.1f, 1.0f));
		Cylinder2.rotate({ 0,0,90 });

		Composite cubes({ 0,0,0 });

		cubes.addElement(Cube1);
		cubes.addElement(Cube2);
		cubes.addElement(Cube3);
		cubes.addElement(Cylinder1);
		cubes.addElement(Cylinder2);
		cubes.rotate({ 90,0,0 });
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// prepare textures
		//GLuint texture0 = LoadMipmapTexture(GL_TEXTURE0, "piesek.png");
		// GLuint texture1 = LoadMipmapTexture(GL_TEXTURE1, "weiti.png");

		ShaderProgram shader("CubeShader.vert", "CubeShader.frag");
		// main event loop
		while (!glfwWindowShouldClose(window))
		{

			current_time = glfwGetTime();
			delta_time = current_time - last_frame;
			last_frame = current_time;
			// Clear the colorbuffer
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader.Use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
			GLuint projectionView = glGetUniformLocation(shader.get_programID(), "projection");
			glUniformMatrix4fv(projectionView, 1, GL_FALSE, glm::value_ptr(projection));
			glm::mat4 view = camera.GetViewMatrix();
			GLuint transformView = glGetUniformLocation(shader.get_programID(), "view");
			glUniformMatrix4fv(transformView, 1, GL_FALSE, glm::value_ptr(view));

			cubes.move({ 0,0, -0.001 });
			cubes.rotate({ 0,1, 0 }/*, { 0,0,0 }*/);
			cubes.draw();
	
			// Bind Textures using texture units
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, texture0);
			//glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture0"), 0);
			//glActiveTexture(GL_TEXTURE1);
			//glBindTexture(GL_TEXTURE_2D, texture1);
			//glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture1"), 1);
			

			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();
			// Swap the screen buffers
			glfwSwapBuffers(window);
		}

	}
	catch(runtime_error er)
	{
		cout << "runtime_error " <<er.what() << endl;
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	glfwTerminate();

	return 0;
}

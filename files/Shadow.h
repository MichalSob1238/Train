#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include "shprogram.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

class Shadow {

	ShaderProgram shader;
	ShaderProgram simpleDepthShader;
	glm::vec3 lightPos;
	unsigned int depthMapFBO;
	unsigned int depthMap;
	const unsigned int SHADOW_WIDTH = 1024; 
	const unsigned int SHADOW_HEIGHT = 1024;
	glm::mat4 lightProjection; 
	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f;
	float far_plane = 7.5f;
	unsigned int SCR_WIDTH;
	unsigned int SCR_HEIGHT;
	Camera camera;
	
	void init(unsigned int width, unsigned int height, Camera cam);
	void render(const void (*renderScene)(ShaderProgram));
};
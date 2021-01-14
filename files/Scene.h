#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "shprogram.h"

#define MAX_NUM_OF_POINT_LIGHTS 10

class Scene
{
private:
    Scene(/* args */);
    ~Scene();

    glm::vec3 light_positions[MAX_NUM_OF_POINT_LIGHTS];
    glm::vec3 light_colors[MAX_NUM_OF_POINT_LIGHTS];
    bool is_light_used[MAX_NUM_OF_POINT_LIGHTS];
    unsigned int number_of_lights;

public:
    glm::vec3 ambient_light;

    ShaderProgram shape_shader;
    ShaderProgram light_shader;
    ShaderProgram shadow_shader;

    static Scene &getScene();

    void operator=(const Scene &) = delete;

    void setVec3InShaders(const std::string &name, glm::vec3 &value);
    void setMatrix4fvInShaders(const std::string &name, glm::mat4 &matrix);

    unsigned int addPointLightSource(glm::vec3 lightPos,glm::vec3 lightColor);
    void updatePointLightSource(unsigned int number, glm::vec3 lightPos,glm::vec3 lightColor);
    void removePointLightSource(unsigned int number);
    unsigned int getNUmberOfLights() { return number_of_lights; }
    void updateLights();
    void updateShadows();
};
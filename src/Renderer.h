#ifndef RENDERER_H
#define RENDERER_H

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "UI.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <array>
#include <map>

class Renderer
{
public:
	Renderer(int width, int height);
	~Renderer();

	void render(Camera &camera, bool wireframe);
    void rebuildFramebuffer(int width, int height);

private:
    typedef struct ObjectUBO
    {
        glm::mat4 PVM;
    }ObjectUBO;

    typedef struct FloorUBO
    {
        glm::mat4 PVM;
        glm::vec3 pos;
    }FloorUBO;

    typedef struct SkyboxUBO
    {
        glm::mat4 PV;
    }SkyboxUBO;

	typedef struct ReflectUBO
    {
        glm::mat4 PVM;
        glm::mat4 M;
        glm::mat4 transposedInvertedM;
        glm::vec3 camera;
    }ReflectUBO;

    float cubeVertices[180] = {
        //positions             //texture Coords
    	// Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left  
    };
    float cubeReflectVertices[216] = {
		//positions             //normals
		// Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Bottom-left
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // top-right
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // bottom-right         
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // top-right
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // bottom-left
        // Left face                       
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // top-right
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // top-left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // top-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // top-left
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // top-right         
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // top-left
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // top-right
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // top-right
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // top-left
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // top-right     
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // bottom-right
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // top-left
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f  // bottom-left  
    };
    float planeVertices[30] = 
    {
        //positions           //texture Coords
         10.0f, 0.0f,  10.0f,  10.0f, 0.0f,
        -10.0f, 0.0f, -10.0f,  0.0f, 10.0f,
        -10.0f, 0.0f,  10.0f,  0.0f, 0.0f,
                      
         10.0f, 0.0f,  10.0f,  10.0f, 0.0f,
         10.0f, 0.0f, -10.0f,  10.0f, 10.0f,
        -10.0f, 0.0f, -10.0f,  0.0f, 10.0f
    };
    float transparentVertices[30] = {
        // positions         //texture Coords
        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  1.0f
    };
    std::array<glm::vec3, 5> windows = 
    { 
        glm::vec3(-1.5f,  0.0f, -0.48f),
        glm::vec3(1.5f,  0.0f,  0.51f),
        glm::vec3(0.0f,  0.0f,  0.7f),
        glm::vec3(-0.3f,  0.0f, -2.3f),
        glm::vec3(0.5f,  0.0f, -0.6f)
    };
    float skyboxVertices[108] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    std::vector<std::string> texturePaths =
    {
			"textures/skybox/right.jpg",
            "textures/skybox/left.jpg",
            "textures/skybox/top.jpg",
            "textures/skybox/bottom.jpg",
            "textures/skybox/front.jpg",
            "textures/skybox/back.jpg"
    };

    GLuint FBO = 0;
    GLuint FBOtexture = 0;
    GLuint RBO = 0;

	std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;
	std::vector<GLuint> textures;
	std::vector<Shader*> programs;
    std::vector<GLuint> UBOs;
    ObjectUBO objectUBOs[7];
    FloorUBO floorUBO;
    SkyboxUBO skyboxUBO;
    ReflectUBO reflectUBO;

    glm::mat4 PV;

	void load2DTexture(const std::string &texturePath, GLint wrapMode);
    void loadCubeMap(const std::vector<std::string> &texturePath);
};

#endif

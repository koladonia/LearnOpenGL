#include "Renderer.h"

Renderer::Renderer(int width, int height) : rng(std::chrono::high_resolution_clock::now().time_since_epoch().count())
{
	//Static data construction and transfer
	GLuint VAO;
	GLuint VBO;
	GLuint UBO;

	glGenVertexArrays(1, &VAO);
	VAOs.push_back(VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	VBOs.push_back(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	float cubeVertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right      
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right	
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// left face
		-1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		// right face
		 1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right  
		 1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left 
		 1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-right
		 1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		 1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-left
		 1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-right
		-1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-left
		 1.0f,  1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-right  
		 1.0f,  1.0f , 1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-left 
		-1.0f,  1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f  // top-left   
	};

	float lampVertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right  
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right	
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		 1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right  
		 1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-left
		 1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-left
		 1.0f,  1.0f , 1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-right  
		 1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-left 
		-1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f  // bottom-left 

	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices) + sizeof(lampVertices), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVertices), cubeVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertices), sizeof(lampVertices), lampVertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glGenVertexArrays(1, &VAO);
	VAOs.push_back(VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	VBOs.push_back(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(lampVertices), &lampVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	//cameraUBO
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	//modelUBO
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(MVUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	//lightUBO
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	//lamps UBO
	for (int i = 0; i < 32; i++)
	{
		glGenBuffers(1, &UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(MVUBO), nullptr, GL_STATIC_DRAW);
		UBOs.push_back(UBO);
	}
	
	std::uniform_real_distribution<float> x(-2.2f, 2.2f);
	std::uniform_real_distribution<float> y(-2.2f, 2.2f);
	std::uniform_real_distribution<float> z(0.3f, 49.7f);
	std::uniform_real_distribution<float> col(0.0f, 10.0f);
	for (int i = 0; i < 32; i++)
	{
		lightPosStart[i] = glm::vec4(x(rng), y(rng), z(rng), 0.0f);
		lightColStart[i] = glm::vec4(col(rng), col(rng), col(rng), 0.0f);
	}

	programs.emplace_back("shaders/screen.vert", "null", "shaders/hdr.frag");
	programs.emplace_back("shaders/screen.vert", "null", "shaders/screenBlur.frag");
	programs.emplace_back("shaders/litHDRObj.vert", "null", "shaders/GBuffer.frag");
	programs.emplace_back("shaders/screen.vert", "null", "shaders/deferredLight.frag");
	programs.emplace_back("shaders/litHDRObj.vert", "null", "shaders/bloomObj.frag");

	load2DTexture("textures/wood.png", GL_REPEAT, true);
		
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	rebuildScreenFramebuffers(width, height);
	
	glEnable(GL_PROGRAM_POINT_SIZE);
}

Renderer::~Renderer()
{
	for (int i = 0; i < UBOs.size(); i++)
		glDeleteBuffers(1, &UBOs[i]);
	for (int i = 0; i < VAOs.size(); i++)
		glDeleteVertexArrays(1, &VAOs[i]);
	for (int i = 0; i < VBOs.size(); i++)
		glDeleteBuffers(1, &VBOs[i]);
	glDeleteTextures(textures.size(), textures.data());
	
	glDeleteRenderbuffers(1, &RBOGBuffer);
	glDeleteTextures(3, textureGBuffer);
	glDeleteFramebuffers(1, &GBuffer);
	
	glDeleteRenderbuffers(1, &RBOScreen);
	glDeleteTextures(2, textureScreen);
	glDeleteFramebuffers(1, &FBOScreen);
	
	glDeleteTextures(2, textureBloom);
	glDeleteFramebuffers(1, &FBOBloom);
}

void Renderer::load2DTexture(const std::string &texturePath, GLint wrapMode, bool srgb)
{
	int width, height, nrChannels;
	GLubyte* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Failed to load texture - " << texturePath << std::endl;
		stbi_image_free(data);
		return;
	}
	GLenum format;
	GLenum intFormat;
	switch (nrChannels)
	{
	case 1:
		intFormat = GL_RED;
		format = GL_RED;
		break;
	case 2:
		intFormat = GL_RG;
		format = GL_RG;
		break;
	case 3:
		intFormat = srgb ? GL_SRGB : GL_RGB;
		format = GL_RGB;
		break;
	case 4:
		intFormat = srgb ? GL_SRGB_ALPHA : GL_RGBA;
		format = GL_RGBA;
		break;
	default:
		intFormat = GL_RGB;
		format = GL_RGB;
		break;
	}
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	float aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, aniso);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, intFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	textures.push_back(texture);
}

void Renderer::loadCubeMap(const std::vector<std::string> &texturePaths)
{
	//Cubemap's texture origin is inverted
	stbi_set_flip_vertically_on_load(false);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	int width, height, nrChannels;
	unsigned char* data;
	for (unsigned int i = 0; i < texturePaths.size(); i++)
	{
		data = stbi_load(texturePaths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else
			std::cout << "Cubemap tex failed to load at path: " << texturePaths[i] << std::endl;
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//Clamp to edge so that when sampling exactly between two faces there would always be edge data
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	textures.push_back(texture);
	stbi_set_flip_vertically_on_load(true);
}

void Renderer::buildDepthMapFramebuffer(GLuint &FBO, GLuint &texture)
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glGenTextures(1, &depthMapTexture);
	//glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapTexture, 0);
	//Since no color attachments
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

void Renderer::updateUniforms(Camera& camera)
{
	//UBOs - cameraUBO, objectUBO, lightUBO
	//Update matrices and UBOs here
	if (camera.wasUpdatedP || camera.wasUpdatedV)
	{
		cameraUBO.P = camera.getP();
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[0]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(cameraUBO), &cameraUBO);
	}

	if (camera.wasUpdatedV)
	{
		modelUBO.VM = camera.getV();
		modelUBO.VM = glm::translate(modelUBO.VM, glm::vec3(0.0f, 0.0f, 25.0f));
		modelUBO.VM = glm::scale(modelUBO.VM, glm::vec3(2.5f, 2.5f, 27.5f));
		modelUBO.cofactorVM = glm::transpose(glm::adjugate(glm::mat3(modelUBO.VM)));
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[1]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(modelUBO), &modelUBO);
	}

	for (int i = 0; i < 32; i++)
	{
		std::chrono::duration<float, std::milli> durationSinceStart = std::chrono::high_resolution_clock::now().time_since_epoch();
		float time = durationSinceStart.count() / 1000.0f;
		lightUBO.lightPos[i].x = glm::sin(time + 2.0f * glm::pi<float>() * lightPosStart[i].x / 4.4f) * 2.2f;
		lightUBO.lightPos[i].y = glm::sin(time + 2.0f * glm::pi<float>() * lightPosStart[i].y / 4.4f) * 2.2f;
		lightUBO.lightPos[i].z = glm::sin(time + 2.0f * glm::pi<float>() * lightPosStart[i].z / 49.4f) * 24.85f + 25.15f;
		lightUBO.lightCol[i].x = glm::sin(time + 2.0f * glm::pi<float>() * lightColStart[i].x / 10.0f) * 5.0f + 5.0f;
		lightUBO.lightCol[i].y = glm::sin(time + 2.0f * glm::pi<float>() * lightColStart[i].y / 10.0f) * 5.0f + 5.0f;
		lightUBO.lightCol[i].z = glm::sin(time + 2.0f * glm::pi<float>() * lightColStart[i].z / 10.0f) * 5.0f + 5.0f;
		modelUBO.VM = camera.getV();
		modelUBO.VM = glm::translate(modelUBO.VM, glm::vec3(lightUBO.lightPos[i]));
		modelUBO.VM = glm::scale(modelUBO.VM, glm::vec3(0.25f, 0.25f, 0.25f));
		modelUBO.cofactorVM = glm::transpose(glm::adjugate(glm::mat3(modelUBO.VM)));
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[3 + i]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(modelUBO), &modelUBO);
		lightUBO.lightPos[i] = camera.getV() * lightUBO.lightPos[i];
	}
	glBindBuffer(GL_UNIFORM_BUFFER, UBOs[2]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(lightUBO), &lightUBO);
}

void Renderer::renderScene()
{
	programs[2].use();
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBOs[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, UBOs[1]);
	glBindVertexArray(VAOs[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::renderForward()
{
	programs[4].use();
	glBindVertexArray(VAOs[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBOs[0]);
	for (int i = 0; i < 32; i++)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, UBOs[3 + i]);
		glUniform3fv(0, 1, glm::value_ptr(glm::vec3(lightUBO.lightCol[i])));
		glDrawArrays(GL_TRIANGLES, 36, 36);
	}
}

void Renderer::render(Camera& camera, bool wireframe)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	updateUniforms(camera);

	//Create GBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, GBuffer);
	glViewport(0, 0, this->width, this->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	renderScene();

	//Render Lighting
	glBindFramebuffer(GL_FRAMEBUFFER, FBOScreen);
	for (int i = 0; i < 3; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureGBuffer[i]);
	}
	//Light UBO
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, UBOs[2]);
	programs[3].use();
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, GBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBOScreen);
	glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	renderForward();

	//Blur the bloom image
	glDisable(GL_DEPTH_TEST);
	if (bloom)
	{
		glViewport(0, 0, this->width / bloomDownscaleFactor, this->height / bloomDownscaleFactor);
		//Apply blur
		programs[1].use();
		glBindFramebuffer(GL_FRAMEBUFFER, FBOBloom);
		glActiveTexture(GL_TEXTURE0);
		for (int i = 0; i < bloomAmount; i++)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + (i % 2));
			if (i == 0)
				glBindTexture(GL_TEXTURE_2D, textureScreen[1]);
			else
				glBindTexture(GL_TEXTURE_2D, textureBloom[(i + 1) % 2]);
			//Horizontal-vertical ping-pong
			glUniform1i(0, i % 2);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::Begin("Bloom Debug", (bool*)0, overlayBox);
	ImGui::Checkbox("Enable Bloom", &bloom);
	if (bloom)
		ImGui::Image(reinterpret_cast<void*>(textureBloom[(bloomAmount + 1) % 2]), ImVec2(480, 270), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	
	//Render to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, this->width, this->height);
	programs[0].use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureScreen[0]);
	//Use last texture on which blur was applied
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureBloom[(bloomAmount + 1) % 2]);
	glUniform1i(0, bloom);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glEnable(GL_DEPTH_TEST);

	if (camera.wasUpdatedP || camera.wasUpdatedV)
	{
		camera.wasUpdatedP = false;
		camera.wasUpdatedV = false;
	}

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::rebuildScreenFramebuffers(int width, int height)
{
	this->width = width;
	this->height = height;

	if (GBuffer != 0)
	{
		glDeleteTextures(3, textureGBuffer);
		glDeleteRenderbuffers(1, &RBOGBuffer);
	}
	else
		glGenFramebuffers(1, &GBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, GBuffer);
	glGenTextures(3, textureGBuffer);
	for (int i = 0; i < 3; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textureGBuffer[i]);
		//8-bit float per component for albedo+specular texture
		glTexImage2D(GL_TEXTURE_2D, 0, i != 2 ? GL_RGBA16F : GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureGBuffer[i], 0);
	}
	glGenRenderbuffers(1, &RBOGBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, RBOGBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBOGBuffer);
	GLuint attachmentsGBuffer[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachmentsGBuffer);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "ERROR::FRAMEBUFFER:: GBuffer is not complete!" << std::endl;
	
	if (FBOScreen != 0)
	{
		glDeleteTextures(2, textureScreen);
		glDeleteRenderbuffers(1, &RBOScreen);
	}
	else
		glGenFramebuffers(1, &FBOScreen);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOScreen);
	glGenTextures(2, textureScreen);
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textureScreen[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureScreen[i], 0);
	}
	glGenRenderbuffers(1, &RBOScreen);
	glBindRenderbuffer(GL_RENDERBUFFER, RBOScreen);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBOScreen);
	GLuint attachmentsScreen[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachmentsScreen);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "ERROR::FRAMEBUFFER:: Resolved Screen Framebuffer is not complete!" << std::endl;

	if (FBOBloom != 0)
		glDeleteTextures(2, textureBloom);
	else
		glGenFramebuffers(1, &FBOBloom);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOBloom);
	glGenTextures(2, textureBloom);
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textureBloom[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->width / bloomDownscaleFactor, this->height / bloomDownscaleFactor, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureBloom[i], 0);
	}
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "ERROR::FRAMEBUFFER:: Resolved Screen Framebuffer is not complete!" << std::endl;
}

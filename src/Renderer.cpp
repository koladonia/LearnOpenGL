#include "Renderer.h"

Renderer::Renderer()
{
	//Object VAO
	VAOs.push_back(0);
	glGenVertexArrays(1, &VAOs[0]);
	glBindVertexArray(VAOs[0]);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	//Light VAO
	VAOs.push_back(0);
	glGenVertexArrays(1, &VAOs[1]);
	glBindVertexArray(VAOs[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//loadTexture("Textures/container.jpg", GL_RGB, GL_RGB);
	//loadTexture("Textures/awesomeface.png", GL_RGB, GL_RGBA);

	//Object program
	programs.push_back(0);
	programs[0] = new Shader("shaders/object.vert", "null", "shaders/object.frag");
	//Light program
	programs.push_back(0);
	programs[1] = new Shader("shaders/lamp.vert", "null", "shaders/lamp.frag");

	M = glm::mat4(1.0f);
}

Renderer::~Renderer()
{
	for (int i = 0; i < programs.size(); i++)
		delete programs[i];
	for (int i = 0; i < VAOs.size(); i++)
		glDeleteVertexArrays(1, &VAOs[i]);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
}

void Renderer::loadTexture(const std::string &texturePath, GLint internalFormat, GLenum format)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	GLubyte* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Failed to load texture - " << texturePath << std::endl;
		exit(1);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	textures.push_back(texture);
}

void Renderer::render(const Camera& camera, bool wireframe)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, textures[0]);
	//glActiveTexture(GL_TEXTURE0 + 1);
	//glBindTexture(GL_TEXTURE_2D, textures[1]);

	//Object
	programs[0]->use();
	glBindVertexArray(VAOs[0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, camera.getV());
	glUniformMatrix4fv(2, 1, GL_FALSE, camera.getP());
	M = glm::mat4(1.0f);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(M));
	glUniform3f(3, 1.0f, 0.5f, 0.31f); //object color
	glUniform3f(4, 1.0f, 1.0f, 1.0f); //light color
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Light
	programs[1]->use();
	glBindVertexArray(VAOs[1]);
	glUniformMatrix4fv(1, 1, GL_FALSE, camera.getV());
	glUniformMatrix4fv(2, 1, GL_FALSE, camera.getP());
	M = glm::translate(lightPosition);
	M = glm::scale(M, glm::vec3(0.2f));
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(M));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

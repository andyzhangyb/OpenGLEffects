#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Common/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // !STB_IMAGE_IMPLEMENTATION

const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;

using namespace std;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "outline", NULL, NULL);

	if (!window) {
		cout << "failed to create window!" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to load glad." << endl;
		return -1;
	}
	
	Shader shader("Resources/Shaders/Noise/2DPerlin.vs", "Resources/Shaders/Noise/2DPerlin.fs");
	float vertices[] = {
		-1.0f, 1.0f, // top-left
		1.0f, 1.0f, // top-right
		1.0f, -1.0f, // bottom-right

		-1.0f, 1.0f, // top-left
		1.0f, -1.0f, // bottom-right
		-1.0f, -1.0f  // bottom-left
	};

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	shader.Use();
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);

	return 0;
}

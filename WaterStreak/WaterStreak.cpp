#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Common/Shader.h"

using namespace std;

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif // !STB_IMAGE_IMPLEMENTATION

const unsigned int WIDTH = 1920;
const unsigned int HEIGHT = 1080;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "water streak", NULL, NULL);
	if (!window) {
		cout << "Failed to create window." << endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow *, int width, int height) {
		glViewport(0, 0, width, height);
	});
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			cout << "You click left button." << endl;
		}
	});
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
	});

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD." << endl;
		return -1;
	}
	float vertices[] = {
		1.f, 1.f, 0.f,	1.f, 1.f, //top-right
		1.f, -1.f, 0.f, 1.f, 0.f, //bottom-right
		-1.f, -1.f, 0.f, 0.f, 0.f, //bottom-left
		-1.f, 1.f, 0.f, 0.f, 1.f //top-left
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

	unsigned int textureId;
	glGenTextures(1, &textureId);
	string fileName = "Resources/Textures/FreeSkyBackground.jpg";
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int textureWidth, textureHeight, textureNrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char * picData = stbi_load(fileName.c_str(), &textureWidth, &textureHeight, &textureNrChannels, 0);
	if (picData) {
		GLenum picFormat;
		if (textureNrChannels == 1)
			picFormat = GL_RED;
		else if (textureNrChannels == 3)
			picFormat = GL_RGB;
		else if (textureNrChannels == 4)
			picFormat = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, picFormat, textureWidth, textureHeight, 0, picFormat, GL_UNSIGNED_BYTE, picData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "failed to load texture : " << fileName << endl;
	}
	stbi_image_free(picData);
	Shader shader("Resources/Shaders/WaterStreak/water_streak.vs", "Resources/Shaders/WaterStreak/water_streak.fs");
	shader.Use();
	shader.Set1UniformValue("texture1", 0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);

		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		shader.Use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}
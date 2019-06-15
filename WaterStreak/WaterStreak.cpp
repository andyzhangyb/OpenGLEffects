#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Common/Shader.h"

using namespace std;

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif // !STB_IMAGE_IMPLEMENTATION

const unsigned int WIDTH = 900;
const unsigned int HEIGHT = 900;
const unsigned int MaxPointsCount = 256;
unsigned int uboPoints;

vector<glm::vec3> points;

void copyUniformToGPU();

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
            double xPos, yPos;
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            glfwGetCursorPos(window, &xPos, &yPos);
            
            float x = xPos / width * 2  - 1;
            float y = -yPos / height * 2 + 1;
            
            if (points.size() >= MaxPointsCount) {
                
            }
            else
                points.push_back(glm::vec3(x, y, glfwGetTime()));
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

	unsigned int frameBuffer;
	glGenFramebuffers(1, &frameBuffer);

	unsigned int textureIds[2];
	glGenTextures(2, textureIds);
	string fileName = "Resources/Textures/UnderSea.jpg";
	glBindTexture(GL_TEXTURE_2D, *textureIds);
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
		glTexImage2D(GL_TEXTURE_2D, 0, picFormat, WIDTH, HEIGHT, 0, picFormat, GL_UNSIGNED_BYTE, picData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "failed to load texture : " << fileName << endl;
	}
	stbi_image_free(picData);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glBindTexture(GL_TEXTURE_2D, *(textureIds + 1));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(textureIds + 1), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader shaderStreak("Resources/Shaders/WaterStreak/water_streak.vs", "Resources/Shaders/WaterStreak/water_streak.fs");
	shaderStreak.Use();
	shaderStreak.Set1UniformValue("texture1", 0);
	shaderStreak.Set1UniformValue("textureNormal", 1);
	Shader shaderNormal("Resources/Shaders/WaterStreak/water_normal.vs", "Resources/Shaders/WaterStreak/water_normal.fs");

	unsigned int uboSize = MaxPointsCount * 16 + 16;
	glGenBuffers(1, &uboPoints);
	glBindBuffer(GL_UNIFORM_BUFFER, uboPoints);
	glBufferData(GL_UNIFORM_BUFFER, uboSize, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboPoints, 0, uboSize);

	unsigned int uniformBlockIndex = glGetUniformBlockIndex(shaderStreak.ProgramID, "LightSourceBlock");
	glUniformBlockBinding(shaderStreak.ProgramID, uniformBlockIndex, 0);
    
    copyUniformToGPU();

	while (!glfwWindowShouldClose(window)) {
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		copyUniformToGPU();

		shaderNormal.Use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderStreak.Use();
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *textureIds);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, *(textureIds + 1));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteTextures(2, textureIds);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}

void copyUniformToGPU() {
    float time = glfwGetTime();
    glBindBuffer(GL_UNIFORM_BUFFER, uboPoints);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &time);
    for (int i = 0; i < points.size(); i++) {
        if (i >= MaxPointsCount)
            break;
        int offset = i * 16 + 16;
        if (points[i].x == 0 && points[i].y == 0 && points[i].z == 0) {
            glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(points[i]));
            break;
        } else
            glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(points[i]));
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

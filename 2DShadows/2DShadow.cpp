#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Common/Shader.h"

using namespace std;

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif // !STB_IMAGE_IMPLEMENTATION

const unsigned int WIDTH = 900;
const unsigned int HEIGHT = 900;
const unsigned int WIDTH_SHADOW = 900;
const unsigned int HEIGHT_SHADOW = 900;
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

void renderCube();
void generateCubeBox(float * vertexs, int originSize, int step, float ** outVertexs, int * size);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "2D shadow", NULL, NULL);
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
            
			lightPos = glm::vec3(x, y, 0);
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

	float verticesObstacle[] = {
		-0.5f, -0.25f, 0.0f,
		-0.25f, 0.20f, 0.0f,
		-0.15f, -0.45f, 0.0f
	};

	float verticesObstacle2[] = {
		0.25f, 0.25f, 0.0f,
		0.75f, 0.25f, 0.0f,
		0.5f, 0.75f, 0.0f
	};

	float verticesPanel[] = {
		1.f, 1.f, 0.0f,
		1.f, -1.f, 0.0f,
		-1.f, -1.f, 0.0f,
		-1.f, 1.f, 0.0f
	};
	unsigned int indicesPanel[] = {
		0, 1, 3,
		1, 2, 3
	};

	float * finalVerticesObstacle;
	int finalSize;
	generateCubeBox(verticesObstacle, sizeof(verticesObstacle) / sizeof(float), 3, &finalVerticesObstacle, &finalSize);

	float * finalVerticesObstacle2;
	int finalSize2;
	generateCubeBox(verticesObstacle2, sizeof(verticesObstacle2) / sizeof(float), 3, &finalVerticesObstacle2, &finalSize2);

	unsigned int VAO[5], VBO[5], EBO;
	glGenVertexArrays(5, VAO);
	glGenBuffers(5, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, finalSize * sizeof(float), finalVerticesObstacle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, finalSize2 * sizeof(float), finalVerticesObstacle2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(verticesObstacle), verticesObstacle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesObstacle2), verticesObstacle2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPanel), verticesPanel, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPanel), indicesPanel, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindVertexArray(0);

	unsigned int frameBuffer;
	glGenFramebuffers(1, &frameBuffer);

	unsigned int depthCubeMap;
	glGenTextures(1, &depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	for (size_t i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, WIDTH_SHADOW, HEIGHT_SHADOW, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader shaderShadowMap("Resources/Shaders/2DShadows/2DShadowsDepth.vs", "Resources/Shaders/2DShadows/2DShadowsDepth.fs", "Resources/Shaders/2DShadows/2DShadowsDepth.gs");
	Shader shaderDraw("Resources/Shaders/2DShadows/2DShadows.vs", "Resources/Shaders/2DShadows/2DShadows.fs");
	shaderDraw.Use();
	shaderDraw.Set1UniformValue("depthMap", 0);

	glEnable(GL_DEPTH_TEST);

	float nearPlane = 0.0f;
	float farPlane = 5.0f;
	while (!glfwWindowShouldClose(window)) {
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderShadowMap.Use();
		shaderShadowMap.Set2UniformValue("lightPos", lightPos.x, lightPos.y);
		shaderShadowMap.Set1UniformValue("farPlane", farPlane);
		
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.f), (float)WIDTH_SHADOW / (float)HEIGHT_SHADOW, nearPlane, farPlane);
		glm::mat4 matrix;
		matrix = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		shaderShadowMap.SetMat4("shadowMatrices[0]", matrix);
		matrix = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		shaderShadowMap.SetMat4("shadowMatrices[1]", matrix);
		matrix = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shaderShadowMap.SetMat4("shadowMatrices[2]", matrix);
		matrix = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		shaderShadowMap.SetMat4("shadowMatrices[3]", matrix);

		glViewport(0, 0, WIDTH_SHADOW, HEIGHT_SHADOW);
		renderCube();
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, finalSize);

		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, finalSize2);

		glViewport(0, 0, WIDTH, HEIGHT);
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderDraw.Use();
		shaderDraw.Set2UniformValue("lightPos", lightPos.x, lightPos.y);
		shaderDraw.Set1UniformValue("farPlane", farPlane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

		shaderDraw.Set1UniformValue("drawTrueColor", 0);
		glBindVertexArray(VAO[4]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		shaderDraw.Set1UniformValue("drawTrueColor", 1);
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VAO[3]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteTextures(1, &depthCubeMap);
	glDeleteVertexArrays(5, VAO);
	glDeleteBuffers(5, VBO);
	glDeleteBuffers(1, &EBO);

	delete finalVerticesObstacle;
	delete finalVerticesObstacle2;

	glfwTerminate();

	return 0;
}

void generateCubeBox(float * originVertexs, int originSize, int step, float ** outVertexs, int * outSize) {
	float * newVertexsBuffer = new float[originSize / step * 6 * 3];
	*outVertexs = newVertexsBuffer;
	*outSize = originSize / step * 6 * 3;
	for (int i = 0; i < originSize / step; ++i) {
		float * start = originVertexs + i * step;
		glm::vec3 first(start[0], start[1], 0.f);
		glm::vec3 second;
		if (i == originSize / step - 1) {
			second = glm::vec3(originVertexs[0], originVertexs[1], 0.f);
		}
		else {
			second = glm::vec3(start[step], start[step + 1], 0.f);
		}

		glm::vec3 first1 = first + glm::vec3(0.f, 0.f, 0.1f);
		glm::vec3 first2 = first + glm::vec3(0.f, 0.f, -0.1f);
		glm::vec3 second1 = second + glm::vec3(0.f, 0.f, 0.1f);
		glm::vec3 second2 = second + glm::vec3(0.f, 0.f, -0.1f);

		glm::vec3 order[6] = { first1, first2, second1, second1, second2, first2 };
		float * starWrite = newVertexsBuffer + i * 6 * 3;
		for (int j = 0; j < 6; j++)
		{
			starWrite[j * 3 + 0] = order[j].x;
			starWrite[j * 3 + 1] = order[j].y;
			starWrite[j * 3 + 2] = order[j].z;
		}
	}
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

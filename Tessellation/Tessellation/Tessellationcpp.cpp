#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "camera.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void formVertex(int patchNum, int iter, int u, int v, glm::mat4 P_x, glm::mat4 P_y, glm::mat4 P_z);
void processInput(GLFWwindow *window);
void getControlPoints();
void generateVertices();

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Control point/vertices data
const int LEVEL = 15;
const int NUM_PATCHES = 32;
const int NUM_TRIS_PER_SQUARE = 2;
const int NUM_VERTS_PER_TRI = 3;
const int NUM_FLOATS_PER_VEC3 = 3;
const int NUM_CNTRL_PTS_PER_PATCH = 16;
float controlPoints[NUM_PATCHES * NUM_CNTRL_PTS_PER_PATCH * NUM_FLOATS_PER_VEC3];
float vertices[NUM_PATCHES * LEVEL * LEVEL * NUM_TRIS_PER_SQUARE * NUM_VERTS_PER_TRI * NUM_FLOATS_PER_VEC3];

unsigned int controlVBO, controlVAO;
unsigned int verticesVBO, verticesVAO;

int main()
{
	// Configuring GLFW settings
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creating window object 
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tessellation", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Manage OpenGL function pointers via GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Configure openGL settings
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader teapotShader("shader.vert", "shader.frag");
	getControlPoints();
	generateVertices();

	// 5) Rendering loop
	while (!glfwWindowShouldClose(window))
	{
		// Per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		// Render background color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		// Construct teapot
		teapotShader.Use();
		teapotShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		teapotShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		glm::mat4 teapotModel = glm::mat4(1.0f);
		teapotModel = glm::translate(teapotModel, glm::vec3(0.0f, -2.0, -2.0f));
		teapotModel = glm::rotate(teapotModel, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 teapotMVP = projection * view * teapotModel;
		teapotShader.setMat4("mvp", teapotMVP);

		glBindVertexArray(verticesVAO);
		for (int patchID = 0; patchID < NUM_PATCHES; patchID++)
		{
			glDrawArrays(GL_TRIANGLES, patchID * LEVEL * LEVEL * NUM_TRIS_PER_SQUARE * NUM_VERTS_PER_TRI, LEVEL * LEVEL * NUM_TRIS_PER_SQUARE * NUM_VERTS_PER_TRI);
		}
		glBindVertexArray(0);

		/*
		glBindVertexArray(controlVAO);
		for (int i = 0; i < 32; i++)
		{
			glDrawArrays(GL_TRIANGLE_STRIP, i * 16, 16);
		}
		glBindVertexArray(0);
		*/

		// GLFW: swap buffer and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &controlVAO);
	glDeleteBuffers(1, &controlVBO);
	glDeleteVertexArrays(1, &verticesVAO);
	glDeleteBuffers(1, &verticesVBO);
	
	glfwTerminate();
	return 0;
}

// Process all input
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	// Camera movement input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(FOREWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboard(LOOKLEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboard(LOOKRIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(LOOKUP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboard(LOOKDOWN, deltaTime);
}

// Resize callback function that allows user to resize the window 
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void getControlPoints()
{
	// Get data from teapot text file
	std::string line;
	std::ifstream myFile;
	myFile.open("data.txt");

	if (myFile.is_open())
	{
		// Ignore the first line
		getline(myFile, line);
		for (int patchID = 0; patchID < NUM_PATCHES; patchID++)
		{
			// Ignore the line with "3 3"
			getline(myFile, line);
			for (int vertID = 0; vertID < NUM_CNTRL_PTS_PER_PATCH; vertID++)
			{
				getline(myFile, line);
				std::istringstream iss(line);
				std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };
				for (int k = 0; k < NUM_FLOATS_PER_VEC3; k++)
				{
					float value = std::stof(tokens[k]);
					controlPoints[k + NUM_FLOATS_PER_VEC3 * vertID + NUM_CNTRL_PTS_PER_PATCH * NUM_FLOATS_PER_VEC3 * patchID] = value;
				}
			}
		}
		myFile.close();
	}
	else
	{
		std::cout << "Unable to open file";
	}

	// Generate VAO and VBO
	glGenVertexArrays(1, &controlVAO);
	glGenBuffers(1, &controlVBO);
	glBindVertexArray(controlVAO);
	glBindBuffer(GL_ARRAY_BUFFER, controlVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(controlPoints), controlPoints, GL_STATIC_DRAW);
	glVertexAttribPointer(0, NUM_FLOATS_PER_VEC3, GL_FLOAT, GL_FALSE, NUM_FLOATS_PER_VEC3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void generateVertices()
{
	// Get vertices from tessellating control points
	for (int patchID = 0; patchID < NUM_PATCHES; patchID++)
	{
		float xValues[NUM_CNTRL_PTS_PER_PATCH];
		float yValues[NUM_CNTRL_PTS_PER_PATCH];
		float zValues[NUM_CNTRL_PTS_PER_PATCH];
		for (int vertID = 0; vertID < NUM_CNTRL_PTS_PER_PATCH; vertID++)
		{
			xValues[vertID] = controlPoints[0 + vertID * NUM_FLOATS_PER_VEC3 + patchID * NUM_CNTRL_PTS_PER_PATCH * NUM_FLOATS_PER_VEC3];
			yValues[vertID] = controlPoints[1 + vertID * NUM_FLOATS_PER_VEC3 + patchID * NUM_CNTRL_PTS_PER_PATCH * NUM_FLOATS_PER_VEC3];
			zValues[vertID] = controlPoints[2 + vertID * NUM_FLOATS_PER_VEC3 + patchID * NUM_CNTRL_PTS_PER_PATCH * NUM_FLOATS_PER_VEC3];
		}

		glm::mat4 P_x = glm::make_mat4(xValues);
		glm::mat4 P_y = glm::make_mat4(yValues);
		glm::mat4 P_z = glm::make_mat4(zValues);

		int iter = 0;
		for (int k = 0; k < LEVEL; k++)
		{
			// Tri formation pattern:
			// Format: (u, v)
			// i = 0: (k, 0) (k, 1) (k + 1, 0)
			//		  +1 +1
			// i = 1: (k + 1, 1) (k, 1) (k + 1, 0)
			//							+0 +2 <- check if v value exceeds limit
			// i = 2: (k + 1, 1) (k, 1) (k + 1, 2)
			//		  -1	 +1
			// i = 3: (k, 2) (k, 1) (k + 1, 2)
			//				 +0 +2 <- check if v value exceeds limit
			int u_0 = k;
			int v_0 = 0;
			int u_1 = k;
			int v_1 = 1;
			int u_2 = k + 1;
			int v_2 = 0;
			int counter = 0;
			while (v_1 <= LEVEL && v_2 <= LEVEL)
			{
				// Input vertices for first triangle
				formVertex(patchID, iter, u_0, v_0, P_x, P_y, P_z);
				iter += NUM_FLOATS_PER_VEC3;
				// Input vertices for second triangle
				formVertex(patchID, iter, u_1, v_1, P_x, P_y, P_z);
				iter += NUM_FLOATS_PER_VEC3;
				// Input vertices for third triangle
				formVertex(patchID, iter, u_2, v_2, P_x, P_y, P_z);
				iter += NUM_FLOATS_PER_VEC3;

				if (counter % 4 == 0)
				{
					u_0 += 1;
					v_0 += 1;
				}
				if (counter % 4 == 1)
				{
					v_2 += 2;
				}
				if (counter % 4 == 2)
				{
					u_0 -= 1;
					v_0 += 1;
				}
				if (counter % 4 == 3)
				{
					v_1 += 2;
				}
				counter++;
			}
		}
	}

	// Generate vertex array and buffers
	glGenVertexArrays(1, &verticesVAO);
	glGenBuffers(1, &verticesVBO);
	glBindVertexArray(verticesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, NUM_FLOATS_PER_VEC3, GL_FLOAT, GL_FALSE, NUM_FLOATS_PER_VEC3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

// Multiplication order: v * Bz_t * P * Bz * u
void formVertex(int patchID, int iter, int u, int v, glm::mat4 P_x, glm::mat4 P_y, glm::mat4 P_z)
{
	float u_value = (float)u / (float)LEVEL;
	float v_value = (float)v / (float)LEVEL;

	glm::vec4 u_vec = glm::vec4(1, u_value, u_value * u_value, u_value * u_value * u_value);
	glm::vec4 v_vec = glm::vec4(1, v_value, v_value * v_value, v_value * v_value * v_value);
	glm::mat4 Bz = glm::mat4(1, 0, 0, 0, -3, 3, 0, 0, 3, -6, 3, 0, -1, 3, -3, 1);

	glm::vec4 x_vec = glm::transpose(Bz) * P_x * Bz * u_vec;
	glm::vec4 y_vec = glm::transpose(Bz) * P_y * Bz * u_vec;
	glm::vec4 z_vec = glm::transpose(Bz) * P_z * Bz * u_vec;

	float x = v_vec.x * x_vec.x + v_vec.y * x_vec.y + v_vec.z * x_vec.z + v_vec.w * x_vec.w;
	float y = v_vec.x * y_vec.x + v_vec.y * y_vec.y + v_vec.z * y_vec.z + v_vec.w * y_vec.w;
	float z = v_vec.x * z_vec.x + v_vec.y * z_vec.y + v_vec.z * z_vec.z + v_vec.w * z_vec.w;

	vertices[0 + iter + patchID * LEVEL * LEVEL * NUM_TRIS_PER_SQUARE * NUM_VERTS_PER_TRI * NUM_FLOATS_PER_VEC3] = x;
	vertices[1 + iter + patchID * LEVEL * LEVEL * NUM_TRIS_PER_SQUARE * NUM_VERTS_PER_TRI * NUM_FLOATS_PER_VEC3] = y;
	vertices[2 + iter + patchID * LEVEL * LEVEL * NUM_TRIS_PER_SQUARE * NUM_VERTS_PER_TRI * NUM_FLOATS_PER_VEC3] = z;
}
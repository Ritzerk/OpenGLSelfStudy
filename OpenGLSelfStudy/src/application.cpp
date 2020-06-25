#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <cmath>
#include <filesystem>
#include "Shader.h"
#include "Camera.h"

void adjustViewportToWindowSize(GLFWwindow* window, int width, int height);
void checkEsc(GLFWwindow* window);
void processInput(GLFWwindow* window, float& val);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Tab name", NULL, NULL);
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1; 
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, 800, 600);	//size of GL rendering window. 
	glfwSetFramebufferSizeCallback(window, adjustViewportToWindowSize);		//Viewport needs to be resized everytime user resizes window, this is done using a callback. 
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader firstShader("shaders/shader.vs", "shaders/shader.fs");

	float vertices3D[] = {
	-0.5f, -0.5f, -0.5f,      0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,      1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,      1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,      1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,      0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,      0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,     0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,     0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,     1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,     1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,     1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,     0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,     0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,     0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,     0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	//Bind Buffers to data next
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3D), vertices3D, GL_STATIC_DRAW);
	

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Do the same for texture - keep it at location 2 stil here and at the shader
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	//Set the texture wrapping/filtering options on the currently bound texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		//Repeats texture where the object is bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Linear for downscailing images - the returned colour of pixel is a mix (interpolated) value of colours, if we are downscailing, then we can't show every single pixel on the image inside the object.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //Linear as we don't use mipmap when upscailing. 


	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* txtjpg = stbi_load("./res/container.jpg", &width, &height, &nrChannels, 0);

	if (txtjpg)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, txtjpg);	//The bound object now has the texture bound to it.
		glGenerateMipmap(GL_TEXTURE_2D);		//A mipmap is a set of different sizes of the texture, to best suit the dimensions of our shape we link the texture too. This function auto-generates those mipmaps.
	}
	else
	{
		std::cout << stbi_failure_reason() << std::endl;
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(txtjpg);	//Texture is already bound, so we can free the image memory.

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//Set the texture wrapping/filtering options on the currently bound texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		//Repeats texture where the object is bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	//Linear for downscailing images - the returned colour of pixel is a mix (interpolated) value of colours, if we are downscailing, then we can't show every single pixel on the image inside the object.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //Linear as we don't use mipmap when upscailing. 


	unsigned char* data = stbi_load("./res/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		std::cout << stbi_failure_reason() << std::endl;
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	firstShader.use();
	firstShader.setInt("texture1", 0); //glUniform1i(glGetUniformLocation(firstShader.programID, "texture1"), 0);
	firstShader.setInt("texture2", 1);

	float visibility = 1.0f;

	glm::mat4 identityMatrix = glm::mat4(1.0f); //this creates an identity matrix

	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);


	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window, visibility);
		glfwSetScrollCallback(window, scroll_callback);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    //calling glClear sets the background to color values set by glClearColor function.

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		firstShader.use();

		projection = glm::perspective(glm::radians(camera.Fov), 800.0f / 600.0f, 0.1f, 100.0f); 
		view = camera.GetViewMatrix();
		
		firstShader.setMatrix4fv("projection", glm::value_ptr(projection));
		firstShader.setMatrix4fv("view", glm::value_ptr(view));

		//render multiple boxes
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);

			float angle = 20.0f * i;
			if (i % 3 == 0 ) {
				angle = (float)glfwGetTime() * 25.0f;
			}
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			firstShader.setMatrix4fv("model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		
		firstShader.setFloat("myVariation", visibility);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clear up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0; 

}

void adjustViewportToWindowSize(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

void checkEsc(GLFWwindow *window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void processInput(GLFWwindow *window, float &val) 
{
	checkEsc(window);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
	{
		val += 0.001f;
		if (val >= 1.0f) {
			val = 1.0f;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
	{
		val -= 0.001f;
		if (val <= 0.0f) {
			val = 0.0f;
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
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

void adjustViewportToWindowSize(GLFWwindow* window, int width, int height);
void checkEsc(GLFWwindow* window);
void processInput(GLFWwindow* window, float &val);

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

	glViewport(0, 0, 800, 600);	//size of GL rendering window. 
	glfwSetFramebufferSizeCallback(window, adjustViewportToWindowSize);		//Viewport needs to be resized everytime user resizes window, this is done using a callback. 

	

	

	Shader firstShader("shaders/shader.vs", "shaders/shader.fs");
	

	float triangleVertices[] = {
		//positions vec 4		//colours rgb vec 3		//texture coords
		 0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,		1.0f, 1.0f,		//top right
		 0.5f, -0.5f, 0.0f,	    0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		//bottom right
		-0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		//bottom left
		-0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,		0.0f, 1.0f		//top left
	}; 

	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//Bind Buffers to data next
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Do the same for colour - not using colour atm
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Do the same for texture - keep it at location 2 stil here and at the shader
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
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
	glUniform1i(glGetUniformLocation(firstShader.programID, "texture1"), 0);
	firstShader.setInt("texture2", 1);

	glBindVertexArray(0);
	float visibility = 1.0f;

	glm::mat4 identityMatrix = glm::mat4(1.0f); //this creates an identity matrix
	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); //this is our vector we want to apply a translation to
	//glm::mat4 trans = glm::translate(identityMatrix, glm::vec3(1.0f, 1.0f, 0.0f));  //Identity matrix + Translation vec3, creates the translation matrix (movement). vec3 is the Tx, Ty, Tz
	//vec = trans * vec;   //We multiply matrix by vector(in that order!)
	//std::cout << vec.x << vec.y << vec.z << std::endl; 

	//Scailing and rotating the container object
	glm::mat4 trans;
	
	//trans = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); //rotating around Z (z is 1 in vec3) by 90 degrees (we convert to radians using glm::radians)
	
	

	unsigned int transformLoc = glGetUniformLocation(firstShader.programID, "transformation");

	while (!glfwWindowShouldClose(window))
	{
		checkEsc(window);	
		processInput(window, visibility);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);				//calling glClear sets the background to color values set by glClearColor function.

		trans = glm::translate(identityMatrix, glm::vec3(0.5f, -0.5f, 0.0f));   //movement xy.
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); //rotation in z with time
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5)); //scailing - making it be 0.5 of its normal size

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		//Order here will be important - apply scailing first, then rotations, and then translations from right to left (bottom command to top command)

		firstShader.use();
		firstShader.setFloat("myVariation", visibility);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

		float sintime = sin(glfwGetTime());
		//Second Container - exercise 2
		trans = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.5f, 0.0f));
		trans = glm::scale(trans, glm::vec3(sintime, sintime, sintime));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
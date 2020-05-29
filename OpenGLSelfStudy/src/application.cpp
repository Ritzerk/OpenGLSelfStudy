#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

void adjustViewportToWindowSize(GLFWwindow* window, int width, int height);
void checkEsc(GLFWwindow* window);

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

	const char* vertexShaderSource = "#version 430 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 430 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//Attaching shaders to program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	//Can delete shader objects after they are linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float triangleVertices[] = {
		 0.5f,  0.5f, 0.0f,		//top right
		 0.5f, -0.5f, 0.0f,		//bottom right
		-0.5f, -0.5f, 0.0f,		//bottom left
		-0.5f,  0.5f, 0.0f		//top left
	};

	float triangleVertices1[] = {
		-0.50f, 0.0f, 0.0f,
		-0.25f, 0.5f, 0.0f,
		 0.00f, 0.0f, 0.0f	
	};

	float triangleVertices2[] = {
		0.0f, 0.0f, 0.0f,
		0.25f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 3,	//first triangle
		1, 2, 3		//second triangle
	};

	/*unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);		//bind VAO first

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	//an element buffer enables us to reuse our vertices if they have the same cooridinates when drawing shapes. 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Next, need to tell opengl how to interpret the vertex data in memory and how it should connect the vertex data to the vertex shader's attributes.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);					//this registers the currently bound VBO as the vertex attribute's bound vertex buffer object. 

	//You can safely unbind VAO and VBO after the instruction before this one. You don't need to, but can. Then rebind it whenever you're using this VAO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);*/

	//Exercise 1 - With two different VBOs, we can have 1 VAO but need to swap what is bound to it.
	unsigned int aVBO[2], VAO2;
	glGenVertexArrays(1, &VAO2);
	

	glBindVertexArray(VAO2);
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);		//format setup without a buffer
	glVertexAttribBinding(0, 0);
	glBindVertexArray(0);

	//Bind Buffers to data next
	glGenBuffers(2, aVBO);
	glBindBuffer(GL_ARRAY_BUFFER, aVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices1), triangleVertices1, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, aVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices2), triangleVertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	

	while (!glfwWindowShouldClose(window))
	{
		checkEsc(window);	

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);				//calling glClear sets the background to color values set by glClearColor function.

		glUseProgram(shaderProgram);				//Every rendering call will have to use this program, hence use the shaders. If no VAO is bound, it has to be bound now.

		//Exercise 1 - Drawing two triangles next to each other. Method 2: Changing VBO bound to VAO using openGL 4.3
		glBindVertexArray(VAO2);

		glBindVertexBuffer(0, aVBO[0], 0, 3*sizeof(float));
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexBuffer(0, aVBO[1], 0, 3*sizeof(float));
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clear up
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, aVBO);
	glDeleteProgram(shaderProgram);

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
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

void adjustViewportToWindowSize(GLFWwindow* window, int width, int height);
void checkEsc(GLFWwindow* window);

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

	const char* vertexShaderSource = "#version 430 core\n"
		"layout (location = 0) in vec3 aPos;\n"								//this line sets the generic attribute index (attribindex/generic vertex attribute), which we use when binding and so on. 
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

	const char* fragmentShaderSourceYellow = "#version 430 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgramOrange, shaderProgramYellow;
	shaderProgramOrange = glCreateProgram();
	shaderProgramYellow = glCreateProgram();

	//Attaching shaders to program
	glAttachShader(shaderProgramOrange, vertexShader);
	glAttachShader(shaderProgramOrange, fragmentShader);
	glLinkProgram(shaderProgramOrange);

	//Now link the extra yellow shader, by overwriting the current fragment shader and then attaching the shader to the yellow program. 
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceYellow, NULL);
	glCompileShader(fragmentShader);

	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShader);
	glLinkProgram(shaderProgramYellow);


	//Can delete shader objects after they are linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float triangleVertices[] = {
		-0.6f, 0.0f, 0.0f,
		-0.35f, 0.5f, 0.0f,
		-0.1f, 0.0f, 0.0f
	};

	float triangleVertices2[] = {
		 0.1f, 0.0f, 0.0f,
		 0.35f, 0.5f, 0.0f,
		 0.60f, 0.0f, 0.0f
	};


	//Exercise 1 - With two different VBOs, we can have 1 VAO but need to swap what is bound to it.
	unsigned int VBO[2], VAO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]);

	//Bind Buffers to data next
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices2), triangleVertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		checkEsc(window);	

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);				//calling glClear sets the background to color values set by glClearColor function.

		glUseProgram(shaderProgramOrange);				//Every rendering call will have to use this program, hence use the shaders. If no VAO is bound, it has to be bound now.

		//Exercise 3: One orange and one yellow triangle.
		glBindVertexArray(VAO[0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glUseProgram(shaderProgramYellow);

		glBindVertexArray(VAO[1]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clear up
	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(1, VBO);
	glDeleteProgram(shaderProgramOrange);
	glDeleteProgram(shaderProgramYellow);

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
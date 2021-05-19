
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//most basic vertex shader written in GLSL
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


int main()
{	
	//glfw: intialize and configure
	// ---------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					//Tells OpenGL we'd like to use version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					//Tells OpenGL we'd like to use version ^.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//Tells OpenGL we'd explicitly want to use the core-profile

	//glfw: window creation
	// ----------------------------
	//glfwCreateWindow(width, height, windowName, ignore, ignore)
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window); //Make the context of our window the main context on the current thread
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//Pass GLAD the function to load teh address of the OpenGL function pointers which are OS-specific
	//glfwGetProcAddress defines the correct function based on which OS we're compiling for
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return EXIT_FAILURE;
	}

	//Build and compile shader program
	// ----------------------------------------
	//compile vertex shader from above written in GLSL
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//check for successful compilation
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//compile fragment shader from above writen in GLSL
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Link Shaders
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//check for successful shader program linking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKER::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//Activate shader program object so every shader and rendering call will use this program object
	glUseProgram(shaderProgram);
	//Delete shader objects after successful creation of shader program object
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//glViewport(locationOfLowerLeftCornerX, locationOfLowerLeftCornerY, width, height)
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//framebuffer_size_callback(GLFWwindow Pointer, newWindowWidth, newWindowHeight)
	//Whenever window size changes, function is called and fills in the proper arguments for you to process.
	
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};

	//Unique vertex buffer object taking bugger ID 1
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	//Bind newly created bugger to the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Copy user-defined data into the currently bound buffer (to VBO as glBindBuffer means calls to GL_ARRAY_BUFFER configure VBO)
	//glBufferData(typeOfBuffer, sizeOfBufferInBytes, actualDataWeWantToSend, specifyHowGraphicsCardManagesGivenData)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Tell OpenGL how to interpret the vertex data after binding vertex buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Render loop
	while (!glfwWindowShouldClose(window)) {

		//input
		processInput(window);

		//Rendering commands
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


//process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

//whenever the window size changed this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


// PAGE 34 CURRENTLY

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					//Tells OpenGL we'd like to use version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					//Tells OpenGL we'd like to use version ^.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//Tells OpenGL we'd explicitly want to use the core-profile

	//glfwCreateWindow(width, height, windowName, ignore, ignore)
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window); //Make the context of our window the main context on the current thread

	//Pass GLAD the function to load teh address of the OpenGL function pointers which are OS-specific
	//glfwGetProcAddress defines the correct function based on which OS we're compiling for
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return EXIT_FAILURE;
	}

	//glViewport(locationOfLowerLeftCornerX, locationOfLowerLeftCornerY, width, height)
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//framebuffer_size_callback(GLFWwindow Pointer, newWindowWidth, newWindowHeight)
	//Whenever window size changes, function is called and fills in the proper arguments for you to process.
	
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//Bind newly created bugger to the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Copy use-defined data into the currently bound buffer (to VBO as glBindBuffer means calls to GL_ARRAY_BUFFER configure VBO)
	//glBufferData(typeOfBuffer, sizeOfBufferInBytes, actualDataWeWantToSend, specifyHowGraphicsCardManagesGivenData)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Render loop
	while (!glfwWindowShouldClose(window)) {

		//input
		processInput(window);

		//Rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


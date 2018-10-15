
#include <GL\glew.h>
#include <glfw\glfw3.h>
#include <iostream>

struct Color
{
public:
	float R, G, B, A;
};

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const Color CLEAR_COLOR = { 0.2F, 0.2F, 0.2F, 1.0F };

const char *VERTEX_SHADER = "#version 330 core\nlayout(location = 0) in vec3 aPos;void main(){gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}";
const char *FRAGMENT_SHADER = "#version 330 core\nout vec4 FragColor;void main(){FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);}";

void OnWindowResized(GLFWwindow *Window, int Width, int Height)
{
	glViewport(0, 0, Width, Height);
}

void ProcessInput(GLFWwindow *Window)
{
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(Window, true);
}

GLFWwindow *CreateWindow(int Width, int Height, const char *Title)
{
	GLFWwindow *window = glfwCreateWindow(Width, Height, Title, nullptr, nullptr);

	if (window == nullptr)
		return nullptr;

	glfwSetFramebufferSizeCallback(window, OnWindowResized);

	OnWindowResized(window, Width, Height);

	return window;
}

unsigned int CreateTriangle()
{
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float vertices[] = {
		-0.5F, -0.5F, 0.0F,
		0.5F, -0.5F, 0.0F,
		0.0F, 0.5F, 0.0F
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	return vbo;
}

unsigned int CompileShader(const char *VertexShader, const char *FragmentShader)
{
	unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &VertexShader, nullptr);
	glCompileShader(vertShader);

	unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &FragmentShader, nullptr);
	glCompileShader(fragShader);

	unsigned int program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	return program;
}

void main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Game");
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return;
	}

	glfwMakeContextCurrent(window);

	glewInit();

	unsigned int trianglesVBO = CreateTriangle();
	unsigned int shader = CompileShader(VERTEX_SHADER, FRAGMENT_SHADER);

	glClearColor(CLEAR_COLOR.R, CLEAR_COLOR.G, CLEAR_COLOR.B, CLEAR_COLOR.A);

	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

Finalize:
	glfwTerminate();

	return;
}
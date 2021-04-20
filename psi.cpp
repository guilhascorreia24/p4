#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
glm::mat4 MVP;
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Guilhas", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive
	// vertices give a triangle. A cube has 6 faces with 2 triangles each, so this
	// makes 6*2=12 triangles, and 12*3 vertices
	// ------------------------------------------------------------------
	float vertices[] = {
			10.8f, 9.2f, 0.125f,
			12.8f, 8.5f, 0.125f,
			14.7f, 7.9f, 0.125f,

			14.7f, 7.9f, 0.125f,
			16.7f, 7.4f, 0.125f,
			18.7f, 7.2f, 0.125f,

			18.7f, 7.2f, 0.125f,
			20.3f, 8.3f, 0.125f,
			21.0f, 10.3f, 0.125f,

			21.0f, 10.3f, 0.125f,
			21.3f, 12.3f, 0.125f,
			21.6f, 14.3f, 0.125f,

			21.6f, 14.3f, 0.125f,
			21.8f, 16.3f, 0.125f,
			22.0f, 18.4f, 0.125f,

			34.8f, 23.6f, 0.125f,
			34.8f, 21.6f, 0.125f,
			34.7f, 19.5f, 0.125f,

			34.7f, 19.5f, 0.125f,
			34.7f, 17.5f, 0.125f,
			34.7f, 15.4f, 0.125f,

			34.7f, 15.4f, 0.125f,
			34.7f, 13.4f, 0.125f,
			34.8f, 11.3f, 0.125f,

			34.8f, 11.3f, 0.125f,
			35.5f, 9.5f, 0.125f,
			37.3f, 8.5f, 0.125f,

			37.3f, 8.5f, 0.125f,
			39.3f, 8.2f, 0.125f,
			40.2f, 9.9f, 0.125f,

			40.2f, 9.9f, 0.125f,
			40.4f, 12.0f, 0.125f,
			40.4f, 14.0f, 0.125f,

			40.4f, 14.0f, 0.125f,
			40.4f, 16.0f, 0.125f,
			40.3f, 18.1f, 0.125f,

			40.3f, 18.1f, 0.125f,
			40.3f, 20.1f, 0.125f,
			40.2f, 22.2f, 0.125f,

			53.3f, 14.9f, 0.125f,
			53.6f, 12.9f, 0.125f,
			54.2f, 10.9f, 0.125f,

			54.2f, 10.9f, 0.125f,
			55.9f, 9.8f, 0.125f,
			57.8f, 9.3f, 0.125f,

			57.8f, 9.3f, 0.125f,
			59.9f, 9.0f, 0.125f,
			61.7f, 9.6f, 0.125f,

			58.1f, 19.0f, 0.125f,
			57.8f, 21.0f, 0.125f,
			57.4f, 23.1f, 0.125f,

			57.4f, 23.1f, 0.125f,
			57.0f, 25.0f, 0.125f,
			56.4f, 27.0f, 0.125f,

			56.4f, 27.0f, 0.125f,
			55.8f, 29.0f, 0.125f,
			55.1f, 30.9f, 0.125f,

			55.1f, 30.9f, 0.125f,
			54.3f, 32.8f, 0.125f,
			53.3f, 34.5f, 0.125f,

			53.3f, 34.5f, 0.125f,
			52.1f, 36.2f, 0.125f,
			50.7f, 37.7f, 0.125f,

			50.7f, 37.7f, 0.125f,
			49.0f, 38.8f, 0.125f,
			47.1f, 39.6f, 0.125f,

			47.1f, 39.6f, 0.125f,
			45.1f, 39.9f, 0.125f,
			43.1f, 40.0f, 0.125f,

			43.0f, 56.2f, 0.125f,
			45.0f, 56.7f, 0.125f,
			46.8f, 57.5f, 0.125f,

			46.8f, 57.5f, 0.125f,
			45.5f, 58.4f, 0.125f,
			43.5f, 58.7f, 0.125f,

			43.5f, 58.7f, 0.125f,
			41.4f, 58.9f, 0.125f,
			39.4f, 58.9f, 0.125f,

			39.4f, 58.9f, 0.125f,
			37.3f, 58.9f, 0.125f,
			35.3f, 58.9f, 0.125f,

			35.3f, 58.9f, 0.125f,
			33.3f, 58.9f, 0.125f,
			31.2f, 58.7f, 0.125f,

			31.2f, 58.7f, 0.125f,
			29.2f, 58.5f, 0.125f,
			28.2f, 57.4f, 0.125f,

			32.6f, 40.9f, 0.125f,
			30.6f, 40.5f, 0.125f,
			28.6f, 40.1f, 0.125f,

			28.6f, 40.1f, 0.125f,
			26.6f, 39.6f, 0.125f,
			24.7f, 38.9f, 0.125f,

			24.7f, 38.9f, 0.125f,
			23.0f, 37.9f, 0.125f,
			21.4f, 36.5f, 0.125f,

			21.4f, 36.5f, 0.125f,
			20.2f, 34.9f, 0.125f,
			19.2f, 33.1f, 0.125f,

			19.2f, 33.1f, 0.125f,
			18.5f, 31.2f, 0.125f,
			18.0f, 29.2f, 0.125f,

			18.0f, 29.2f, 0.125f,
			17.6f, 27.2f, 0.125f,
			17.2f, 25.2f, 0.125f,

			17.2f, 25.2f, 0.125f,
			16.9f, 23.2f, 0.125f,
			16.7f, 21.1f, 0.125f,

			12.4f, 11.2f, 0.125f,
			10.4f, 11.5f, 0.125f,
			8.3f, 11.5f, 0.125f,

			8.3f, 11.5f, 0.125f,
			9.0f, 10.1f, 0.125f,
			10.8f, 9.2f, 0.125f,

			10.8f, 9.2f, 0.125f,
			14.7f, 7.9f, 0.125f,
			18.7f, 7.2f, 0.125f,

			18.7f, 7.2f, 0.125f,
			21.0f, 10.3f, 0.125f,
			21.6f, 14.3f, 0.125f,

			21.6f, 14.3f, 0.125f,
			22.0f, 18.4f, 0.125f,
			22.1f, 20.4f, 0.125f,

			34.8f, 23.6f, 0.125f,
			34.7f, 19.5f, 0.125f,
			34.7f, 15.4f, 0.125f,

			34.7f, 15.4f, 0.125f,
			34.8f, 11.3f, 0.125f,
			37.3f, 8.5f, 0.125f,

			37.3f, 8.5f, 0.125f,
			40.2f, 9.9f, 0.125f,
			40.4f, 14.0f, 0.125f,

			40.4f, 14.0f, 0.125f,
			40.3f, 18.1f, 0.125f,
			40.2f, 22.2f, 0.125f,

			52.9f, 16.9f, 0.125f,
			53.3f, 14.9f, 0.125f,
			54.2f, 10.9f, 0.125f,

			54.2f, 10.9f, 0.125f,
			57.8f, 9.3f, 0.125f,
			61.7f, 9.6f, 0.125f,

			58.1f, 19.0f, 0.125f,
			57.4f, 23.1f, 0.125f,
			56.4f, 27.0f, 0.125f,

			56.4f, 27.0f, 0.125f,
			55.1f, 30.9f, 0.125f,
			53.3f, 34.5f, 0.125f,

			53.3f, 34.5f, 0.125f,
			50.7f, 37.7f, 0.125f,
			47.1f, 39.6f, 0.125f,

			43.0f, 56.2f, 0.125f,
			46.8f, 57.5f, 0.125f,
			43.5f, 58.7f, 0.125f,

			43.5f, 58.7f, 0.125f,
			39.4f, 58.9f, 0.125f,
			35.3f, 58.9f, 0.125f,

			35.3f, 58.9f, 0.125f,
			31.2f, 58.7f, 0.125f,
			28.2f, 57.4f, 0.125f,

			34.6f, 41.2f, 0.125f,
			32.6f, 40.9f, 0.125f,
			28.6f, 40.1f, 0.125f,

			28.6f, 40.1f, 0.125f,
			24.7f, 38.9f, 0.125f,
			21.4f, 36.5f, 0.125f,

			21.4f, 36.5f, 0.125f,
			19.2f, 33.1f, 0.125f,
			18.0f, 29.2f, 0.125f,

			18.0f, 29.2f, 0.125f,
			17.2f, 25.2f, 0.125f,
			16.7f, 21.1f, 0.125f,

			12.4f, 11.2f, 0.125f,
			8.3f, 11.5f, 0.125f,
			10.8f, 9.2f, 0.125f,

			10.8f, 9.2f, 0.125f,
			18.7f, 7.2f, 0.125f,
			21.6f, 14.3f, 0.125f,

			34.8f, 23.6f, 0.125f,
			34.7f, 15.4f, 0.125f,
			37.3f, 8.5f, 0.125f,

			37.3f, 8.5f, 0.125f,
			40.4f, 14.0f, 0.125f,
			40.2f, 22.2f, 0.125f,

			52.6f, 18.9f, 0.125f,
			52.9f, 16.9f, 0.125f,
			54.2f, 10.9f, 0.125f,

			54.2f, 10.9f, 0.125f,
			61.7f, 9.6f, 0.125f,
			60.7f, 11.3f, 0.125f,

			58.6f, 17.0f, 0.125f,
			58.1f, 19.0f, 0.125f,
			56.4f, 27.0f, 0.125f,

			56.4f, 27.0f, 0.125f,
			53.3f, 34.5f, 0.125f,
			47.1f, 39.6f, 0.125f,

			41.5f, 54.9f, 0.125f,
			43.0f, 56.2f, 0.125f,
			43.5f, 58.7f, 0.125f,

			43.5f, 58.7f, 0.125f,
			35.3f, 58.9f, 0.125f,
			28.2f, 57.4f, 0.125f,

			34.6f, 41.2f, 0.125f,
			28.6f, 40.1f, 0.125f,
			21.4f, 36.5f, 0.125f,

			21.4f, 36.5f, 0.125f,
			18.0f, 29.2f, 0.125f,
			16.7f, 21.1f, 0.125f,

			14.4f, 11.3f, 0.125f,
			12.4f, 11.2f, 0.125f,
			10.8f, 9.2f, 0.125f,

			34.9f, 25.6f, 0.125f,
			34.8f, 23.6f, 0.125f,
			37.3f, 8.5f, 0.125f,

			37.3f, 8.5f, 0.125f,
			40.2f, 22.2f, 0.125f,
			40.1f, 24.2f, 0.125f,

			52.2f, 20.9f, 0.125f,
			52.6f, 18.9f, 0.125f,
			54.2f, 10.9f, 0.125f,

			54.2f, 10.9f, 0.125f,
			60.7f, 11.3f, 0.125f,
			59.7f, 13.1f, 0.125f,

			58.6f, 17.0f, 0.125f,
			56.4f, 27.0f, 0.125f,
			47.1f, 39.6f, 0.125f,

			41.5f, 54.9f, 0.125f,
			43.5f, 58.7f, 0.125f,
			28.2f, 57.4f, 0.125f,

			21.4f, 36.5f, 0.125f,
			16.7f, 21.1f, 0.125f,
			16.4f, 19.1f, 0.125f,

			14.4f, 11.3f, 0.125f,
			10.8f, 9.2f, 0.125f,
			21.6f, 14.3f, 0.125f,

			34.9f, 27.7f, 0.125f,
			34.9f, 25.6f, 0.125f,
			37.3f, 8.5f, 0.125f,

			37.3f, 8.5f, 0.125f,
			40.1f, 24.2f, 0.125f,
			40.0f, 26.3f, 0.125f,

			51.9f, 22.9f, 0.125f,
			52.2f, 20.9f, 0.125f,
			54.2f, 10.9f, 0.125f,

			54.2f, 10.9f, 0.125f,
			59.7f, 13.1f, 0.125f,
			59.1f, 15.1f, 0.125f,

			59.1f, 15.1f, 0.125f,
			58.6f, 17.0f, 0.125f,
			47.1f, 39.6f, 0.125f,

			41.5f, 54.9f, 0.125f,
			28.2f, 57.4f, 0.125f,
			30.2f, 56.9f, 0.125f,

			21.4f, 36.5f, 0.125f,
			16.4f, 19.1f, 0.125f,
			16.1f, 17.1f, 0.125f,

			15.3f, 13.1f, 0.125f,
			14.4f, 11.3f, 0.125f,
			21.6f, 14.3f, 0.125f,

			34.9f, 29.7f, 0.125f,
			34.9f, 27.7f, 0.125f,
			37.3f, 8.5f, 0.125f,

			37.3f, 8.5f, 0.125f,
			40.0f, 26.3f, 0.125f,
			40.0f, 28.3f, 0.125f,

			51.9f, 22.9f, 0.125f,
			54.2f, 10.9f, 0.125f,
			59.1f, 15.1f, 0.125f,

			41.5f, 54.9f, 0.125f,
			30.2f, 56.9f, 0.125f,
			32.1f, 56.4f, 0.125f,

			21.4f, 36.5f, 0.125f,
			16.1f, 17.1f, 0.125f,
			15.8f, 15.1f, 0.125f,

			15.8f, 15.1f, 0.125f,
			15.3f, 13.1f, 0.125f,
			21.6f, 14.3f, 0.125f,

			34.9f, 31.8f, 0.125f,
			34.9f, 29.7f, 0.125f,
			37.3f, 8.5f, 0.125f,

			37.3f, 8.5f, 0.125f,
			40.0f, 28.3f, 0.125f,
			39.9f, 30.3f, 0.125f,

			51.4f, 24.9f, 0.125f,
			51.9f, 22.9f, 0.125f,
			59.1f, 15.1f, 0.125f,

			41.0f, 53.0f, 0.125f,
			41.5f, 54.9f, 0.125f,
			32.1f, 56.4f, 0.125f,

			21.4f, 36.5f, 0.125f,
			15.8f, 15.1f, 0.125f,
			21.6f, 14.3f, 0.125f,

			34.8f, 33.8f, 0.125f,
			34.9f, 31.8f, 0.125f,
			37.3f, 8.5f, 0.125f,

			37.3f, 8.5f, 0.125f,
			39.9f, 30.3f, 0.125f,
			39.9f, 32.4f, 0.125f,

			51.0f, 26.9f, 0.125f,
			51.4f, 24.9f, 0.125f,
			59.1f, 15.1f, 0.125f,

			40.8f, 50.9f, 0.125f,
			41.0f, 53.0f, 0.125f,
			32.1f, 56.4f, 0.125f,

			21.4f, 36.5f, 0.125f,
			21.6f, 14.3f, 0.125f,
			22.1f, 20.4f, 0.125f,

			34.7f, 35.8f, 0.125f,
			34.8f, 33.8f, 0.125f,
			37.3f, 8.5f, 0.125f,

			37.3f, 8.5f, 0.125f,
			39.9f, 32.4f, 0.125f,
			39.9f, 34.4f, 0.125f,

			50.5f, 28.9f, 0.125f,
			51.0f, 26.9f, 0.125f,
			59.1f, 15.1f, 0.125f,

			40.7f, 48.9f, 0.125f,
			40.8f, 50.9f, 0.125f,
			32.1f, 56.4f, 0.125f,

			21.4f, 36.5f, 0.125f,
			22.1f, 20.4f, 0.125f,
			22.4f, 22.4f, 0.125f,

			34.7f, 35.8f, 0.125f,
			37.3f, 8.5f, 0.125f,
			39.9f, 34.4f, 0.125f,

			49.8f, 30.8f, 0.125f,
			50.5f, 28.9f, 0.125f,
			59.1f, 15.1f, 0.125f,

			40.7f, 48.9f, 0.125f,
			32.1f, 56.4f, 0.125f,
			33.6f, 55.0f, 0.125f,

			21.4f, 36.5f, 0.125f,
			22.4f, 22.4f, 0.125f,
			22.6f, 24.5f, 0.125f,

			34.4f, 37.9f, 0.125f,
			34.7f, 35.8f, 0.125f,
			39.9f, 34.4f, 0.125f,

			49.1f, 32.7f, 0.125f,
			49.8f, 30.8f, 0.125f,
			59.1f, 15.1f, 0.125f,

			40.6f, 46.8f, 0.125f,
			40.7f, 48.9f, 0.125f,
			33.6f, 55.0f, 0.125f,

			21.4f, 36.5f, 0.125f,
			22.6f, 24.5f, 0.125f,
			23.0f, 26.5f, 0.125f,

			34.4f, 37.9f, 0.125f,
			39.9f, 34.4f, 0.125f,
			40.1f, 36.5f, 0.125f,

			48.2f, 34.5f, 0.125f,
			49.1f, 32.7f, 0.125f,
			59.1f, 15.1f, 0.125f,

			40.6f, 44.8f, 0.125f,
			40.6f, 46.8f, 0.125f,
			33.6f, 55.0f, 0.125f,

			21.4f, 36.5f, 0.125f,
			23.0f, 26.5f, 0.125f,
			23.5f, 28.5f, 0.125f,

			32.7f, 38.6f, 0.125f,
			34.4f, 37.9f, 0.125f,
			40.1f, 36.5f, 0.125f,

			48.2f, 34.5f, 0.125f,
			59.1f, 15.1f, 0.125f,
			47.1f, 39.6f, 0.125f,

			40.7f, 42.8f, 0.125f,
			40.6f, 44.8f, 0.125f,
			33.6f, 55.0f, 0.125f,

			21.4f, 36.5f, 0.125f,
			23.5f, 28.5f, 0.125f,
			24.1f, 30.4f, 0.125f,

			32.7f, 38.6f, 0.125f,
			40.1f, 36.5f, 0.125f,
			41.2f, 37.9f, 0.125f,

			46.9f, 36.1f, 0.125f,
			48.2f, 34.5f, 0.125f,
			47.1f, 39.6f, 0.125f,

			41.3f, 40.8f, 0.125f,
			40.7f, 42.8f, 0.125f,
			33.6f, 55.0f, 0.125f,

			21.4f, 36.5f, 0.125f,
			24.1f, 30.4f, 0.125f,
			25.0f, 32.2f, 0.125f,

			32.7f, 38.6f, 0.125f,
			41.2f, 37.9f, 0.125f,
			43.3f, 37.9f, 0.125f,

			45.2f, 37.3f, 0.125f,
			46.9f, 36.1f, 0.125f,
			47.1f, 39.6f, 0.125f,

			41.3f, 40.8f, 0.125f,
			33.6f, 55.0f, 0.125f,
			34.4f, 53.1f, 0.125f,

			21.4f, 36.5f, 0.125f,
			25.0f, 32.2f, 0.125f,
			26.2f, 33.9f, 0.125f,

			43.3f, 37.9f, 0.125f,
			45.2f, 37.3f, 0.125f,
			47.1f, 39.6f, 0.125f,

			41.3f, 40.8f, 0.125f,
			34.4f, 53.1f, 0.125f,
			34.8f, 51.1f, 0.125f,

			21.4f, 36.5f, 0.125f,
			26.2f, 33.9f, 0.125f,
			27.6f, 35.4f, 0.125f,

			32.7f, 38.6f, 0.125f,
			43.3f, 37.9f, 0.125f,
			47.1f, 39.6f, 0.125f,

			41.3f, 40.8f, 0.125f,
			34.8f, 51.1f, 0.125f,
			34.9f, 49.1f, 0.125f,

			21.4f, 36.5f, 0.125f,
			27.6f, 35.4f, 0.125f,
			29.1f, 36.7f, 0.125f,

			32.7f, 38.6f, 0.125f,
			47.1f, 39.6f, 0.125f,
			43.1f, 40.0f, 0.125f,

			41.3f, 40.8f, 0.125f,
			34.9f, 49.1f, 0.125f,
			35.0f, 47.1f, 0.125f,

			34.6f, 41.2f, 0.125f,
			21.4f, 36.5f, 0.125f,
			29.1f, 36.7f, 0.125f,

			32.7f, 38.6f, 0.125f,
			43.1f, 40.0f, 0.125f,
			41.3f, 40.8f, 0.125f,

			41.3f, 40.8f, 0.125f,
			35.0f, 47.1f, 0.125f,
			35.0f, 45.0f, 0.125f,

			34.6f, 41.2f, 0.125f,
			29.1f, 36.7f, 0.125f,
			30.8f, 37.9f, 0.125f,

			41.3f, 40.8f, 0.125f,
			35.0f, 45.0f, 0.125f,
			35.0f, 43.0f, 0.125f,

			34.6f, 41.2f, 0.125f,
			30.8f, 37.9f, 0.125f,
			32.7f, 38.6f, 0.125f,

			41.3f, 40.8f, 0.125f,
			35.0f, 43.0f, 0.125f,
			34.6f, 41.2f, 0.125f,

			34.6f, 41.2f, 0.125f,
			32.7f, 38.6f, 0.125f,
			41.3f, 40.8, 0.125f,

			/////////////////

			10.8f, 9.2f, -0.125f,
			12.8f, 8.5f, -0.125f,
			14.7f, 7.9f, -0.125f,

			14.7f, 7.9f, -0.125f,
			16.7f, 7.4f, -0.125f,
			18.7f, 7.2f, -0.125f,

			18.7f, 7.2f, -0.125f,
			20.3f, 8.3f, -0.125f,
			21.0f, 10.3f, -0.125f,

			21.0f, 10.3f, -0.125f,
			21.3f, 12.3f, -0.125f,
			21.6f, 14.3f, -0.125f,

			21.6f, 14.3f, -0.125f,
			21.8f, 16.3f, -0.125f,
			22.0f, 18.4f, -0.125f,

			34.8f, 23.6f, -0.125f,
			34.8f, 21.6f, -0.125f,
			34.7f, 19.5f, -0.125f,

			34.7f, 19.5f, -0.125f,
			34.7f, 17.5f, -0.125f,
			34.7f, 15.4f, -0.125f,

			34.7f, 15.4f, -0.125f,
			34.7f, 13.4f, -0.125f,
			34.8f, 11.3f, -0.125f,

			34.8f, 11.3f, -0.125f,
			35.5f, 9.5f, -0.125f,
			37.3f, 8.5f, -0.125f,

			37.3f, 8.5f, -0.125f,
			39.3f, 8.2f, -0.125f,
			40.2f, 9.9f, -0.125f,

			40.2f, 9.9f, -0.125f,
			40.4f, 12.0f, -0.125f,
			40.4f, 14.0f, -0.125f,

			40.4f, 14.0f, -0.125f,
			40.4f, 16.0f, -0.125f,
			40.3f, 18.1f, -0.125f,

			40.3f, 18.1f, -0.125f,
			40.3f, 20.1f, -0.125f,
			40.2f, 22.2f, -0.125f,

			53.3f, 14.9f, -0.125f,
			53.6f, 12.9f, -0.125f,
			54.2f, 10.9f, -0.125f,

			54.2f, 10.9f, -0.125f,
			55.9f, 9.8f, -0.125f,
			57.8f, 9.3f, -0.125f,

			57.8f, 9.3f, -0.125f,
			59.9f, 9.0f, -0.125f,
			61.7f, 9.6f, -0.125f,

			58.1f, 19.0f, -0.125f,
			57.8f, 21.0f, -0.125f,
			57.4f, 23.1f, -0.125f,

			57.4f, 23.1f, -0.125f,
			57.0f, 25.0f, -0.125f,
			56.4f, 27.0f, -0.125f,

			56.4f, 27.0f, -0.125f,
			55.8f, 29.0f, -0.125f,
			55.1f, 30.9f, -0.125f,

			55.1f, 30.9f, -0.125f,
			54.3f, 32.8f, -0.125f,
			53.3f, 34.5f, -0.125f,

			53.3f, 34.5f, -0.125f,
			52.1f, 36.2f, -0.125f,
			50.7f, 37.7f, -0.125f,

			50.7f, 37.7f, -0.125f,
			49.0f, 38.8f, -0.125f,
			47.1f, 39.6f, -0.125f,

			47.1f, 39.6f, -0.125f,
			45.1f, 39.9f, -0.125f,
			43.1f, 40.0f, -0.125f,

			43.0f, 56.2f, -0.125f,
			45.0f, 56.7f, -0.125f,
			46.8f, 57.5f, -0.125f,

			46.8f, 57.5f, -0.125f,
			45.5f, 58.4f, -0.125f,
			43.5f, 58.7f, -0.125f,

			43.5f, 58.7f, -0.125f,
			41.4f, 58.9f, -0.125f,
			39.4f, 58.9f, -0.125f,

			39.4f, 58.9f, -0.125f,
			37.3f, 58.9f, -0.125f,
			35.3f, 58.9f, -0.125f,

			35.3f, 58.9f, -0.125f,
			33.3f, 58.9f, -0.125f,
			31.2f, 58.7f, -0.125f,

			31.2f, 58.7f, -0.125f,
			29.2f, 58.5f, -0.125f,
			28.2f, 57.4f, -0.125f,

			32.6f, 40.9f, -0.125f,
			30.6f, 40.5f, -0.125f,
			28.6f, 40.1f, -0.125f,

			28.6f, 40.1f, -0.125f,
			26.6f, 39.6f, -0.125f,
			24.7f, 38.9f, -0.125f,

			24.7f, 38.9f, -0.125f,
			23.0f, 37.9f, -0.125f,
			21.4f, 36.5f, -0.125f,

			21.4f, 36.5f, -0.125f,
			20.2f, 34.9f, -0.125f,
			19.2f, 33.1f, -0.125f,

			19.2f, 33.1f, -0.125f,
			18.5f, 31.2f, -0.125f,
			18.0f, 29.2f, -0.125f,

			18.0f, 29.2f, -0.125f,
			17.6f, 27.2f, -0.125f,
			17.2f, 25.2f, -0.125f,

			17.2f, 25.2f, -0.125f,
			16.9f, 23.2f, -0.125f,
			16.7f, 21.1f, -0.125f,

			12.4f, 11.2f, -0.125f,
			10.4f, 11.5f, -0.125f,
			8.3f, 11.5f, -0.125f,

			8.3f, 11.5f, -0.125f,
			9.0f, 10.1f, -0.125f,
			10.8f, 9.2f, -0.125f,

			10.8f, 9.2f, -0.125f,
			14.7f, 7.9f, -0.125f,
			18.7f, 7.2f, -0.125f,

			18.7f, 7.2f, -0.125f,
			21.0f, 10.3f, -0.125f,
			21.6f, 14.3f, -0.125f,

			21.6f, 14.3f, -0.125f,
			22.0f, 18.4f, -0.125f,
			22.1f, 20.4f, -0.125f,

			34.8f, 23.6f, -0.125f,
			34.7f, 19.5f, -0.125f,
			34.7f, 15.4f, -0.125f,

			34.7f, 15.4f, -0.125f,
			34.8f, 11.3f, -0.125f,
			37.3f, 8.5f, -0.125f,

			37.3f, 8.5f, -0.125f,
			40.2f, 9.9f, -0.125f,
			40.4f, 14.0f, -0.125f,

			40.4f, 14.0f, -0.125f,
			40.3f, 18.1f, -0.125f,
			40.2f, 22.2f, -0.125f,

			52.9f, 16.9f, -0.125f,
			53.3f, 14.9f, -0.125f,
			54.2f, 10.9f, -0.125f,

			54.2f, 10.9f, -0.125f,
			57.8f, 9.3f, -0.125f,
			61.7f, 9.6f, -0.125f,

			58.1f, 19.0f, -0.125f,
			57.4f, 23.1f, -0.125f,
			56.4f, 27.0f, -0.125f,

			56.4f, 27.0f, -0.125f,
			55.1f, 30.9f, -0.125f,
			53.3f, 34.5f, -0.125f,

			53.3f, 34.5f, -0.125f,
			50.7f, 37.7f, -0.125f,
			47.1f, 39.6f, -0.125f,

			43.0f, 56.2f, -0.125f,
			46.8f, 57.5f, -0.125f,
			43.5f, 58.7f, -0.125f,

			43.5f, 58.7f, -0.125f,
			39.4f, 58.9f, -0.125f,
			35.3f, 58.9f, -0.125f,

			35.3f, 58.9f, -0.125f,
			31.2f, 58.7f, -0.125f,
			28.2f, 57.4f, -0.125f,

			34.6f, 41.2f, -0.125f,
			32.6f, 40.9f, -0.125f,
			28.6f, 40.1f, -0.125f,

			28.6f, 40.1f, -0.125f,
			24.7f, 38.9f, -0.125f,
			21.4f, 36.5f, -0.125f,

			21.4f, 36.5f, -0.125f,
			19.2f, 33.1f, -0.125f,
			18.0f, 29.2f, -0.125f,

			18.0f, 29.2f, -0.125f,
			17.2f, 25.2f, -0.125f,
			16.7f, 21.1f, -0.125f,

			12.4f, 11.2f, -0.125f,
			8.3f, 11.5f, -0.125f,
			10.8f, 9.2f, -0.125f,

			10.8f, 9.2f, -0.125f,
			18.7f, 7.2f, -0.125f,
			21.6f, 14.3f, -0.125f,

			34.8f, 23.6f, -0.125f,
			34.7f, 15.4f, -0.125f,
			37.3f, 8.5f, -0.125f,

			37.3f, 8.5f, -0.125f,
			40.4f, 14.0f, -0.125f,
			40.2f, 22.2f, -0.125f,

			52.6f, 18.9f, -0.125f,
			52.9f, 16.9f, -0.125f,
			54.2f, 10.9f, -0.125f,

			54.2f, 10.9f, -0.125f,
			61.7f, 9.6f, -0.125f,
			60.7f, 11.3f, -0.125f,

			58.6f, 17.0f, -0.125f,
			58.1f, 19.0f, -0.125f,
			56.4f, 27.0f, -0.125f,

			56.4f, 27.0f, -0.125f,
			53.3f, 34.5f, -0.125f,
			47.1f, 39.6f, -0.125f,

			41.5f, 54.9f, -0.125f,
			43.0f, 56.2f, -0.125f,
			43.5f, 58.7f, -0.125f,

			43.5f, 58.7f, -0.125f,
			35.3f, 58.9f, -0.125f,
			28.2f, 57.4f, -0.125f,

			34.6f, 41.2f, -0.125f,
			28.6f, 40.1f, -0.125f,
			21.4f, 36.5f, -0.125f,

			21.4f, 36.5f, -0.125f,
			18.0f, 29.2f, -0.125f,
			16.7f, 21.1f, -0.125f,

			14.4f, 11.3f, -0.125f,
			12.4f, 11.2f, -0.125f,
			10.8f, 9.2f, -0.125f,

			34.9f, 25.6f, -0.125f,
			34.8f, 23.6f, -0.125f,
			37.3f, 8.5f, -0.125f,

			37.3f, 8.5f, -0.125f,
			40.2f, 22.2f, -0.125f,
			40.1f, 24.2f, -0.125f,

			52.2f, 20.9f, -0.125f,
			52.6f, 18.9f, -0.125f,
			54.2f, 10.9f, -0.125f,

			54.2f, 10.9f, -0.125f,
			60.7f, 11.3f, -0.125f,
			59.7f, 13.1f, -0.125f,

			58.6f, 17.0f, -0.125f,
			56.4f, 27.0f, -0.125f,
			47.1f, 39.6f, -0.125f,

			41.5f, 54.9f, -0.125f,
			43.5f, 58.7f, -0.125f,
			28.2f, 57.4f, -0.125f,

			21.4f, 36.5f, -0.125f,
			16.7f, 21.1f, -0.125f,
			16.4f, 19.1f, -0.125f,

			14.4f, 11.3f, -0.125f,
			10.8f, 9.2f, -0.125f,
			21.6f, 14.3f, -0.125f,

			34.9f, 27.7f, -0.125f,
			34.9f, 25.6f, -0.125f,
			37.3f, 8.5f, -0.125f,

			37.3f, 8.5f, -0.125f,
			40.1f, 24.2f, -0.125f,
			40.0f, 26.3f, -0.125f,

			51.9f, 22.9f, -0.125f,
			52.2f, 20.9f, -0.125f,
			54.2f, 10.9f, -0.125f,

			54.2f, 10.9f, -0.125f,
			59.7f, 13.1f, -0.125f,
			59.1f, 15.1f, -0.125f,

			59.1f, 15.1f, -0.125f,
			58.6f, 17.0f, -0.125f,
			47.1f, 39.6f, -0.125f,

			41.5f, 54.9f, -0.125f,
			28.2f, 57.4f, -0.125f,
			30.2f, 56.9f, -0.125f,

			21.4f, 36.5f, -0.125f,
			16.4f, 19.1f, -0.125f,
			16.1f, 17.1f, -0.125f,

			15.3f, 13.1f, -0.125f,
			14.4f, 11.3f, -0.125f,
			21.6f, 14.3f, -0.125f,

			34.9f, 29.7f, -0.125f,
			34.9f, 27.7f, -0.125f,
			37.3f, 8.5f, -0.125f,

			37.3f, 8.5f, -0.125f,
			40.0f, 26.3f, -0.125f,
			40.0f, 28.3f, -0.125f,

			51.9f, 22.9f, -0.125f,
			54.2f, 10.9f, -0.125f,
			59.1f, 15.1f, -0.125f,

			41.5f, 54.9f, -0.125f,
			30.2f, 56.9f, -0.125f,
			32.1f, 56.4f, -0.125f,

			21.4f, 36.5f, -0.125f,
			16.1f, 17.1f, -0.125f,
			15.8f, 15.1f, -0.125f,

			15.8f, 15.1f, -0.125f,
			15.3f, 13.1f, -0.125f,
			21.6f, 14.3f, -0.125f,

			34.9f, 31.8f, -0.125f,
			34.9f, 29.7f, -0.125f,
			37.3f, 8.5f, -0.125f,

			37.3f, 8.5f, -0.125f,
			40.0f, 28.3f, -0.125f,
			39.9f, 30.3f, -0.125f,

			51.4f, 24.9f, -0.125f,
			51.9f, 22.9f, -0.125f,
			59.1f, 15.1f, -0.125f,

			41.0f, 53.0f, -0.125f,
			41.5f, 54.9f, -0.125f,
			32.1f, 56.4f, -0.125f,

			21.4f, 36.5f, -0.125f,
			15.8f, 15.1f, -0.125f,
			21.6f, 14.3f, -0.125f,

			34.8f, 33.8f, -0.125f,
			34.9f, 31.8f, -0.125f,
			37.3f, 8.5f, -0.125f,

			37.3f, 8.5f, -0.125f,
			39.9f, 30.3f, -0.125f,
			39.9f, 32.4f, -0.125f,

			51.0f, 26.9f, -0.125f,
			51.4f, 24.9f, -0.125f,
			59.1f, 15.1f, -0.125f,

			40.8f, 50.9f, -0.125f,
			41.0f, 53.0f, -0.125f,
			32.1f, 56.4f, -0.125f,

			21.4f, 36.5f, -0.125f,
			21.6f, 14.3f, -0.125f,
			22.1f, 20.4f, -0.125f,

			34.7f, 35.8f, -0.125f,
			34.8f, 33.8f, -0.125f,
			37.3f, 8.5f, -0.125f,

			37.3f, 8.5f, -0.125f,
			39.9f, 32.4f, -0.125f,
			39.9f, 34.4f, -0.125f,

			50.5f, 28.9f, -0.125f,
			51.0f, 26.9f, -0.125f,
			59.1f, 15.1f, -0.125f,

			40.7f, 48.9f, -0.125f,
			40.8f, 50.9f, -0.125f,
			32.1f, 56.4f, -0.125f,

			21.4f, 36.5f, -0.125f,
			22.1f, 20.4f, -0.125f,
			22.4f, 22.4f, -0.125f,

			34.7f, 35.8f, -0.125f,
			37.3f, 8.5f, -0.125f,
			39.9f, 34.4f, -0.125f,

			49.8f, 30.8f, -0.125f,
			50.5f, 28.9f, -0.125f,
			59.1f, 15.1f, -0.125f,

			40.7f, 48.9f, -0.125f,
			32.1f, 56.4f, -0.125f,
			33.6f, 55.0f, -0.125f,

			21.4f, 36.5f, -0.125f,
			22.4f, 22.4f, -0.125f,
			22.6f, 24.5f, -0.125f,

			34.4f, 37.9f, -0.125f,
			34.7f, 35.8f, -0.125f,
			39.9f, 34.4f, -0.125f,

			49.1f, 32.7f, -0.125f,
			49.8f, 30.8f, -0.125f,
			59.1f, 15.1f, -0.125f,

			40.6f, 46.8f, -0.125f,
			40.7f, 48.9f, -0.125f,
			33.6f, 55.0f, -0.125f,

			21.4f, 36.5f, -0.125f,
			22.6f, 24.5f, -0.125f,
			23.0f, 26.5f, -0.125f,

			34.4f, 37.9f, -0.125f,
			39.9f, 34.4f, -0.125f,
			40.1f, 36.5f, -0.125f,

			48.2f, 34.5f, -0.125f,
			49.1f, 32.7f, -0.125f,
			59.1f, 15.1f, -0.125f,

			40.6f, 44.8f, -0.125f,
			40.6f, 46.8f, -0.125f,
			33.6f, 55.0f, -0.125f,

			21.4f, 36.5f, -0.125f,
			23.0f, 26.5f, -0.125f,
			23.5f, 28.5f, -0.125f,

			32.7f, 38.6f, -0.125f,
			34.4f, 37.9f, -0.125f,
			40.1f, 36.5f, -0.125f,

			48.2f, 34.5f, -0.125f,
			59.1f, 15.1f, -0.125f,
			47.1f, 39.6f, -0.125f,

			40.7f, 42.8f, -0.125f,
			40.6f, 44.8f, -0.125f,
			33.6f, 55.0f, -0.125f,

			21.4f, 36.5f, -0.125f,
			23.5f, 28.5f, -0.125f,
			24.1f, 30.4f, -0.125f,

			32.7f, 38.6f, -0.125f,
			40.1f, 36.5f, -0.125f,
			41.2f, 37.9f, -0.125f,

			46.9f, 36.1f, -0.125f,
			48.2f, 34.5f, -0.125f,
			47.1f, 39.6f, -0.125f,

			41.3f, 40.8f, -0.125f,
			40.7f, 42.8f, -0.125f,
			33.6f, 55.0f, -0.125f,

			21.4f, 36.5f, -0.125f,
			24.1f, 30.4f, -0.125f,
			25.0f, 32.2f, -0.125f,

			32.7f, 38.6f, -0.125f,
			41.2f, 37.9f, -0.125f,
			43.3f, 37.9f, -0.125f,

			45.2f, 37.3f, -0.125f,
			46.9f, 36.1f, -0.125f,
			47.1f, 39.6f, -0.125f,

			41.3f, 40.8f, -0.125f,
			33.6f, 55.0f, -0.125f,
			34.4f, 53.1f, -0.125f,

			21.4f, 36.5f, -0.125f,
			25.0f, 32.2f, -0.125f,
			26.2f, 33.9f, -0.125f,

			43.3f, 37.9f, -0.125f,
			45.2f, 37.3f, -0.125f,
			47.1f, 39.6f, -0.125f,

			41.3f, 40.8f, -0.125f,
			34.4f, 53.1f, -0.125f,
			34.8f, 51.1f, -0.125f,

			21.4f, 36.5f, -0.125f,
			26.2f, 33.9f, -0.125f,
			27.6f, 35.4f, -0.125f,

			32.7f, 38.6f, -0.125f,
			43.3f, 37.9f, -0.125f,
			47.1f, 39.6f, -0.125f,

			41.3f, 40.8f, -0.125f,
			34.8f, 51.1f, -0.125f,
			34.9f, 49.1f, -0.125f,

			21.4f, 36.5f, -0.125f,
			27.6f, 35.4f, -0.125f,
			29.1f, 36.7f, -0.125f,

			32.7f, 38.6f, -0.125f,
			47.1f, 39.6f, -0.125f,
			43.1f, 40.0f, -0.125f,

			41.3f, 40.8f, -0.125f,
			34.9f, 49.1f, -0.125f,
			35.0f, 47.1f, -0.125f,

			34.6f, 41.2f, -0.125f,
			21.4f, 36.5f, -0.125f,
			29.1f, 36.7f, -0.125f,

			32.7f, 38.6f, -0.125f,
			43.1f, 40.0f, -0.125f,
			41.3f, 40.8f, -0.125f,

			41.3f, 40.8f, -0.125f,
			35.0f, 47.1f, -0.125f,
			35.0f, 45.0f, -0.125f,

			34.6f, 41.2f, -0.125f,
			29.1f, 36.7f, -0.125f,
			30.8f, 37.9f, -0.125f,

			41.3f, 40.8f, -0.125f,
			35.0f, 45.0f, -0.125f,
			35.0f, 43.0f, -0.125f,

			34.6f, 41.2f, -0.125f,
			30.8f, 37.9f, -0.125f,
			32.7f, 38.6f, -0.125f,

			41.3f, 40.8f, -0.125f,
			35.0f, 43.0f, -0.125f,
			34.6f, 41.2f, -0.125f,

			34.6f, 41.2f, -0.125f,
			32.7f, 38.6f, -0.125f,
			41.3f, 40.8, -0.125f,
	};

	const int totalVertices = sizeof(vertices) / sizeof(float) / 3;
	for (int i = 0; i < totalVertices; ++i)
	{
		vertices[i * 3 + 0] = -(vertices[i * 3 + 0] - 37.0f) / 90.0f;
		vertices[i * 3 + 1] = -(vertices[i * 3 + 1] - 32.0f) / 90.0f;
	}

	const int totalIndices = totalVertices + totalVertices + totalVertices;
	unsigned int indices[totalIndices];
	for (int i = 0; i < totalVertices; ++i)
		indices[i] = i;

	float colors[totalIndices * 3];
	for (int i = 0; i < totalIndices; i += 3)
	{
		colors[i + 0] = ((double)rand() / (RAND_MAX));
		colors[i + 1] = ((double)rand() / (RAND_MAX));
		colors[i + 2] = ((double)rand() / (RAND_MAX));
	}


	for (int i = 0; i < totalVertices / 2; i += 3)
	{
		int offset = totalVertices;
		int offset2 = offset + totalVertices / 2;
		int offset3 = offset2 + totalVertices / 2;
		int offset4 = offset3 + totalVertices / 2;

		indices[i + 0 + offset] = i;
		indices[i + 1 + offset] = i + totalVertices / 2;
		indices[i + 2 + offset] = i + totalVertices / 2 + 1;

		indices[i + 0 + offset2] = i + 2;
		indices[i + 1 + offset2] = i + totalVertices / 2 + 2;
		indices[i + 2 + offset2] = i + totalVertices / 2 + 1;

		indices[i + 0 + offset3] = i + 1;
		indices[i + 1 + offset3] = i + totalVertices / 2 + 1;
		indices[i + 2 + offset3] = i + 2;

		indices[i + 0 + offset4] = i + 1;
		indices[i + 1 + offset4] = i + totalVertices / 2 + 1;
		indices[i + 2 + offset4] = i;
	}

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EB;
	glGenBuffers(1, &EB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Input vertex data, different for all executions of this shader.
	// Output data color, will be interpolated for each fragment.
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 vertexColor;\n"
		"out vec3 fragmentColor;\n"
		"uniform mat4 MVP;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = MVP * vec4(aPos, 1.0);\n"
		"   fragmentColor = vertexColor;\n"
		"}\0";

	// declare and define fshader, position in color vector declaration
	// are RGBA from [0,1] simply in and out
	const char* fragmentShaderSource = "#version 330 core\n"
		"in vec3 fragmentColor;\n;"
		"out vec3 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = fragmentColor;\n"
		"}\n\0";

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for vertex shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for fragment shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders : puts together the vertex and the fragment shaders
	// into a "shader program object"
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for shader linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// draw our first triangle: using shader program
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(
		0, // attribute. No particular reason for 0, but
		// must match the layout in the shader.
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(
		1, // attribute. No particular reason for 1, but
		// must match the layout in the shader.
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);

	// Get a handle for our "MVP" uniform
	unsigned int MatrixID = glGetUniformLocation(shaderProgram, "MVP");

	// create transformations    
	// Model matrix : an identity matrix (model will be at the origin)
	// Model = glm::mat4(1.0f);
	Model = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1, 0, 0));

	// View camera matrix
	View = glm::lookAt(
		glm::vec3(0, 0, 1), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Projection matrix : 45 Field of View, 4:3 ratio,
	// display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Or, for an ortho camera :
	//Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f); // In world coordinates

	// Our ModelViewProjection : multiplication of our 3 matrices
	// Remember, matrix multiplication is the other way around
	MVP = Projection * View * Model;

	// delete shaders, we don't need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		//glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //grey rendering
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // Dark blue background

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		// also clear the depth buffer now!
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// seeing as we only have a single VAO there's no need to bind
		// it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_TRIANGLES, totalIndices * 3, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, (totalVertices + totalVertices/2) * 3);
		// glBindVertexArray(0); // no need to unbind it every time

		/* glfw: swap buffers and poll IO events (keys pressed/released,
	   mouse moved etc.)
	   --------------------------------------------------------------*/
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

/* process all input: query GLFW whether relevant keys are pressed/released
this frame and react accordingly
-----------------------------------------------------------------------*/
void changeCamera(glm::vec3 eye, glm::vec3 target = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0),
	float degrees = 30.0f, float x = 1.0f, float y = 0.0f, float z = 0.0f)
{

	Model = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), glm::vec3(x, y, z));
	View = glm::lookAt(eye, target, up);
	MVP = Projection * View * Model;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		changeCamera(glm::vec3(-1, 0, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		changeCamera(glm::vec3(1, 0, 0));
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		changeCamera(glm::vec3(0, 0, 1));
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		changeCamera(glm::vec3(0, 0, -1));
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		changeCamera(glm::vec3(0.0, 1.0f, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		changeCamera(glm::vec3(0.0, -1.0f, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
}

/* glfw: whenever the window size changed (by OS or user resize) this
   callback function executes
   -------------------------------------------------------------------*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that
	// width and height will be significantly larger than specified on
	// retina displays.
	glViewport(0, 0, width, height);
}
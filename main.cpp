#include<filesystem>
#include"Model.h"
#include<stb/stb_image.h>

using namespace std;
namespace fs = filesystem;

const unsigned int width = 800;
const unsigned int height = 800;


int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "IPEngine ver.0.0.0.1", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, height);

	Shader shaderProgram("./shaders/default.vert", "./shaders/default.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	std::string parentDir = (fs::current_path()).string();
	std::string modelPath = "/models/sword/scene.gltf";
	
	Model model((parentDir + modelPath).c_str());

	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	
	unsigned int counter = 0;

	int parr = 0;
	while (!glfwWindowShouldClose(window))
	{
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		if (timeDiff >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "IPEngine ver.0.0.0.1 " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			prevTime = crntTime;
			counter = 0;
		}
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 1000.0f);

		model.Draw(shaderProgram, camera, glm::vec3(0.f, 0.f, 0.f + parr/10));
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		parr++;
	}

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
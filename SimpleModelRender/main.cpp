#include "common.h"

#include "model/my_shader.h"
#include "camera/my_camera.h"
#include "model/my_model.h"

#include <direct.h>
#include <string>

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 450;

MyCamera cam = MyCamera(glm::vec3(.0f, .0f, 3.f), .0f, -90.f);
float camSpeed = 2.5f;
double lastXPos, lastYpos;
bool mouseFistTime = true;
double deltaTime, lastFrameTime;
float MAX_PITCH = 80.f;

struct PointLight
{
	glm::vec3 _position;
	glm::vec3 _color;

	float constant;
	float linear;
	float quadratic;

	PointLight(glm::vec3 pos, glm::vec3 clr, float cons, float linr, float quad) :
		_position(pos), _color(clr), constant(cons), linear(linr), quadratic(quad)
	{}
};
PointLight pointLights[] = {
	PointLight(glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(.5f, .5f, .5f), 1.f, .09f, .032f),
	PointLight(glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(1.f, 0.f, 0.f), 1.f, .09f, .032f),	// color red
	PointLight(glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.f, 1.f, 0.f), 1.f, .09f, .032f),	// color green
	PointLight(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.f, 0.f, 1.f), 1.f, .09f, .032f),	// color blue
};

void framebufferCallback(GLFWwindow*, int, int);
void mouseCallback(GLFWwindow*, double, double);
void inputProcessHandler(GLFWwindow*);

int main()
{
#pragma region init opengl info
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#pragma endregion

#pragma region create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "create window fail!" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
#pragma endregion

#pragma region process func address with glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "glad fail!" << endl;
		glfwTerminate();
		return -1;
	}
#pragma endregion

#pragma region set frame_buffer callback
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetFramebufferSizeCallback(window, framebufferCallback);

	glEnable(GL_DEPTH_TEST);

#pragma endregion
	MyShader myShader("shader_source_files/vertex.glsl", "shader_source_files/fragment.glsl");

	glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, .1f, 100.f);

	glm::mat4 model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down


	MyModel myModel("model_res/nanosuit/nanosuit.obj");

#pragma region render loop
	while (!glfwWindowShouldClose(window))
	{
		double curFrameTime = glfwGetTime();
		deltaTime = curFrameTime - lastFrameTime;
		lastFrameTime = curFrameTime;
		inputProcessHandler(window);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myShader.use();
		myShader.setMat4("projection", projection);
		myShader.setMat4("model", model);
		myShader.setMat4("view", cam.getViewMat4());

		// direction light
		myShader.setVec3("dirLight.color", glm::vec3(1.f, 1.f, 1.f));
		myShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

		// point lights
		int len = sizeof(pointLights) / sizeof(PointLight);
		for (int i = 0; i < len; i++)
		{
			string light = "pointLights[" + to_string(i) + "]";
			myShader.setVec3(light + ".color", pointLights[i]._color);
			myShader.setVec3(light + ".position", pointLights[i]._position);
			myShader.setFloat(light + ".constant", pointLights[i].constant);
			myShader.setFloat(light + ".linear", pointLights[i].linear);
			myShader.setFloat(light + ".quadratic", pointLights[i].quadratic);
		}

		// spot light
		myShader.setVec3("spotLight.color", glm::vec3(.9f, .9f, .9f));
		myShader.setFloat("spotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
		myShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

		myShader.setVec3("viewPos", cam.getCamPosition());

		myShader.setVec3("spotLight.position", cam.getCamPosition());
		myShader.setVec3("spotLight.direction", cam.getCamDirection());

		myModel.Draw(myShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
#pragma endregion

#pragma region clear and stop
	glfwTerminate();
	return 0;
#pragma endregion
}

#pragma region callback implement
void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (mouseFistTime == true)
	{
		lastXPos = xPos;
		lastYpos = yPos;
		mouseFistTime = false;

		return;
	}

	double deltaX = (xPos - lastXPos) * deltaTime; // yaw
	double deltaY = (lastYpos - yPos) * deltaTime; // pitch
	float yaw = cam.getYaw() + deltaX;
	float pitch = cam.getPitch() + deltaY;
	if (pitch > MAX_PITCH)
		pitch = MAX_PITCH;
	else if (pitch < -MAX_PITCH)
		pitch = -MAX_PITCH;

	cam.updateCamDirection(pitch, yaw);
	lastXPos = xPos;
	lastYpos = yPos;

}

void framebufferCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void inputProcessHandler(GLFWwindow* window)
{
	// close application
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}


	// handle moving camera
	float realSpeed = camSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_W, realSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_S, realSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_A, realSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_D, realSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_Q, realSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_E, realSpeed);
	}
}
#pragma endregion
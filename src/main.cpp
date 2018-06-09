#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

#include "Plane.h"
#include "Water.h"

int screen_width = 1280;
int screen_height = 720;
bool direction_lock = true;
glm::vec3 camera_pos;
float angle_phi = 0.0f;
float angle_theta = 45.0f;
double last_mouse_x, last_mouse_y;
bool first_mouse = true;

const float mouse_sensitivity = 0.5f;

glm::vec3 calc_camera_position();
void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main(int argc, char const *argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Create window
	auto window = glfwCreateWindow(screen_width, screen_height, "Water", nullptr, nullptr);

	if (window == nullptr) {
		std::cout << "Failed to Create OpenGL Context" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, screen_width, screen_height);

	glfwSetCursorPosCallback(window, mouse_callback);

	//glEnable(GL_CULL_FACE);
	//glEnable(GL_BLEND);

	// init ////////////////
	camera_pos = calc_camera_position();
	Water water;
	Plane p(50);
	Shader shader("res/shader/water_mesh.vs", "res/shader/water_mesh.fs");
	Texture texTiles("res/texture/tiles.jpg", "text2d");
	texTiles.bind(1);
	//Shader debug_normal_shader("res/shader/water_mesh.vs", "res/shader/debug_normal.gs", "res/shader/debug_normal.fs");
	int timer = 0;
	
	for (int i = 0; i < 20; i++) {
		water.addDrop((1.0f * rand()/ 0x7fff) * 2 - 1, (1.0f * rand() / 0x7fff) * 2 - 1, 0.03, (i & 1) ? 0.01 : -0.01);
	}

	//water.addDrop(0, 0, 0.05, 0.8);
	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//// update
		//if (timer >= 30) {
		//    water.addDrop(0, 0, 0.05, 0.8);
		//    //water.addDrop(0, -0.1, 0.03, 0.05);
		//    //timer = 0;
		//}
		//else timer++;

		water.update();
		water.updateNormals();

		shader.use();
		shader.setMat4("model", glm::mat4(1.0f));
		shader.setMat4("view", glm::lookAt(camera_pos, glm::vec3(0), glm::vec3(0, 1, 0)));
		shader.setMat4("projection", glm::perspective(glm::radians(45.0f), (float)screen_width / screen_height, 0.1f, 100.0f));
		water.getInfoTexture()->bind();
		shader.setInt("water", 0);
		shader.setVec3("light", glm::vec3(2, 2, -1));
		shader.setInt("tiles", 1);
		//shader.setInt("causticTex", 2);
		shader.setVec3("eye", camera_pos);


		//draw
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		p.draw(&shader);
		glDisable(GL_DEPTH_TEST);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//debug_normal_shader.use();
		//debug_normal_shader.setMat4("model", glm::mat4(1.0f));
		//debug_normal_shader.setMat4("view", glm::lookAt(camera_pos, glm::vec3(0), glm::vec3(0, 1, 0)));
		//debug_normal_shader.setMat4("projection", glm::perspective(glm::radians(45.0f), (float)screen_width / screen_height, 0.1f, 100.0f));
		//water.getInfoTexture()->bind();
		//debug_normal_shader.setInt("info_texture", 0);
		//p.draw(&debug_normal_shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void process_input(GLFWwindow* window) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		direction_lock = false;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		direction_lock = true;
		first_mouse = true;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (!direction_lock) {
		if (first_mouse) {
			last_mouse_x = xpos;
			last_mouse_y = ypos;
			first_mouse = false;
		}
		double xoffset = xpos - last_mouse_x;
		double yoffset = last_mouse_y - ypos;
		last_mouse_x = xpos;
		last_mouse_y = ypos;

		angle_phi -= xoffset * mouse_sensitivity;
		angle_theta += yoffset * mouse_sensitivity;

		if (angle_phi >= 360.0f) {
			angle_phi -= 360.0f;
		}
		if (angle_phi < 0.0f) {
			angle_phi += 360.0f;
		}
		angle_theta = angle_theta >= 179.0f ? 179.0f : (angle_theta <= 1.0f ? 1.0f : angle_theta);
		camera_pos = calc_camera_position();
	}
}

glm::vec3 calc_camera_position() {
	float z = sin(glm::radians(angle_theta)) * cos(glm::radians(angle_phi));
	float x = sin(glm::radians(angle_theta)) * sin(glm::radians(angle_phi));
	float y = cos(glm::radians(angle_theta));
	glm::vec3 p(x, y, z);
	return 3.0f * p;
}
#pragma once
#include "Game.h"

#include <GLFW/glfw3.h>
#include <time.h>
#include <chrono>
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	srand(time(NULL));


	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Tetris", nullptr, nullptr);
	Game g(window, WIDTH, HEIGHT);


	auto previousTime = std::chrono::high_resolution_clock::now();

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
		glfwPollEvents();
		g.Update(dt);
		previousTime = currentTime;
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 1;
}
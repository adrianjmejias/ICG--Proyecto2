#pragma once
#include "../headers/GL/glew.h"

#define GLFW_INCLUDE_GLU
#include "../headers/glfw3.h"
#include "Loader.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include <vector>

void updateUserInterface();

void display();

void reshape(GLFWwindow *window, int width, int height);

void keyInput(GLFWwindow *window, int key, int scancode, int action, int mods);

void mouseButton(GLFWwindow* window, int button, int action, int mods);

void cursorPos(GLFWwindow* window, double x, double y);

void charInput(GLFWwindow* window, unsigned int scanChar);

void destroy();

bool initGlfw();

bool initUserInterface();
void externalReshape();
int main(void);
void AddModel(string path);
void AddLight(string path);
string openfilename();
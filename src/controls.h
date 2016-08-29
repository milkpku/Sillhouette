#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include <GLFW/glfw3.h>

void computeMatrixFromInputs();

void setupObject();

void mouseClick(GLFWwindow* window, int button, int action, int mods);

void mouseMove(GLFWwindow* window, double xpos, double ypos);

void help();

void keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods);

void reshape(int w, int h);

#endif

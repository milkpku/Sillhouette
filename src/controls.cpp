#include <GLFW/glfw3.h>
extern GLFWwindow* mainWindow;
extern int win_width, win_height;
extern int shadeFlag;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "viewer/Arcball.h"
using namespace MeshLib;

/* arcball object */
CArcball arcball;

/* rotation quaternion and translation vector for the object */
CQrot       ObjRot(0, 0, 1, 0);
CPoint      ObjTrans(0, 0, 0);

/* inner variables */
double startx, starty;
int gButton; 

void computeMatrixFromInputs()
{
}

/*! setup the object, transform from the world to the object coordinate system */

void setupObject(void)
{
    glm::dmat4 rot;

	glTranslated(ObjTrans[0], ObjTrans[1], ObjTrans[2]);
	ObjRot.convert(glm::value_ptr(rot));
	glMultMatrixd(glm::value_ptr(rot));
}

/*! mouse click call back function */
void  mouseClick(GLFWwindow* window, int button, int action, int mods) {
	/* set up an arcball around the Eye's center
	switch y coordinates to right handed system  */
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		gButton = GLFW_MOUSE_BUTTON_LEFT;
		arcball = CArcball(win_width, win_height, xpos - win_width / 2, win_height - ypos - win_height / 2);
	}

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		startx = xpos;
		starty = ypos;
		gButton = GLFW_MOUSE_BUTTON_MIDDLE;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		startx = xpos;
		starty = ypos;
		gButton = GLFW_MOUSE_BUTTON_RIGHT;
	}
	return;
}

/*! mouse motion call back function */
void mouseMove(GLFWwindow* window, double xpos, double ypos)
{
	CPoint trans;
	CQrot  rot;

	/* rotation, call arcball */
	if (gButton == GLFW_MOUSE_BUTTON_LEFT)
	{
		rot = arcball.update(xpos - win_width / 2, win_height - ypos - win_height / 2);
		ObjRot = rot * ObjRot;
	}

	/*xy translation */
	if (gButton == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		double scale = 10. / win_height;
		trans = CPoint(scale*(xpos - startx), scale*(starty - ypos), 0);
		startx = xpos;
		starty = ypos;
		ObjTrans = ObjTrans + trans;
	}

	/* zoom in and out */
	if (gButton == GLFW_MOUSE_BUTTON_RIGHT) {
		double scale = 10. / win_height;
		trans = CPoint(0, 0, scale*(starty - ypos));
		startx = xpos;
		starty = ypos;
		ObjTrans = ObjTrans + trans;
	}

}

/*! helper function to remind the user about commands, hot keys */
void help()
{
	printf("w  -  Wireframe Display\n");
	printf("f  -  Flat Shading \n");
	printf("s  -  Smooth Shading\n");
	printf("?  -  Help Information\n");
	printf("q - quit\n");
}

/*! Keyboard call back function */
void keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS){

	switch (key)
	{
	case GLFW_KEY_F:
		//Flat Shading
		glPolygonMode(GL_FRONT, GL_FILL);
		shadeFlag = 0;
		break;
	case GLFW_KEY_S:
		//Smooth Shading
		glPolygonMode(GL_FRONT, GL_FILL);
		shadeFlag = 1;
		break;
	case GLFW_KEY_W:
		//Wireframe mode
		glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case GLFW_KEY_UNKNOWN:
		help();
		break;
	case GLFW_KEY_Q:
	case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(mainWindow, GLFW_TRUE);
		break;
	}
    }
}

/*! Called when a "resize" event is received by the window. */
void reshape(int w, int h)
{
	float ar;
	//std::cout << "w:" << w << "\th:" << h << std::endl;
	win_width = w;
	win_height = h;

	ar = (float)(w) / h;
	glViewport(0, 0, w, h);               /* Set Viewport */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// magic imageing commands

    /* gluPerspective(40.0,
		ar, 
		1.0,
		100.0);
    */
	glMatrixMode(GL_MODELVIEW);

}



#include <GLFW/glfw3.h>
extern GLFWwindow* mainWindow;
extern int win_width, win_height;
extern int shadeFlag;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "viewer/Arcball.h"
using namespace MeshLib;

/* arcball object */
CArcball arcball;

/* rotation quaternion and translation vector for the object */
CQrot       ObjRot(0, 0, 1, 0);
glm::vec3   camera(0, 0, 1);

/* inner variables */
double startx, starty;
int gButton; 
int gState; 

/* inner mantained */
glm::dmat4 Model;
glm::mat4 View;
glm::mat4 Projection;

/* update at each main loop */
void computeMatrixFromInputs()
{
    ObjRot.convert(glm::value_ptr(Model));
    View = glm::lookAt(
                        camera,                             /* Camera location */
                        glm::vec3(camera.x, camera.y, 0), /* and looks at this point */
                        glm::vec3(0, 1, 0)                    /* Head is up */
            );
    Projection = glm::perspective(45.0f, win_width/(float) win_height, 0.1f, 100.0f);
}

glm::vec3 getCamera(){
    return camera;
}

glm::mat4 getMVP(){
	return Projection * View * glm::mat4(Model);
}

glm::mat4 getView(){
    return View;
}

glm::mat4 getModel(){
    return Model;
}

glm::mat4 getProjection(){
    return Projection;
}


/*! mouse click call back function */
void  mouseClick(GLFWwindow* window, int button, int action, int mods) {
	/* set up an arcball around the Eye's center
	switch y coordinates to right handed system  */
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    gState = action;
    
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
    glm::vec3 trans;
	CQrot  rot;

    if (gState!=GLFW_PRESS)
    {
        return;
    }

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
		trans = glm::vec3(scale*(xpos - startx), scale*(starty - ypos), 0);
		startx = xpos;
		starty = ypos;
		camera = camera + trans;
	}

	/* zoom in and out */
	if (gButton == GLFW_MOUSE_BUTTON_RIGHT) {
		double scale = 10. / win_height;
		trans = glm::vec3(0, 0, scale*(starty - ypos));
		startx = xpos;
		starty = ypos;
		camera = camera + trans;
	}

    printf("%f %f %f\n", camera.x, camera.y, camera.z);

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
    if (action != GLFW_PRESS){
        return;
    }

    printf("%d\n", key);

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
	case GLFW_KEY_H:
	case GLFW_KEY_UNKNOWN:
		help();
		break;
	case GLFW_KEY_Q:
	case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(mainWindow, GLFW_TRUE);
		break;
	}
}

/*! Called when a "resize" event is received by the window. */
void reshape(int w, int h)
{
	win_width = w;
	win_height = h;
}



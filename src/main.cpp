#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "viewer/Arcball.h"                           /*  Arc Ball  Interface         */

#include "MyMesh.h"
#include "controls.h"
#include "shaderLoader.h"

using namespace MeshLib;

/* window width and height */
GLFWwindow* mainWindow;
int win_width = 600;
int win_height = 400;
int shadeFlag = 0;

glm::vec4 lightColor(0.8, 0.8, 0.8, 1.0);
glm::vec4 globalAmb(0.1, 0.1, 0.1, 1.0);
glm::vec3 lightPosition(0.0, 0.0, 1.0);

/* global mesh */
CMyMesh mesh;

/* GL render */
enum {verVBO, norVBO, faceVBO, cordVBO, num_VBO};
GLuint VAO;
GLuint VBO[num_VBO];
GLuint ProgramID;
GLuint eyePosID;
glm::vec3 eyePos(0, 0, 10);
std::vector<glm::vec3> vertex;
std::vector<glm::vec3> norm;
std::vector<size_t> faceID;

bool str_replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}


/*! the eye is always fixed at world z = +5 */
void setupEye(void) {
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
}

/*! setup light */
void setupLight()
{
	GLfloat lightOnePosition[4] = { 0, 0, 1, 0 };
	glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);
}

/*! draw axis */
void draw_axis()
{
	glLineWidth(2.0);
	//x axis
	glColor3f(1.0, 0.0, 0.0);	//red
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(1, 0, 0);
	glEnd();

	//y axis
	glColor3f(0.0, 1.0, 0);		//green
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 1, 0);
	glEnd();

	//z axis
	glColor3f(0.0, 0.0, 1.0);	//blue
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 1);
	glEnd();

	glLineWidth(1.0);
}

/*! draw mesh */
void draw_mesh()
{
	glBegin(GL_TRIANGLES);
	for (CMyMesh::_MeshFaceIterator fiter(&mesh); !fiter.end(); ++fiter)
	{
		CMyFace * pf = *fiter;
		for (CMyMesh::_FaceVertexIterator fviter(pf); !fviter.end(); ++fviter)
		{
			CMyVertex * v = *fviter;
			CPoint pt = v->point();
			CPoint n;
			switch (shadeFlag)
			{
			case 0:
				n = pf->normal();
				break;
			case 1:
				n = v->normal();
				break;
			}
			glNormal3d(n[0], n[1], n[2]);
			glColor3f(1.0, 1.0, 1.0);
			glVertex3d(pt[0], pt[1], pt[2]);
		}
	}
	glEnd();
}

/*! display call back function
*/
void display()
{
	setupLight();
	/* transform from the eye coordinate system to the world system */
	setupEye();
	glPushMatrix();
	/* transform from the world to the ojbect coordinate system */
	setupObject();
	/* draw the mesh */
	draw_mesh();

	//mesh.display_vertexes();

	//mesh.display_faces();

	draw_axis();
	glPopMatrix();
}

void buff_model()
{ 
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(num_VBO, VBO);

    /* prepare vertex & normal data */
    size_t N = mesh.numVertices();
    vertex.assign(N, glm::vec3(0,0,0));
    norm.assign(N, glm::vec3(0,0,0));
    size_t count = 0;
    for (CMyMesh::_MeshVertexIterator viter(&mesh); !viter.end(); ++viter)
    {
        CMyVertex* pV = *viter;
        pV->id() = count;
        CPoint pos = pV->point();
        CPoint nor = pV->normal();
        vertex[count] = glm::vec3(pos[0], pos[1], pos[2]); 
        norm[count] = glm::vec3(nor[0], nor[1], nor[2]);
        count ++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO[verVBO]);
    glBufferData(GL_ARRAY_BUFFER, vertex.size()*sizeof(glm::vec3), &vertex[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[norVBO]);
    glBufferData(GL_ARRAY_BUFFER, norm.size()*sizeof(glm::vec3), &norm[0], GL_STATIC_DRAW);

    /* prepare faceID data */
    size_t M = mesh.numFaces();
    faceID.assign(M*3, 0);
    count = 0;
    for (CMyMesh::_MeshFaceIterator fiter(&mesh); !fiter.end(); ++fiter)
    {
        CMyFace* pF = *fiter;
        CMyHalfEdge* pH = (CMyHalfEdge*)pF->halfedge();
        faceID[3*count] = pH->source()->id();
        faceID[3*count + 1] = pH->target()->id();
        faceID[3*count + 2] = pH->he_next()->target()->id();
        count ++;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[faceVBO]);
    glBufferData(GL_ARRAY_BUFFER, faceID.size(), &faceID[0], GL_STATIC_DRAW);

}

void prepareProgram()
{
    /* prepare Program */
    ProgramID = shaderLoader("src/vertexShader.glsl", "fragmentShader.glsl");
    eyePosID = glGetUniformLocation(ProgramID, "eyePos");
}

/*! setup GL states */
void setupGLstate() {

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.35, 0.53, 0.70, 0);
	glShadeModel(GL_SMOOTH);
}


int init_openGL(int argc, char * argv[])
{
    if (!glfwInit())
    {
        printf("Fail to initialize GLFW.\n");
        return -1;
    }

    /* create a window and its OpenGL context */
    mainWindow = glfwCreateWindow(win_width, win_height, "Mesh Viewer", NULL, NULL);
    if (!mainWindow)
    {
        printf("Fail to create Window.\n");
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(mainWindow, keyBoard);
    glfwSetMouseButtonCallback(mainWindow, mouseClick);
    glfwSetCursorPosCallback(mainWindow, mouseMove);

    /* Make the window's context current */
    glfwMakeContextCurrent(mainWindow);

	setupGLstate();

    while (!glfwWindowShouldClose(mainWindow))
    {
        /* Render */
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        display();

        /* Swap front and back buffers*/
        glfwSwapBuffers(mainWindow);

        /* Poll for and process event */
        glfwPollEvents();

    }


}

/*! main function for viewer
*/
int main(int argc, char * argv[])
{
	std::cout << std::setprecision(12);

	if (argc < 2)
	{
		std::cout << "Usage: input.m" << std::endl;
		return -1;
	}

	std::string mesh_name(argv[1]);
	if (strutil::endsWith(mesh_name, ".obj"))
	{
		mesh.read_obj(mesh_name.c_str());
	}
	if (strutil::endsWith(mesh_name, ".m"))
	{
		mesh.read_m(mesh_name.c_str());
	}
	if (strutil::endsWith(mesh_name, ".off"))
	{
		mesh.read_off(mesh_name.c_str());
	}

	normalize_mesh(&mesh);
	compute_normal(&mesh);

	mesh.check_gauss_bonnet_theory();

	init_openGL(argc, argv);

	return 0;
}

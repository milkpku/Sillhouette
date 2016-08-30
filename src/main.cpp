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
glm::vec3 eyePos(0, 0, 10);
std::vector<glm::vec3> vertex;
std::vector<glm::vec3> norm;
std::vector<unsigned int> faceID;
/* shader data */
GLuint ProgramID;
GLuint eyePosID, MVPID;

bool str_replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
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
    glUseProgram(ProgramID);
    glBindVertexArray(VAO);

    //glUniform3fv(eyePosID, 3, &eyePos[0]);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[verVBO]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[norVBO]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[faceVBO]);
    glDrawElements(GL_TRIANGLES, faceID.size(), GL_UNSIGNED_INT, (void*)0);
}

void buff_model()
{ 
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(num_VBO, VBO);

    /* prepare vertex & normal data */
    unsigned int N = mesh.numVertices();
    vertex.assign(N, glm::vec3(0,0,0));
    norm.assign(N, glm::vec3(0,0,0));
    unsigned int count = 0;
    for (CMyMesh::_MeshVertexIterator viter(&mesh); !viter.end(); ++viter)
    {
        CMyVertex* pV = *viter;
        pV->draw_id() = count;
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
        faceID[3*count] = ((CMyVertex*)pH->source())->draw_id();
        faceID[3*count + 1] = ((CMyVertex*)pH->target())->draw_id();
        faceID[3*count + 2] = ((CMyVertex*)pH->he_next()->target())->draw_id();
        count ++;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[faceVBO]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceID.size()*sizeof(unsigned int), &faceID[0], GL_STATIC_DRAW);

}

void prepareProgram()
{
    /* prepare Program */
    ProgramID = shaderLoader("src/vertexShader.glsl", "src/fragmentShader.glsl");
    //eyePosID = glGetUniformLocation(ProgramID, "eyePos");
}

/*! setup GL states */
void setupGLstate() {
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.35, 0.53, 0.70, 0);

    buff_model();
    prepareProgram();
}


void init_openGL(int argc, char * argv[])
{
    if (!glfwInit())
    {
        printf("Fail to initialize GLFW.\n");
        return;
    }

    /* create a window and its OpenGL context */
    mainWindow = glfwCreateWindow(win_width, win_height, "Mesh Viewer", NULL, NULL);
    if (!mainWindow)
    {
        printf("Fail to create Window.\n");
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(mainWindow);

    if (glewInit() != GLEW_OK)
    {
        printf("Fail to initialize GLEW.\n");
        glfwTerminate();
        return;
    }

    glfwSetKeyCallback(mainWindow, keyBoard);
    glfwSetMouseButtonCallback(mainWindow, mouseClick);
    glfwSetCursorPosCallback(mainWindow, mouseMove);

	setupGLstate();

    while (!glfwWindowShouldClose(mainWindow))
    {
        /* Render */
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        draw_mesh();

        draw_axis();

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

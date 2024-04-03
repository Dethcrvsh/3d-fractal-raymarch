// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to cocam_pos.y your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"
#include <GL/glext.h>
// uses framework OpenGL
// uses framework Cocoa

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

GLuint program;

// Globals
// Data would normally be read from files
GLfloat vertices[] =
{
    1.0, 1.0, 0.0,
    1.0, -1.0, 0.0,
    -1.0, -1.0, 0.0,

    1.0, 1.0, 0.0,
    -1.0, -1.0, 0.0,
    -1.0, 1.0, 0.0
};

// vertex array object
unsigned int vertex_array_obj_id;

void init(void)
{
    // vertex buffer object, used for uploading the geometry
    unsigned int vertex_buffer_obj_id;

    dumpInfo();

    // GL inits
    glClearColor(0.2,0.2,0.5,0);
    glDisable(GL_DEPTH_TEST);
    printError("GL inits");

    // Load and compile shader
    program = loadShaders("shader.vert", "shader.frag");
    printError("init shader");

    // Allocate and activate Vertex Array Object
    glGenVertexArrays(1, &vertex_array_obj_id);
    glBindVertexArray(vertex_array_obj_id);
    // Allocate Vertex Buffer Objects
    glGenBuffers(1, &vertex_buffer_obj_id);

    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

    // Send the screen size to the shader
    glUniform2i(glGetUniformLocation(program, "in_ScreenSize"), SCREEN_WIDTH, SCREEN_HEIGHT);

    printError("init arrays");
}

const float MOVE_SPEED = 0.01;
const float MOUSE_SENSATIVITY = 1000;
vec3 cam_pos = vec3(0, 0, 5);
vec2 cam_angel;

float previous_time;


void mouse_moved(int mx, int my) {
  cam_angel.x -= (SCREEN_WIDTH/2 - mx) / MOUSE_SENSATIVITY;
  cam_angel.y -= (SCREEN_HEIGHT/2 - my) / MOUSE_SENSATIVITY;
  cam_angel.y = std::max((float)-M_PI/2, std::min(cam_angel.y, (float)M_PI/2));
  glutWarpPointer(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
}

void display(void)
{

    // Handel time and delta time
    GLfloat time = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    float delta = time - previous_time;
    previous_time = time;

	// Handel movement relative to viewing direction
    if (glutKeyIsDown('w')) {
        cam_pos.x -= delta * sin(-cam_angel.x) * cos(-cam_angel.y) * MOVE_SPEED;
        cam_pos.y -= delta * sin(cam_angel.y) * MOVE_SPEED;
        cam_pos.z -= delta * cos(-cam_angel.x) * cos(-cam_angel.y) * MOVE_SPEED;
    }
    if (glutKeyIsDown('a')) {
        cam_pos.x -= delta * cos(cam_angel.x) * MOVE_SPEED;
        cam_pos.z -= delta * sin(cam_angel.x) * MOVE_SPEED;
    }
    if (glutKeyIsDown('s')) {
        cam_pos.x += delta * sin(-cam_angel.x) * cos(-cam_angel.y) * MOVE_SPEED;
        cam_pos.y += delta * sin(cam_angel.y) * MOVE_SPEED;
        cam_pos.z += delta * cos(-cam_angel.x) * cos(-cam_angel.y) * MOVE_SPEED;
    }
    if (glutKeyIsDown('d')) {
        cam_pos.x += delta * cos(cam_angel.x) * MOVE_SPEED;
        cam_pos.z += delta * sin(cam_angel.x) * MOVE_SPEED;
    }

	// Get camera rotation as matrix
    GLfloat rotMatrix_x[] = {1.0f, 0.0f, 0.0f, 0.0f,
						     0.0f, cos(-cam_angel.y), -sin(-cam_angel.y), 0.0f, 
							 0.0f, sin(-cam_angel.y), cos(-cam_angel.y), 0.0f,
						     0.0f, 0.0f, 0.0f, 1.0f};

    GLfloat rotMatrix_y[] = {cos(-cam_angel.x),  0.0f, sin(-cam_angel.x), 0.0f, 
						     0.0f, 1.0f, 0.0f, 0.0f,
                             -sin(-cam_angel.x), 0.0f, cos(-cam_angel.x), 0.0f, 
						     0.0f, 0.0f, 0.0f, 1.0f};

	time = time/1000;
    GLfloat rotMatrix1[] = {cos(-time), 0.0f, sin(-time), 0.0f, 
						     0.0f, 1.0f, 0.0f, 0.0f,
                             -sin(-time), 0.0f, cos(-time), 0.0f, 
						     0.0f, 0.0f, 0.0f, 1.0f};

	time = time/1.4;
    GLfloat rotMatrix2[] = {1.0f, 0.0f, 0.0f, 0.0f,
						     0.0f, cos(-time), -sin(-time), 0.0f, 
							 0.0f, sin(-time), cos(-time), 0.0f,
						     0.0f, 0.0f, 0.0f, 1.0f};

	// Send camera data to shader
    glUniformMatrix4fv(glGetUniformLocation(program, "in_CamRotX"), 1, GL_TRUE,
                     rotMatrix_x);

    glUniformMatrix4fv(glGetUniformLocation(program, "in_CamRotY"), 1, GL_TRUE,
                     rotMatrix_y);

    glUniformMatrix4fv(glGetUniformLocation(program, "in_RotTest1"), 1, GL_TRUE,
                     rotMatrix1);

    glUniformMatrix4fv(glGetUniformLocation(program, "in_RotTest2"), 1, GL_TRUE,
                     rotMatrix2);
    
	glUniform3f(glGetUniformLocation(program, "in_CamPosition"),
					cam_pos.x, cam_pos.y, cam_pos.z);


    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vertex_array_obj_id);	// Select VAO
    glDrawArrays(GL_TRIANGLES, 0, 6);	// draw object

    // End of upload of geometry

    printError("display");

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow ("Raymarch fractal");
    glutDisplayFunc(display); 
	glutRepeatingTimer(2);
    glutPassiveMotionFunc(mouse_moved);
  	glutHideCursor();
    init ();
    glutMainLoop();
    return 0;
}

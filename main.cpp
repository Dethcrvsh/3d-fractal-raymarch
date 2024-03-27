// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"
// uses framework OpenGL
// uses framework Cocoa

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
    // Reference to shader program
    GLuint program;

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

    // End of upload of geometry

    printError("init arrays");
}

void display(void)
{
    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vertex_array_obj_id);	// Select VAO
    glDrawArrays(GL_TRIANGLES, 0, 6);	// draw object

    printError("display");

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(600, 600);
    glutCreateWindow ("GL3 white triangle example");
    glutDisplayFunc(display); 
    init ();
    glutMainLoop();
    return 0;
}

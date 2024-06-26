
#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"
#include "shader.h"
#include <GL/glext.h>
#include <math.h>
#include "menu.h"
#include "SimpleGUI.h"
// uses framework OpenGL
// uses framework Cocoa

const int SCREEN_WIDTH = 1200;
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

ShaderInstance instance;

float rotation1 = 0;
float rotation2 = 0;
vec3 offset = vec3(0.0, 0.0, 0.0);

unsigned key_timer {};
int const KEY_DELAY = 500;

void init_arrays() {
    // vertex buffer object, used for uploading the geometry
    unsigned int vertex_buffer_obj_id;

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

void init(void)
{

    dumpInfo();

    // GL inits
    glClearColor(0.2,0.2,0.5,0);
    glDisable(GL_DEPTH_TEST);
    printError("GL inits");

    instance = load_shaders("shaders/shader.vert", "shaders/shader.frag");
    program = instance.program;
    init_arrays();

    printError("init shader");

    Menu::init();
}

const float MOVE_SPEED = 0.0015;
const float MOUSE_SENSITIVITY = 10000;

struct Camera
{
    vec3 pos {};
    vec2 angle {};
} camera;

float previous_time;
bool in_menu = false;


void mouse_moved(int mx, int my)
{
    if(not in_menu){
        camera.angle.x -= (SCREEN_WIDTH/2.0 - mx) / MOUSE_SENSITIVITY;
        camera.angle.y -= (SCREEN_HEIGHT/2.0 - my) / MOUSE_SENSITIVITY;
        camera.angle.y = std::max((float)-M_PI/2, std::min(camera.angle.y, (float)M_PI/2));
        glutWarpPointer(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        glutHideCursor();
    }
    else{
        glutShowCursor();
    }
}

void do_movement(float const delta)
{
	//Handle movement relative to viewing direction
    if (glutKeyIsDown('w')) {
        camera.pos.x -= delta * sin(-camera.angle.x) * cos(-camera.angle.y) * MOVE_SPEED;
        camera.pos.y -= delta * sin(camera.angle.y) * MOVE_SPEED;
        camera.pos.z -= delta * cos(-camera.angle.x) * cos(-camera.angle.y) * MOVE_SPEED;
    }
    if (glutKeyIsDown('a')) {
        camera.pos.x -= delta * cos(camera.angle.x) * MOVE_SPEED;
        camera.pos.z -= delta * sin(camera.angle.x) * MOVE_SPEED;
    }
    if (glutKeyIsDown('s')) {
        camera.pos.x += delta * sin(-camera.angle.x) * cos(-camera.angle.y) * MOVE_SPEED;
        camera.pos.y += delta * sin(camera.angle.y) * MOVE_SPEED;
        camera.pos.z += delta * cos(-camera.angle.x) * cos(-camera.angle.y) * MOVE_SPEED;
    }
    if (glutKeyIsDown('d')) {
        camera.pos.x += delta * cos(camera.angle.x) * MOVE_SPEED;
        camera.pos.z += delta * sin(camera.angle.x) * MOVE_SPEED;
    }
    if (glutKeyIsDown(GLUT_KEY_ESC) && key_timer > KEY_DELAY){
        in_menu = not in_menu;
        key_timer = 0;
    }
}

void display(void)
{
    // Handle time and delta time
    GLfloat time = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    float const delta = time - previous_time;
    previous_time = time;
    key_timer += delta;

    // Upload the menu parameters to the shaders
    Menu::Parameters *params {Menu::get_parameters()};

    if (params->has_changed) {
        insert_shader(instance, params->get_variables(), params->get_code());
        init_arrays();
    }

    params->upload(program);

    do_movement(delta);

	// Get camera rotation as matrix
    GLfloat rotMatrix_x[] = {1.0f, 0.0f, 0.0f, 0.0f,
						     0.0f, cos(-camera.angle.y), -sin(-camera.angle.y), 0.0f, 
							 0.0f, sin(-camera.angle.y), cos(-camera.angle.y), 0.0f,
						     0.0f, 0.0f, 0.0f, 1.0f};

    GLfloat rotMatrix_y[] = {cos(-camera.angle.x),  0.0f, sin(-camera.angle.x), 0.0f, 
						     0.0f, 1.0f, 0.0f, 0.0f,
                             -sin(-camera.angle.x), 0.0f, cos(-camera.angle.x), 0.0f, 
						     0.0f, 0.0f, 0.0f, 1.0f};

    // Send camera data to shader
    glUniformMatrix4fv(glGetUniformLocation(program, "in_CamRotX"), 1, GL_TRUE, rotMatrix_x);

    glUniformMatrix4fv(glGetUniformLocation(program, "in_CamRotY"), 1, GL_TRUE, rotMatrix_y);

	glUniform3f(glGetUniformLocation(program, "in_CamPosition"),
        camera.pos.x, camera.pos.y, camera.pos.z);
    
    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vertex_array_obj_id);	// Select VAO
    glDrawArrays(GL_TRIANGLES, 0, 6);	// draw object

    // End of upload of geometry

    printError("display");

	sgDraw();

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
	if (button == 0) sgMouse(state, x, y);
}

void drag(int x, int y)
{
	sgMouseDrag(x, y);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow ("Raymarch fractal");
    glutDisplayFunc(display); 
    glutRepeatingTimer(64);
    glutPassiveMotionFunc(mouse_moved);
    glutMouseFunc(mouse);
    glutMotionFunc(drag);
    init();
    glutMainLoop();
}

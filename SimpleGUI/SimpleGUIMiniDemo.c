// SimpleGUI mini demo, with just a slider to give you the easiest
// possible introduction.

#include "MicroGlut.h"
#include "SimpleGUI.h"
#include <string.h>
//uses framework Cocoa
//uses framework OpenGL

float testf = 5;

void init()
{
	sgCreateStaticString(40, 40, "Minimal demo of SimpleGUI");
	
	// A slider and a float display
	sgCreateStaticString(40, 80, "Slider and float display");
	sgCreateSlider(40, 100, 200, &testf, 5, 10);
	sgCreateDisplayFloat(40, 120, "Value: ", &testf);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sgDraw();
	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
	sgMouse(state, x, y);
	glutPostRedisplay();
}

void drag(int x, int y)
{
	sgMouseDrag(x, y);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(40,50);
	glutInitWindowSize(330,150);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SimpleGUI mini demo");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	
	init();

	glutMainLoop();
}

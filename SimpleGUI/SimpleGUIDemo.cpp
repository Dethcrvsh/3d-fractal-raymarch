// SimpleGUI demo, demonstrating most SimpleGUI controls.

#include "MicroGlut.h"
#include "SimpleGUI.h"
#include <string.h>
//uses framework Cocoa
//uses framework OpenGL

float testr, testg, testb, testf = 5;
int testboolean;
int testradio;
char testString[1024] = "Type here";

float myvec3[3];

void HitButton()
{
	testr = 0;
	testg = 0;
	testb = 0;
}

void SetToRed()
{
	testr = 1;
	testg = 0;
	testb = 0;
}

void SetToGreen()
{
	testr = 0;
	testg = 1;
	testb = 0;
}

void SetToBlue()
{
	testr = 0;
	testg = 0;
	testb = 1;
}

void init()
{
	sgCreateStaticString(100, 40, "This is a demo of SimpleGUI");
	
	// A slider and a float display
	sgCreateStaticString(40, 80, "Slider and float display");
	sgCreateSlider(40, 100, 100, &testf, 5, 10);
	sgCreateDisplayFloat(40, 120, "Value: ", &testf);

	sgCreateStaticString(40, 160, "Checkbox controlling a boolean variable");
	sgCreateCheckBox(40, 180, "Check me", &testboolean);
	sgCreateDisplayInt(160, 184, "Result: ", &testboolean);

	sgCreateStaticString(40, 220, "Slider color group");
	sgCreateSliderColorGroup(20, 240, 150, &testr, &testg, &testb);

	// Radio group: Connect to same variable!
	sgCreateStaticString(280, 270, "Radio buttons:");
	sgCreateRadio(280, 300, "One", &testradio, 1);
	sgCreateRadio(280, 320, "Two", &testradio, 2);
	sgCreateRadio(280, 340, "Three", &testradio, 3);
	sgCreateDisplayInt(280, 380, "Result: ", &testradio);

	sgCreateStaticString(40, 400, "Single line editor:");
	sgCreateDynamicString(40, 420, testString);

//	"Military theme"
//	sgSetSliderFillColor(0.3, 0.5, 1.0);
//	sgSetFrameColor(0.2, 0.0, 0.5);
//	sgSetBackgroundBorder(15);
//	sgSetBackgroundColor(0.7, 1.0, 0.7, 0.3);
//	sgSetTextColor(0.8, 1.0, 1.0);
	
	sgCreateSmallColorPalette(40, 320, &testr, &testg, &testb);
	sgCreateStaticString(150, 330, "Palette");
	sgCreateButton(150, 350, "RESET", HitButton);

	sgCreateMenu(40, 300, "Colors menu");
	sgCreateMenuItem("Set to red", SetToRed);
	sgCreateMenuItem("Set to green", SetToGreen);
	sgCreateMenuItem("Set to blue", SetToBlue);

	sgSetPosition(70, 20);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sgDraw();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	// The "single line editor", edit the string
	int l = strlen(testString);
	if ((key == 127) && (l > 0))
	{
		testString[l-1] = 0;
	}
	else
	if (key >= 32 && l < 128)
	{
		testString[l] = key;
		testString[l+1] = 0;
	}
	
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (button == 0) sgMouse(state, x, y);
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
	glutInitWindowSize(600,550);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SimpleGUI demo");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	
	init();

	glutMainLoop();
}

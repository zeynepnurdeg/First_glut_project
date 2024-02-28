#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 500
#define TIMER_PERIOD   10   // Period for the timer
#define TIMER_ON        1   // 0:Disable timer, 1:Enable timer

#define D2R 0.0174532
#define PI 3.1415
#define SHIP 0

bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // Current Window width and height

bool activeTimer = true;
int mode = SHIP; // Initial shape is BALLOON
int r = 180, g = 172, b = 50; // Initial color
char shapes[1][20] = { "BOAT" };

#define INITIAL_xB 560
int xB = -490, yB = -30; // Boat's initial position
int radiusB = 80; // Boat's initial radius

// To draw a filled circle, centered at (x,y) with radius r
void circle(int x, int y, int r) {
	float angle;

	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++) {
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r) {
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++) {
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char* string, void* font) {
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, string[i]);
}

void vprint(int x, int y, void* font, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, str[i]);
}

void vprint2(int x, int y, float size, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	glPopMatrix();
}

void drawSky() {
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex2f(-201, 150);
	glVertex2f(-170, 120);
	glVertex2f(-205, 85);
	glVertex2f(-260, 100);
	glVertex2f(-270, 130);
	glVertex2f(-240, 155);
	glEnd();

	glColor3f(1, 1, 1);
	circle(-201, 150, 35);

	glColor3f(1, 1, 1);
	circle(-170, 120, 26);

	glColor3f(1, 1, 1);
	circle(-205, 85, 33);

	glColor3f(1, 1, 1);
	circle(-260, 100, 29);

	glColor3f(1, 1, 1);
	circle(-270, 130, 29);

	glColor3f(1, 1, 1);
	circle(-240, 150, 19);

	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex2f(201, 150);
	glVertex2f(170, 120);
	glVertex2f(205, 85);
	glVertex2f(260, 100);
	glVertex2f(270, 130);
	glVertex2f(240, 165);
	glEnd();

	glColor3f(1, 1, 1);
	circle(201, 170, 29);

	glColor3f(1, 1, 1);
	circle(170, 140, 29);

	glColor3f(1, 1, 1);
	circle(205, 105, 29);

	glColor3f(1, 1, 1);
	circle(260, 120, 29);

	glColor3f(1, 1, 1);
	circle(270, 150, 29);

	glColor3f(1, 1, 1);
	circle(240, 170, 19);

	//sun
	glColor3f(0.9, 0.9, 0);
	circle(-470, 240, 35);
}

void drawBoat() {
	glBegin(GL_TRIANGLES);
	glColor3ub(r, g, b);
	glVertex2f(xB, yB - (radiusB * 0.66));
	glVertex2f(xB - (radiusB / 2), yB - (radiusB * 1.5) - 5);
	glVertex2f(xB + (radiusB / 2), yB - (radiusB * 1.5) - 5);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 0.6);
	glVertex2f(xB, (yB - (radiusB * 0.66)));
	glVertex2f(xB - (radiusB / 2), (yB - (radiusB * 1.5)) + 25);
	glVertex2f(xB + (radiusB / 2), (yB - (radiusB * 1.5)) + 25);
	glEnd();

	glLineWidth(3);
	glColor3f(1, 1, 0); // Yellow
	glBegin(GL_LINES);
	glVertex2f(xB, yB - radiusB + 30);
	glVertex2f(xB, yB - radiusB - 18);
	glEnd();
	glLineWidth(1);

	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 1);
	glVertex2f(xB, (yB - (radiusB * 0.66)) + 20);
	glVertex2f(xB - (radiusB / 2) - 10, (yB - (radiusB * 1.5)) + 30);
	glVertex2f(xB + (radiusB / 2) - 5, (yB - (radiusB * 1.5)) + 30);
	glEnd();
}

void displaySea() {
	glColor3f(0, 0, 0.6);
	glPointSize(5);
	glLineWidth(3);
	glBegin(GL_TRIANGLES);
	glVertex2f(WINDOW_HEIGHT, -49);
	glVertex2f(-WINDOW_HEIGHT, -49);
	glVertex2f(WINDOW_HEIGHT, -250);
	glEnd();

	glColor3f(0, 0, 0.6);
	glPointSize(5);
	glLineWidth(3);
	glBegin(GL_TRIANGLES);
	glVertex2f(-WINDOW_HEIGHT, -49);
	glVertex2f(-WINDOW_HEIGHT, -250);
	glVertex2f(WINDOW_HEIGHT, -250);
	glEnd();
}

void display() {
	glClearColor(0.4, 0.5, 0.9, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	displaySea();
	glColor3ub(r, g, b);
	drawSky();
	drawBoat();
	glColor3f(1, 1, 1);
	vprint(-(winWidth / 2) + 20, -(winHeight / 2) + 40, GLUT_BITMAP_8_BY_13, "R=%d G=%d B=%d", r, g, b);

	glColor3f(1, 1, 0);
	vprint(-(winWidth / 2) + 20, -(winHeight / 2) + 20, GLUT_BITMAP_8_BY_13, "Shape = Boat");

	glColor3f(0, 1, 1);
	vprint((winWidth / 2) - 290, -(winHeight / 2) + 45, GLUT_BITMAP_8_BY_13, "left click : change color");
	vprint((winWidth / 2) - 290, -(winHeight / 2) + 35, GLUT_BITMAP_8_BY_13, "space bar  : stop / start animation");

	glutSwapBuffers();
}

// Key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
void onKeyDown(unsigned char key, int x, int y) {
	// Exit when ESC is pressed.
	if (key == 27)
		exit(0);

	if (key == ' ')
		activeTimer = !activeTimer;

	// To refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y) {
	// Exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// To refresh the window it calls display() function
	glutPostRedisplay();
}

void onClick(int button, int stat, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;
	}
	// To refresh the window it calls display() function
	glutPostRedisplay();
}

/*
   This function is called when the window size changes.
   w : is the new width of the window in pixels
   h : is the new height of the window in pixels
*/

void onResize(int w, int h) {
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // Refresh window
}

#if TIMER_ON == 1
void onTimer(int v) {
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	if (activeTimer) {
		xB++;
		if (xB == INITIAL_xB)
			xB = (-1) * INITIAL_xB;
	}
	//to refresh the window it calls display() function
	glutPostRedisplay(); //display()
}
#endif

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(600, 300);
	glutCreateWindow("CTIS164 Project of mine own :)");

	// Window Events
	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	// Keyboard Events
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);

	// Mouse Events
	glutMouseFunc(onClick);

#if TIMER_ON == 1
	// Timer Event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	// Initialize random generator
	srand(time(0));

	glutMainLoop();
	return 0;
}
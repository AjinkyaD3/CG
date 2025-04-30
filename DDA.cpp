#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

int x1_input, y1_input, x2_input, y2_input;
int clickCount = 0;

void plot(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void DDA_Line(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = max(abs(dx), abs(dy));
    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++) {
        plot(round(x), round(y));
        x += xInc;
        y += yInc;
    }
}

void drawAxes() {
    glColor3f(1, 1, 1); // White
    glBegin(GL_LINES);
    glVertex2i(-500, 0); glVertex2i(500, 0); // X-axis
    glVertex2i(0, -500); glVertex2i(0, 500); // Y-axis
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    if (clickCount == 2) {
        glColor3f(1, 0, 0); // Red line
        DDA_Line(x1_input, y1_input, x2_input, y2_input);
    }

    glFlush();
}

// Convert mouse window coordinates to OpenGL coords
int windowToOpenGLX(int x) {
    return x - 400; // Since window is 800 width
}

int windowToOpenGLY(int y) {
    return 400 - y; // Flip y-axis
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (clickCount == 0) {
            x1_input = windowToOpenGLX(x);
            y1_input = windowToOpenGLY(y);
            clickCount++;
        }
        else if (clickCount == 1) {
            x2_input = windowToOpenGLX(x);
            y2_input = windowToOpenGLY(y);
            clickCount++;
            glutPostRedisplay(); // Trigger display to draw line
        }
    }
}

void init() {
    glClearColor(0, 0, 0, 1); // Black background
    gluOrtho2D(-500, 500, -500, 500); // Set 2D coordinate system
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DDA Line Drawing with Mouse");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMainLoop();

    return 0;
}

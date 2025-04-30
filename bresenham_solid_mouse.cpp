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

void Bresenham_Solid_Line(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x2 >= x1) ? 1 : -1;
    int sy = (y2 >= y1) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        plot(x1, y1);
        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }
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
        glColor3f(0, 0.8, 1); // Cyan solid line
        Bresenham_Solid_Line(x1_input, y1_input, x2_input, y2_input);
    }

    glFlush();
}

int windowToOpenGLX(int x) {
    return x - 400;
}

int windowToOpenGLY(int y) {
    return 400 - y;
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (clickCount == 0) {
            x1_input = windowToOpenGLX(x);
            y1_input = windowToOpenGLY(y);
            clickCount++;
        } else if (clickCount == 1) {
            x2_input = windowToOpenGLX(x);
            y2_input = windowToOpenGLY(y);
            clickCount++;
            glutPostRedisplay();
        }
    }
}

void init() {
    glClearColor(0, 0, 0, 1);
    gluOrtho2D(-500, 500, -500, 500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Solid Line with Mouse");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMainLoop();
    return 0;
}

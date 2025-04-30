#include <GL/glut.h>
#include <iostream>
#include <cmath>

using namespace std;

int xc, yc, radius;

void plotCirclePoints(int xc, int yc, int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);
    glEnd();
}

void drawCircle(int xc, int yc, int r)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;

    while (x <= y)
    {
        plotCirclePoints(xc, yc, x, y);
        x++;
        if (d < 0)
            d += 4 * x + 6;
        else
        {
            y--;
            d += 4 * (x - y) + 10;
        }
    }
}

void drawAxes()
{
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2i(-500, 0);
    glVertex2i(500, 0); // X-axis
    glVertex2i(0, -500);
    glVertex2i(0, 500); // Y-axis
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    glColor3f(1, 1, 0); // Yellow
    drawCircle(xc, yc, radius);

    glFlush();
}

void init()
{
    glClearColor(0, 0, 0, 1);
    gluOrtho2D(-500, 500, -500, 500);
}

int main(int argc, char **argv)
{
    cout << "Enter circle center (x y): ";
    cin >> xc >> yc;
    cout << "Enter radius: ";
    cin >> radius;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Circle Drawing");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

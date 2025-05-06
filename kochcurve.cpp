#include <GL/glut.h>
#include <cmath>

int depth = 5; // Set recursion depth (0, 1, 2...)

void koch(float x1, float y1, float x2, float y2, int n)
{
    if (n == 0)
    {
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }
    else
    {
        float dx = (x2 - x1) / 3.0;
        float dy = (y2 - y1) / 3.0;

        float x3 = x1 + dx;
        float y3 = y1 + dy;

        float x5 = x1 + 2 * dx;
        float y5 = y1 + 2 * dy;

        // Coordinates of the peak point (rotated 60°)
        float x4 = 0.5 * (x1 + x2) - sqrt(3) * (y1 - y2) / 6.0;
        float y4 = 0.5 * (y1 + y2) - sqrt(3) * (x2 - x1) / 6.0;

        koch(x1, y1, x3, y3, n - 1);
        koch(x3, y3, x4, y4, n - 1);
        koch(x4, y4, x5, y5, n - 1);
        koch(x5, y5, x2, y2, n - 1);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 1);              // Blue color
    koch(100, 300, 500, 300, depth); // Horizontal line
    glFlush();
}

void init()
{
    glClearColor(1, 1, 1, 1); // White background
    gluOrtho2D(0, 600, 0, 600);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Simple Koch Curve");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

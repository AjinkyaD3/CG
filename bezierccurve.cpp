#include <GL/glut.h>
#include <cmath>

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0); // Red color

    glBegin(GL_LINE_STRIP);
    for (float t = 0; t <= 1.0; t += 0.01)
    {
        float x = pow(1 - t, 3) * 100 +
                  3 * pow(1 - t, 2) * t * 150 +
                  3 * (1 - t) * t * t * 250 +
                  t * t * t * 300;

        float y = pow(1 - t, 3) * 100 +
                  3 * pow(1 - t, 2) * t * 300 +
                  3 * (1 - t) * t * t * 300 +
                  t * t * t * 100;

        glVertex2f(x, y);
    }
    glEnd();

    glFlush();
}

void init()
{
    glClearColor(1, 1, 1, 1);   // White background
    gluOrtho2D(0, 400, 0, 400); // 2D view
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Bezier Curve - Minimal");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

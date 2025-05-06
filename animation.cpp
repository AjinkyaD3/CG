#include <GL/glut.h>
#include <cmath>

float x = 320, y = 240;   // Ball position
float dx = 2.0, dy = 0.0; // Velocity
float gravity = 0.2;
float bounce = 0.8;
float radius = 20;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0); // Red

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int a = 0; a <= 360; a += 10)
        glVertex2f(x + radius * cos(a * M_PI / 180), y + radius * sin(a * M_PI / 180));
    glEnd();

    glFlush();
}

void update(int)
{
    dy -= gravity;
    x += dx;
    y += dy;

    if (x < radius || x > 640 - radius)
        dx = -dx;
    if (y < radius)
    {
        y = radius;
        dy = -dy * bounce;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void init()
{
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 640, 0, 480);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Mini Bounce");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}

#include <GL/glut.h>
#include <iostream>
#include <cmath>

// Window dimensions
const int WIDTH = 800, HEIGHT = 600;
int points[4] = {-1, -1, -1, -1}; // {x1, y1, x2, y2}

// Boxy staircase line algorithm
void drawDDAStaircaseLine(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1, dy = y2 - y1;
    bool steep = abs(dy) > abs(dx);
    int steps = std::max(abs(dx), abs(dy));
    float xInc = (float)dx / steps, yInc = (float)dy / steps;
    float x = x1, y = y1;
    float lastX = x1, lastY = y1;

    glBegin(GL_POINTS);
    glVertex2i(x1, y1);

    for (int i = 1; i <= steps; ++i)
    {
        x += xInc;
        y += yInc;

        if (steep)
        {
            if (i % 2 == 1)
                glVertex2i(lastX, round(y));
            else
                glVertex2i(round(x), lastY);
        }
        else
        {
            if (i % 2 == 1)
                glVertex2i(round(x), lastY);
            else
                glVertex2i(lastX, round(y));
        }
        lastX = round(x);
        lastY = round(y);
    }
    glEnd();
}

// Draw coordinate axes
void drawAxes()
{
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    glVertex2i(-WIDTH / 2, 0);
    glVertex2i(WIDTH / 2, 0);
    glVertex2i(0, -HEIGHT / 2);
    glVertex2i(0, HEIGHT / 2);
    glEnd();
}

// Display function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    if (points[0] != -1 && points[2] != -1)
    {
        glColor3f(1.0, 1.0, 1.0);
        drawDDAStaircaseLine(points[0], points[1], points[2], points[3]);
    }

    glutSwapBuffers();
}

// Mouse click handler
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        int cartX = x - WIDTH / 2, cartY = HEIGHT / 2 - y;

        if (points[0] == -1)
        {
            points[0] = cartX;
            points[1] = cartY;
        }
        else
        {
            points[2] = cartX;
            points[3] = cartY;
        }

        glutPostRedisplay();
    }
}

// Keyboard handler
void keyboard(unsigned char key, int x, int y)
{
    if (key == 'c' || key == 'C')
    {
        points[0] = points[1] = points[2] = points[3] = -1;
        glutPostRedisplay();
    }
    else if (key == 27)
        exit(0); // ESC
}

// Main function
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("DDA Boxy Staircase Line");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    std::cout << "Click to select 2 points. Press 'C' to clear. ESC to exit.\n";
    glutMainLoop();
    return 0;
}

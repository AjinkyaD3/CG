#include <GL/glut.h>
#include <iostream>
#include <cmath>

// Window dimensions
const int WIDTH = 800, HEIGHT = 600;
int points[4] = {-1, -1, -1, -1}; // {x1, y1, x2, y2}
int pointCount = 0;
bool drawLine = false;

// DDA staircase line algorithm
void drawDDALine(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    bool steep = abs(dy) > abs(dx);
    int steps = std::max(abs(dx), abs(dy));
    float xInc = (float)dx / steps;
    float yInc = (float)dy / steps;

    float x = x1, y = y1;
    int prevX = round(x), prevY = round(y);

    // Draw first pixel
    glBegin(GL_POINTS);
    glVertex2i(prevX, prevY);

    // Draw the staircase
    for (int i = 0; i < steps; i++)
    {
        x += xInc;
        y += yInc;

        int currX = round(x);
        int currY = round(y);

        if (currX != prevX || currY != prevY)
        {
            // Draw step segment
            if (steep && currY != prevY)
                glVertex2i(prevX, currY);
            else if (!steep && currX != prevX)
                glVertex2i(currX, prevY);

            // Draw current point
            glVertex2i(currX, currY);

            prevX = currX;
            prevY = currY;
        }
    }
    glEnd();
}

// Draw coordinate axes
void drawAxes()
{
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    glVertex2i(-WIDTH / 2, 0);
    glVertex2i(WIDTH / 2, 0); // X-axis
    glVertex2i(0, -HEIGHT / 2);
    glVertex2i(0, HEIGHT / 2); // Y-axis
    glEnd();
}

// Display function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    // Draw points
    if (points[0] != -1)
    {
        glColor3f(1.0, 0.0, 0.0);
        glPointSize(5.0);
        glBegin(GL_POINTS);
        glVertex2i(points[0], points[1]);
        if (points[2] != -1)
            glVertex2i(points[2], points[3]);
        glEnd();
    }

    // Draw line
    if (drawLine)
    {
        glColor3f(1.0, 1.0, 1.0);
        glPointSize(3.0);
        drawDDALine(points[0], points[1], points[2], points[3]);
    }

    glutSwapBuffers();
}

// Mouse click handler
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // Convert to Cartesian coordinates
        int cartX = x - WIDTH / 2;
        int cartY = HEIGHT / 2 - y;

        if (pointCount == 0)
        {
            points[0] = cartX;
            points[1] = cartY;
            pointCount = 1;
            drawLine = false;
        }
        else
        {
            points[2] = cartX;
            points[3] = cartY;
            pointCount = 0;
            drawLine = true;
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
        pointCount = 0;
        drawLine = false;
        glutPostRedisplay();
    }
    else if (key == 27)
    { // ESC
        exit(0);
    }
}

// Main function
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("DDA Staircase Line");

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
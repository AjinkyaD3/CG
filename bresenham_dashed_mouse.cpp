#include <GL/glut.h> // GLUT for OpenGL functions
#include <cmath>     // For abs()
#include <iostream>  // For I/O

using namespace std;

// Line endpoints
int x1_input, y1_input, x2_input, y2_input;
int clickCount = 0;  // Tracks mouse clicks
int dashLength = 10; // Length of dashes

// Plot a single pixel
void plot(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Draw dashed line using Bresenham's algorithm
void Bresenham_Dashed_Line(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    int dash = 10; // Dash length
    int gap = 5;   // Gap length
    int patternLength = dash + gap;
    int step = 0; // Steps moved

    while (true)
    {
        // Only draw during the dash part
        if (step % patternLength < dash)
        {
            plot(x1, y1);
        }

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }

        step++; // Increment step after each move
    }
}

// Draw X and Y axes
void drawAxes()
{
    glColor3f(1, 1, 1); // White color
    glBegin(GL_LINES);
    glVertex2i(-500, 0);
    glVertex2i(500, 0); // X-axis
    glVertex2i(0, -500);
    glVertex2i(0, 500); // Y-axis
    glEnd();
}

// Display callback
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    if (clickCount == 2)
    {
        glColor3f(1, 0, 0); // Red dashed line
        Bresenham_Dashed_Line(x1_input, y1_input, x2_input, y2_input);
    }
    glFlush();
}

// Convert window X to OpenGL X
int windowToOpenGLX(int x)
{
    return x - 400;
}

// Convert window Y to OpenGL Y
int windowToOpenGLY(int y)
{
    return 400 - y;
}

// Mouse click handler
void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (clickCount == 0)
        {
            x1_input = windowToOpenGLX(x);
            y1_input = windowToOpenGLY(y);
            clickCount++;
        }
        else if (clickCount == 1)
        {
            x2_input = windowToOpenGLX(x);
            y2_input = windowToOpenGLY(y);
            clickCount++;
            glutPostRedisplay(); // Refresh display
        }
    }
}

// OpenGL setup
void init()
{
    glClearColor(0, 0, 0, 1);         // Black background
    gluOrtho2D(-500, 500, -500, 500); // Set 2D projection
}

// Main function
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Dashed Line");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMainLoop();
    return 0;
}

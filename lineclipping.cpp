#include <iostream>
#include <GL/glut.h>
#include <math.h>
using namespace std;

// These are the clipping window boundaries (xmin, xmax, ymin, ymax)
int xl = 50, xh = 200, yl = 50, yh = 200;

// flag = 0 means waiting for first point, flag = 1 means waiting for second
int flag = 0;

// Stores mouse click coordinates
float u1, v1, u2, v2;

// Structure to store outcodes (top, bottom, right, left)
struct code
{
    int t, b, r, l;
};

// Set background color and initialize OpenGL
void init()
{
    glClearColor(1, 1, 1, 0);     // White background
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
    glColor3f(0, 0, 0);           // Set drawing color to black
}

// Generate outcode for a point (whether it's outside top/bottom/left/right)
code get_code(int u, int v)
{
    code c = {0, 0, 0, 0}; // Initially assume point is inside

    if (u < xl)
        c.l = 1; // Left
    if (u > xh)
        c.r = 1; // Right
    if (v < yl)
        c.b = 1; // Bottom
    if (v > yh)
        c.t = 1; // Top

    return c;
}

// Draws a line using DDA (Digital Differential Analyzer) Algorithm
void line(float u1, float v1, float u2, float v2)
{
    float dx = u2 - u1;
    float dy = v2 - v1;
    float x = u1, y = v1;

    // Calculate steps based on the larger difference
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    // Calculate the increment per step
    float xi = dx / steps;
    float yi = dy / steps;

    // Start drawing the points of the line
    glBegin(GL_POINTS);
    glVertex2f(x, y);

    for (int i = 0; i < steps; i++)
    {
        x += xi;
        y += yi;
        glVertex2f(x, y); // Draw point
    }

    glEnd();
    glFlush(); // Show all points
}

// Draw the rectangular clipping window
void draw_window()
{
    line(xl, yl, xh, yl); // Bottom edge
    line(xl, yl, xl, yh); // Left edge
    line(xh, yl, xh, yh); // Right edge
    line(xl, yh, xh, yh); // Top edge
}

// Get mouse input to define line
void mymouse(int button, int state, int x, int y)
{
    glColor3f(0, 0, 0); // Black color

    if (state == GLUT_DOWN && flag == 0)
    {
        u1 = x;       // Save first point's x
        v1 = 480 - y; // Save first point's y (invert Y for OpenGL)
        flag = 1;     // Set flag for next point
    }
    else if (state == GLUT_DOWN && flag == 1)
    {
        u2 = x;
        v2 = 480 - y;
        flag = 2;             // Both points are now captured
        line(u1, v1, u2, v2); // Draw the original line
    }
}

// Cohen-Sutherland Line Clipping Algorithm
void cohen()
{
    code c1, c2, c;
    float m;    // slope of the line
    int xi, yi; // intersection points
    int flag;

    m = (v2 - v1) / (u2 - u1); // Calculate slope
    c1 = get_code(u1, v1);     // Outcode for point 1
    c2 = get_code(u2, v2);     // Outcode for point 2

    while (1)
    {
        // If both points are inside the window, draw the line
        if ((c1.t | c1.b | c1.r | c1.l | c2.t | c2.b | c2.r | c2.l) == 0)
            break;

        // If both points share an outside zone (AND of outcodes), discard line
        else if ((c1.t & c2.t) || (c1.b & c2.b) || (c1.r & c2.r) || (c1.l & c2.l))
        {
            u1 = v1 = u2 = v2 = 0; // Line is completely outside, don't draw
            break;
        }

        // Line is partially inside — clip it
        else
        {
            if (c1.l || c2.l) // If either point is left of window
            {
                xi = xl;
                yi = v1 + m * (xl - u1);
                flag = (c1.l == 1) ? 0 : 1;
            }
            else if (c1.r || c2.r) // Right of window
            {
                xi = xh;
                yi = v1 + m * (xh - u1);
                flag = (c1.r == 1) ? 0 : 1;
            }
            else if (c1.b || c2.b) // Below window
            {
                xi = u1 + (1.0 / m) * (yl - v1);
                yi = yl;
                flag = (c1.b == 1) ? 0 : 1;
            }
            else if (c1.t || c2.t) // Above window
            {
                xi = u1 + (1.0 / m) * (yh - v1);
                yi = yh;
                flag = (c1.t == 1) ? 0 : 1;
            }

            c = get_code(xi, yi); // Get outcode for intersection point

            // Replace the outside point with the intersection
            if (flag == 0)
            {
                u1 = xi;
                v1 = yi;
                c1 = c;
            }
            else
            {
                u2 = xi;
                v2 = yi;
                c2 = c;
            }
        }
    }

    // Finally, draw the window and clipped line
    draw_window();
    line(u1, v1, u2, v2);
}

// Keyboard function
void mykey(unsigned char key, int x, int y)
{
    if (key == 'c') // Press 'c' to clip
    {
        init();
        cohen();
    }
    else if (key == 'r') // Press 'r' to reset
    {
        init();
        draw_window();
        flag = 0;
    }
}

// Main function
int main(int argc, char **argv)
{
    glutInit(&argc, argv);             // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE);  // Single buffer window
    glutInitWindowSize(640, 480);      // Set window size
    glutInitWindowPosition(0, 0);      // Window position
    glutCreateWindow("Line_Clipping"); // Create window

    gluOrtho2D(0, 640, 0, 480); // Set orthographic 2D view

    init();    // Set up OpenGL
    glFlush(); // Render first frame

    draw_window(); // Draw clipping rectangle

    glutMouseFunc(mymouse);  // Mouse input for line
    glutKeyboardFunc(mykey); // Keyboard input

    glutMainLoop(); // Keep window open

    return 0;
}

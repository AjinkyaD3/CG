#include <GL/glut.h> // OpenGL Utility Toolkit for GUI and graphics
#include <cmath>     // For sqrt function

// Global variables to store circle center, radius, and number of mouse clicks
int xc, yc, r, clicks = 0;

// Function to plot 8 symmetrical points of a circle
void plot(int xc, int yc, int x, int y)
{
    glBegin(GL_POINTS);         // Start drawing points
    glVertex2i(xc + x, yc + y); // Right-Top
    glVertex2i(xc - x, yc + y); // Left-Top
    glVertex2i(xc + x, yc - y); // Right-Bottom
    glVertex2i(xc - x, yc - y); // Left-Bottom
    glVertex2i(xc + y, yc + x); // Top-Right
    glVertex2i(xc - y, yc + x); // Top-Left
    glVertex2i(xc + y, yc - x); // Bottom-Right
    glVertex2i(xc - y, yc - x); // Bottom-Left
    glEnd();                    // End drawing
}

// Function to draw a circle using Bresenham's Algorithm
void drawCircle(int xc, int yc, int r)
{
    int x = 0, y = r; // Start from top of the circle

    int d = 3 - 2 * r; // Initial decision parameter

    while (x <= y)
    {                       // Loop until x crosses y
        plot(xc, yc, x, y); // Plot symmetric points
        if (d < 0)          // Midpoint is inside circle
            d += 4 * x + 6; // Update d accordingly
        else
        {                          // Midpoint is outside or on circle
            d += 4 * (x - y) + 10; // Update d and move inward
            y--;                   // Decrease y
        }
        x++; // Always increase x
    }
}

// Draw X and Y axes on the screen
void drawAxes()
{
    glColor3f(1, 1, 1);  // White color
    glBegin(GL_LINES);   // Begin line drawing
    glVertex2i(-500, 0); // X-axis: left
    glVertex2i(500, 0);  // X-axis: right
    glVertex2i(0, -500); // Y-axis: bottom
    glVertex2i(0, 500);  // Y-axis: top
    glEnd();             // End line drawing
}

// Main display function that draws everything on screen
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear background
    drawAxes();                   // Draw X and Y axes

    if (clicks == 2)
    {                          // If both clicks done
        glColor3f(0, 1, 0);    // Set color to green
        drawCircle(xc, yc, r); // Draw the circle
    }

    glFlush(); // Execute all drawing commands
}

// Convert window x coordinate to OpenGL x coordinate
int toGLX(int x) { return x - 400; }

// Convert window y coordinate to OpenGL y coordinate
int toGLY(int y) { return 400 - y; }

// Mouse click handler
void mouse(int btn, int state, int x, int y)
{
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (clicks == 0)
        {
            // First click - set circle center
            xc = toGLX(x);
            yc = toGLY(y);
            clicks++;
        }
        else if (clicks == 1)
        {
            // Second click - calculate radius from distance
            int x2 = toGLX(x), y2 = toGLY(y);
            r = sqrt((x2 - xc) * (x2 - xc) + (y2 - yc) * (y2 - yc));
            clicks++;
            glutPostRedisplay(); // Refresh display to draw circle
        }
    }
}

// OpenGL initialization
void init()
{
    glClearColor(0, 0, 0, 1);         // Set background to black
    gluOrtho2D(-500, 500, -500, 500); // Define 2D orthographic projection
}

// Main function - entry point
int main(int argc, char **argv)
{
    glutInit(&argc, argv);                       // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Single buffer and RGB color mode
    glutInitWindowSize(800, 800);                // Set window size
    glutInitWindowPosition(100, 100);            // Set window position on screen
    glutCreateWindow("Bresenham Circle");        // Create window with title
    init();                                      // Initialize settings
    glutDisplayFunc(display);                    // Register display callback
    glutMouseFunc(mouse);                        // Register mouse callback
    glutMainLoop();                              // Start the event loop
    return 0;
}

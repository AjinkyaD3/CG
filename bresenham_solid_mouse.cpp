#include <GL/glut.h>                        // For OpenGL functions
#include <cmath>                            // For abs()
int x1_in, y1_in, x2_in, y2_in, clicks = 0; // Store 2 points and click count

// Plot a pixel
void plot(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Bresenham's Line Drawing Algorithm
void bresenham(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);       // Delta x
    int dy = abs(y2 - y1);       // Delta y
    int sx = (x2 > x1) ? 1 : -1; // X step direction
    int sy = (y2 > y1) ? 1 : -1; // Y step direction
    int err = dx - dy;           // Error term

    while (true)
    {
        plot(x1, y1); // Plot current pixel
        if (x1 == x2 && y1 == y2)
            break;        // Exit when end point reached
        int e2 = 2 * err; // Double error for decision
        if (e2 > -dy)  // Check if we need to move in X direction
        {
            err -= dy; // Update error
            x1 += sx;  // Move in X
        }
        if (e2 < dx) // Check if we need to move in Y direction
        {
            err += dx; // Update error
            y1 += sy;  // Move in Y
        }
    }
}

// Display callback
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear screen
    glColor3f(1, 1, 1);           // White color
    glBegin(GL_LINES);            // Draw X and Y axes
    glVertex2i(-500, 0);
    glVertex2i(500, 0);
    glVertex2i(0, -500);
    glVertex2i(0, 500);
    glEnd();

    if (clicks == 2)
    {                                          // If both points clicked
        glColor3f(0, 1, 1);                    // Cyan color
        bresenham(x1_in, y1_in, x2_in, y2_in); // Draw line
    }

    glFlush(); // Render now
}

// Convert mouse to OpenGL coords
int toGLX(int x) { return x - 400; }
int toGLY(int y) { return 400 - y; }

// Mouse click handler
void mouse(int btn, int state, int x, int y)
{
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (clicks == 0)
        {
            x1_in = toGLX(x);
            y1_in = toGLY(y);
        }
        else if (clicks == 1)
        {
            x2_in = toGLX(x);
            y2_in = toGLY(y);
            glutPostRedisplay();
        }
        clicks++;
    }
}

// Initialization
void init()
{
    glClearColor(0, 0, 0, 1);         // Black background
    gluOrtho2D(-500, 500, -500, 500); // Set coordinate range
}

// Main function
int main(int argc, char **argv)
{
    glutInit(&argc, argv);                       // Init GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Single buffer, RGB mode
    glutInitWindowSize(800, 800);                // Window size
    glutCreateWindow("Bresenham Line");          // Title
    init();                                      // Set background and coords
    glutDisplayFunc(display);                    // Draw callback
    glutMouseFunc(mouse);                        // Mouse click handler
    glutMainLoop();                              // Start main loop
    return 0;
}

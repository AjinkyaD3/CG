// Boundary Fill Algorithm using OpenGL (simplified with comments)
#include <GL/glut.h> // OpenGL toolkit
#include <iostream>  // Console I/O
#include <math.h>    // Math functions

using namespace std;

// Set up OpenGL: white background and 2D orthographic projection
void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0); // Set background color to white
    glMatrixMode(GL_PROJECTION);      // Use projection matrix
    gluOrtho2D(0, 640, 0, 480);       // Set coordinate system
}

// Compare two colors with tolerance
bool isSameColor(float *a, float *b, float tol = 0.1f)
{
    return (fabs(a[0] - b[0]) < tol &&
            fabs(a[1] - b[1]) < tol &&
            fabs(a[2] - b[2]) < tol);
}

// Recursive boundary fill function
void boundaryFill(int x, int y, float *fillColor, float *boundaryColor)
{
    float pixelColor[3];                                    // Store current pixel color
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixelColor); // Read pixel

    // If pixel is not boundary and not filled yet
    if (!isSameColor(pixelColor, boundaryColor) && !isSameColor(pixelColor, fillColor))
    {
        glColor3fv(fillColor); // Set fill color
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();   // Draw point
        glFlush(); // Render immediately

        // Recursive calls in 4 directions
        boundaryFill(x + 1, y, fillColor, boundaryColor);
        boundaryFill(x - 1, y, fillColor, boundaryColor);
        boundaryFill(x, y + 1, fillColor, boundaryColor);
        boundaryFill(x, y - 1, fillColor, boundaryColor);
    }
}

// Mouse click handler: starts filling on left click
void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        float boundary[] = {0, 0, 1};             // Blue boundary color
        float fill[] = {0, 1, 0};                 // Green fill color
        boundaryFill(x, 480 - y, fill, boundary); // Convert y and fill
    }
}

// Draw the triangle to fill
void drawTriangle()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear screen
    glColor3f(0, 0, 1);           // Set boundary color (blue)

    glBegin(GL_LINE_LOOP); // Draw triangle outline
    glVertex2i(150, 100);
    glVertex2i(300, 300);
    glVertex2i(450, 100);
    glEnd();
    glFlush(); // Render the triangle
}

// Main program entry point
int main(int argc, char **argv)
{
    glutInit(&argc, argv);                       // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Single buffer, RGB
    glutInitWindowSize(640, 480);                // Set window size
    glutInitWindowPosition(200, 200);            // Set window position
    glutCreateWindow("Boundary Fill Algorithm"); // Window title

    init();                        // Setup OpenGL
    glutDisplayFunc(drawTriangle); // Draw triangle on display
    glutMouseFunc(mouseClick);     // Set mouse callback
    glutMainLoop();                // Run the event loop

    return 0;
}

#include <GL/glut.h> // OpenGL Utility Toolkit

// Define clipping window boundaries
int xmin = 100, xmax = 300, ymin = 100, ymax = 300;

// Coordinates of line endpoints
float x_start, y_start, x_end, y_end;

int clickCount = 0;

// Function to compute region code for a point
int getCode(float x, float y)
{
    int code = 0;
    if (x < xmin)
        code |= 1; // Left
    if (x > xmax)
        code |= 2; // Right
    if (y < ymin)
        code |= 4; // Bottom
    if (y > ymax)
        code |= 8; // Top
    return code;
}

// Cohen-Sutherland Line Clipping Algorithm
void clipLine()
{
    int code1 = getCode(x_start, y_start);
    int code2 = getCode(x_end, y_end);
    bool accept = false;

    while (true)
    {
        if ((code1 | code2) == 0)
        {
            // Both points inside — trivially accept
            accept = true;
            break;
        }
        else if (code1 & code2)
        {
            // Both points share an outside region — trivially reject
            break;
        }
        else
        {
            // At least one point is outside — need to clip
            float x, y;
            int outCode = code1 ? code1 : code2;

            // Find intersection point
            if (outCode & 8)
            { // Top
                x = x_start + (x_end - x_start) * (ymax - y_start) / (y_end - y_start);
                y = ymax;
            }
            else if (outCode & 4)
            { // Bottom
                x = x_start + (x_end - x_start) * (ymin - y_start) / (y_end - y_start);
                y = ymin;
            }
            else if (outCode & 2)
            { // Right
                y = y_start + (y_end - y_start) * (xmax - x_start) / (x_end - x_start);
                x = xmax;
            }
            else
            { // Left
                y = y_start + (y_end - y_start) * (xmin - x_start) / (x_end - x_start);
                x = xmin;
            }

            // Replace the point outside with intersection
            if (outCode == code1)
            {
                x_start = x;
                y_start = y;
                code1 = getCode(x_start, y_start);
            }
            else
            {
                x_end = x;
                y_end = y;
                code2 = getCode(x_end, y_end);
            }
        }
    }

    // Draw the result
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINE_LOOP); // Draw clipping rectangle
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glEnd();

    if (accept)
    {
        glBegin(GL_LINES); // Draw clipped line
        glVertex2f(x_start, y_start);
        glVertex2f(x_end, y_end);
        glEnd();
    }

    glFlush(); // Show drawing
}

// Mouse function to get 2 clicks for line endpoints

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        if (clickCount == 0)
        {
            x_start = x;
            y_start = 480 - y; // Invert Y for OpenGL
            clickCount = 1;
        }
        else if (clickCount == 1)
        {
            x_end = x;
            y_end = 480 - y;
            clickCount = 2;

            // Draw the clipping rectangle
            glClear(GL_COLOR_BUFFER_BIT);
            glBegin(GL_LINE_LOOP);
            glVertex2i(xmin, ymin);
            glVertex2i(xmax, ymin);
            glVertex2i(xmax, ymax);
            glVertex2i(xmin, ymax);
            glEnd();

            // Draw the original (unclipped) line
            glBegin(GL_LINES);
            glVertex2f(x_start, y_start);
            glVertex2f(x_end, y_end);
            glEnd();

            glFlush(); // Show everything
        }
    }
}

// Keyboard function: 'c' to clip, 'r' to reset
void keyboard(unsigned char key, int x, int y)
{
    if (key == 'c' && clickCount == 2)
    {
        clipLine(); // Clip and draw line
    }
    else if (key == 'r')
    {
        clickCount = 0; // Reset
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_LINE_LOOP); // Draw rectangle again
        glVertex2i(xmin, ymin);
        glVertex2i(xmax, ymin);
        glVertex2i(xmax, ymax);
        glVertex2i(xmin, ymax);
        glEnd();
        glFlush();
    }
}

// OpenGL setup
void init()
{
    glClearColor(1, 1, 1, 1);     // White background
    gluOrtho2D(0, 640, 0, 480);   // Set 2D view
    glColor3f(0, 0, 0);           // Black drawing color
    glClear(GL_COLOR_BUFFER_BIT); // Clear screen

    // Draw initial clipping rectangle
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glEnd();
    glFlush();
}

// Main function
int main(int argc, char **argv)
{
    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Cohen-Sutherland Line Clipping");

    init(); // Set up view and color

    glutMouseFunc(mouse);       // Mouse input
    glutKeyboardFunc(keyboard); // Keyboard input
    glutMainLoop();             // Run the loop
    return 0;
}

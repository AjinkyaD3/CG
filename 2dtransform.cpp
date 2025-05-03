#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// Store polygon points
vector<int> xpoints, ypoints;
int edges; // Number of polygon edges
int choice;

// Round-off function
double roundOff(double value)
{
    return floor(value + 0.5);
}

// OpenGL Initialization
void init()
{
    glClearColor(1, 1, 1, 0);   // White background
    gluOrtho2D(0, 640, 0, 480); // 2D projection
    glClear(GL_COLOR_BUFFER_BIT);
}

// Draw the initial polygon
void drawPolygon(float r, float g, float b)
{
    glColor3f(r, g, b); // RGB color
    glBegin(GL_POLYGON);
    for (int i = 0; i < edges; i++)
    {
        glVertex2i(xpoints[i], ypoints[i]);
    }
    glEnd();
    glFlush();
}

// Translation
void translatePolygon()
{
    int tx, ty;
    cout << "Enter Tx and Ty (translation factors): ";
    cin >> tx >> ty;

    for (int i = 0; i < edges; i++)
    {
        xpoints[i] += tx;
        ypoints[i] += ty;
    }

    drawPolygon(0, 0, 1); // Blue polygon
}

// Scaling (origin at screen center)
void scalePolygon()
{
    int sx, sy;
    cout << "Enter Sx and Sy (scaling factors): ";
    cin >> sx >> sy;

    for (int i = 0; i < edges; i++)
    {
        xpoints[i] = (xpoints[i] - 320) * sx + 320;
        ypoints[i] = (ypoints[i] - 240) * sy + 240;
    }

    drawPolygon(0, 1, 0); // Green polygon
}

// Rotation about an arbitrary point
void rotatePolygon()
{
    int rx, ry;
    double angle;
    cout << "Enter Arbitrary Point (x y): ";
    cin >> rx >> ry;
    cout << "Enter Rotation Angle (in degrees): ";
    cin >> angle;

    angle = angle * M_PI / 180; // Convert to radians

    for (int i = 0; i < edges; i++)
    {
        int x = xpoints[i];
        int y = ypoints[i];
        xpoints[i] = roundOff((x - rx) * cos(angle) - (y - ry) * sin(angle) + rx);
        ypoints[i] = roundOff((x - rx) * sin(angle) + (y - ry) * cos(angle) + ry);
    }

    drawPolygon(1, 0, 0); // Red polygon
}

// Reflection over X or Y axis
void reflectPolygon()
{
    char axis;
    cout << "Reflect over which axis (X or Y)? ";
    cin >> axis;

    for (int i = 0; i < edges; i++)
    {
        if (axis == 'x' || axis == 'X')
        {
            ypoints[i] = 480 - ypoints[i]; // Reflect over X-axis (invert y)
        }
        else if (axis == 'y' || axis == 'Y')
        {
            xpoints[i] = 640 - xpoints[i]; // Reflect over Y-axis (invert x)
        }
    }

    drawPolygon(0, 0, 1); // Blue polygon
}

// GLUT display function
void display()
{
    drawPolygon(1, 0, 0); // Original polygon in red

    switch (choice)
    {
    case 1:
        scalePolygon();
        break;
    case 2:
        rotatePolygon();
        break;
    case 3:
        reflectPolygon();
        break;
    case 4:
        translatePolygon();
        break;
    }
}

// Main function
int main(int argc, char **argv)
{
    cout << "2D Transformations:\n";
    cout << "1. Scaling\n2. Rotation\n3. Reflection\n4. Translation\n";
    cout << "Enter your choice (1-4): ";
    cin >> choice;

    if (choice < 1 || choice > 4)
    {
        cout << "Invalid choice. Exiting.\n";
        return 0;
    }

    cout << "Enter number of edges: ";
    cin >> edges;

    cout << "Enter " << edges << " polygon points (x y):\n";
    for (int i = 0; i < edges; i++)
    {
        int x, y;
        cin >> x >> y;
        xpoints.push_back(x + 320); // Shift origin to center (320, 240)
        ypoints.push_back(y + 240);
    }

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2D Transformations");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

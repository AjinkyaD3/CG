#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

vector<int> xpoints, ypoints;
int edges, choice;

void init()
{
    glClearColor(1, 1, 1, 0);   // White background
    gluOrtho2D(0, 640, 0, 480); // Setting up the coordinate system
}

void drawPolygon(float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < edges; i++)
        glVertex2i(xpoints[i], ypoints[i]);
    glEnd();
    glFlush();
}

void transformPolygon()
{
    int tx, ty;
    float sx, sy, angle;
    char axis;

    switch (choice)
    {
    case 1: // Scale
        cout << "Enter scaling factors (Sx, Sy): ";
        cin >> sx >> sy;
        for (int i = 0; i < edges; i++)
        {
            xpoints[i] = (xpoints[i] - 320) * sx + 320;
            ypoints[i] = (ypoints[i] - 240) * sy + 240;
        }
        break;

    case 2: // Rotate
        cout << "Enter angle (in degrees): ";
        cin >> angle;
        angle = angle * M_PI / 180; // Convert to radians
        for (int i = 0; i < edges; i++)
        {
            int x = xpoints[i] - 320;
            int y = ypoints[i] - 240;
            xpoints[i] = cos(angle) * x - sin(angle) * y + 320;
            ypoints[i] = sin(angle) * x + cos(angle) * y + 240;
        }
        break;

    case 3: // Reflect
        cout << "Enter axis (X or Y): ";
        cin >> axis;
        for (int i = 0; i < edges; i++)
        {
            if (axis == 'X' || axis == 'x')
                ypoints[i] = 480 - ypoints[i];
            else if (axis == 'Y' || axis == 'y')
                xpoints[i] = 640 - xpoints[i];
        }
        break;

    case 4: // Translate
        cout << "Enter translation values (Tx, Ty): ";
        cin >> tx >> ty;
        for (int i = 0; i < edges; i++)
        {
            xpoints[i] += tx;
            ypoints[i] += ty;
        }
        break;

    default:
        cout << "Invalid choice!" << endl;
    }

    // Debugging: Print the transformed points to verify the transformation
    cout << "Transformed points: \n";
    for (int i = 0; i < edges; i++)
    {
        cout << "(" << xpoints[i] << ", " << ypoints[i] << ")\n";
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
    drawPolygon(1, 0, 0);         // Draw original polygon in red

    transformPolygon(); // Apply transformation

    drawPolygon(0, 0, 1); // Draw transformed polygon in blue
}

int main(int argc, char **argv)
{
    cout << "1. Scale\n2. Rotate\n3. Reflect\n4. Translate\nEnter choice: ";
    cin >> choice;

    cout << "Enter number of edges: ";
    cin >> edges;
    cout << "Enter " << edges << " points (x y):\n";
    for (int i = 0; i < edges; i++)
    {
        int x, y;
        cin >> x >> y;
        xpoints.push_back(x + 320); // Center the polygon at (320, 240)
        ypoints.push_back(y + 240);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("2D Transformations");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

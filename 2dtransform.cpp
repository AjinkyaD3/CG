#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

vector<int> xpoints, ypoints;
int edges, choice;

void init()
{
    glClearColor(1, 1, 1, 0); // White background
    gluOrtho2D(0, 640, 0, 480);
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

void translatePolygon()
{
    int tx, ty;
    cout << "Enter Tx and Ty: ";
    cin >> tx >> ty;
    for (int i = 0; i < edges; i++)
    {
        xpoints[i] += tx;
        ypoints[i] += ty;
    }
}

void scalePolygon()
{
    float sx, sy;
    cout << "Enter Sx and Sy: ";
    cin >> sx >> sy;
    for (int i = 0; i < edges; i++)
    {
        xpoints[i] = (xpoints[i] - 320) * sx + 320;
        ypoints[i] = (ypoints[i] - 240) * sy + 240;
    }
}

void rotatePolygon()
{
    float angle;
    cout << "Enter angle (degrees): ";
    cin >> angle;
    angle = angle * M_PI / 180;
    for (int i = 0; i < edges; i++)
    {
        int x = xpoints[i] - 320;
        int y = ypoints[i] - 240;
        xpoints[i] = cos(angle) * x - sin(angle) * y + 320;
        ypoints[i] = sin(angle) * x + cos(angle) * y + 240;
    }
}

void reflectPolygon()
{
    char axis;
    cout << "Enter axis (X or Y): ";
    cin >> axis;
    for (int i = 0; i < edges; i++)
    {
        if (axis == 'X' || axis == 'x')
            ypoints[i] = 480 - ypoints[i];
        else if (axis == 'Y' || axis == 'y')
            xpoints[i] = 640 - xpoints[i];
    }
}

void display()
{
    drawPolygon(1, 0, 0); // Red original

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

    drawPolygon(0, 0, 1); // Blue transformed
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
        xpoints.push_back(x + 320);
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

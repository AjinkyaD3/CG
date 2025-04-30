#include <GL/glut.h>
#include <iostream>
#include <vector>
using namespace std;

int wxmin = 200, wxmax = 500, wymin = 100, wymax = 350; // Clipping window coordinates
vector<pair<int, int>> points;                          // Polygon points as pairs of x, y
int edge;                                               // Number of edges in the polygon

void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0); // Set background color to white
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480); // Set coordinate system
    glClear(GL_COLOR_BUFFER_BIT);
}

void Draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the clipped polygon
    glColor3f(0.2, 0.2, 1); // Set polygon color to blue
    glBegin(GL_POLYGON);
    for (int i = 0; i < edge; i++)
    {
        glVertex2i(points[i].first, points[i].second);
    }
    glEnd();

    // Draw the clipping window (green)
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(wxmin, wymin);
    glVertex2i(wxmax, wymin);
    glVertex2i(wxmax, wymax);
    glVertex2i(wxmin, wymax);
    glEnd();

    glFlush();
}

int BottomCliping(int e)
{
    vector<pair<int, int>> t;
    int k = 0;
    for (int i = 0; i < e; i++)
    {
        int x1 = points[i].first, y1 = points[i].second;
        int x2 = points[(i + 1) % e].first, y2 = points[(i + 1) % e].second;

        // Check if point is below the clipping window
        if (y1 < wymin && y2 >= wymin)
        {
            float x = x1 + (wymin - y1) * (x2 - x1) / (y2 - y1);
            t.push_back({x, wymin});
        }
        if (y1 >= wymin)
        {
            t.push_back({x1, y1});
        }
    }

    // Update points array with clipped points
    points = t;
    return points.size();
}

int TopCliping(int e)
{
    vector<pair<int, int>> t;
    int k = 0;
    for (int i = 0; i < e; i++)
    {
        int x1 = points[i].first, y1 = points[i].second;
        int x2 = points[(i + 1) % e].first, y2 = points[(i + 1) % e].second;

        if (y1 > wymax && y2 <= wymax)
        {
            float x = x1 + (wymax - y1) * (x2 - x1) / (y2 - y1);
            t.push_back({x, wymax});
        }
        if (y1 <= wymax)
        {
            t.push_back({x1, y1});
        }
    }

    // Update points array with clipped points
    points = t;
    return points.size();
}

int LeftCliping(int e)
{
    vector<pair<int, int>> t;
    int k = 0;
    for (int i = 0; i < e; i++)
    {
        int x1 = points[i].first, y1 = points[i].second;
        int x2 = points[(i + 1) % e].first, y2 = points[(i + 1) % e].second;

        if (x1 < wxmin && x2 >= wxmin)
        {
            float y = y1 + (wxmin - x1) * (y2 - y1) / (x2 - x1);
            t.push_back({wxmin, y});
        }
        if (x1 >= wxmin)
        {
            t.push_back({x1, y1});
        }
    }

    // Update points array with clipped points
    points = t;
    return points.size();
}

int RightCliping(int e)
{
    vector<pair<int, int>> t;
    int k = 0;
    for (int i = 0; i < e; i++)
    {
        int x1 = points[i].first, y1 = points[i].second;
        int x2 = points[(i + 1) % e].first, y2 = points[(i + 1) % e].second;

        if (x1 > wxmax && x2 <= wxmax)
        {
            float y = y1 + (wxmax - x1) * (y2 - y1) / (x2 - x1);
            t.push_back({wxmax, y});
        }
        if (x1 <= wxmax)
        {
            t.push_back({x1, y1});
        }
    }

    // Update points array with clipped points
    points = t;
    return points.size();
}

void goMenu(int value)
{
    switch (value)
    {
    case 1:
        edge = LeftCliping(edge);
        break;
    case 2:
        edge = RightCliping(edge);
        break;
    case 3:
        edge = TopCliping(edge);
        break;
    case 4:
        edge = BottomCliping(edge);
        break;
    }
    Draw();
    glutPostRedisplay();
}

// Function to add a point when clicked
void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // OpenGL's origin is at the bottom left, so we need to invert y
        y = 480 - y;

        // Add point to the polygon
        points.push_back({x, y});
        edge = points.size();

        // Draw the polygon as the points are added
        Draw();
        glutPostRedisplay();
    }
}

// Function to finalize the polygon when 'Enter' key is pressed
void keyboard(unsigned char key, int x, int y)
{
    if (key == 13)
    { // Enter key
        cout << "Polygon finalized!" << endl;
        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Polygon Clipping");

    init();

    // Right-click menu for clipping operations
    glutCreateMenu(goMenu);
    glutAddMenuEntry("Left", 1);
    glutAddMenuEntry("Right", 2);
    glutAddMenuEntry("Top", 3);
    glutAddMenuEntry("Bottom", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // Mouse click callback to add points to polygon
    glutMouseFunc(mouseClick);

    // Keyboard callback to finalize the polygon
    glutKeyboardFunc(keyboard);

    // Display callback to draw the polygon and clipping window
    glutDisplayFunc(Draw);

    glutMainLoop();
    return 0;
}

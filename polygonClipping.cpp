#include <GL/glut.h>
#include <iostream>
#include <vector>
using namespace std;

// Clipping window coordinates
int wxmin = 200, wxmax = 500, wymin = 100, wymax = 350;

vector<pair<int, int>> points; // Stores polygon vertices (x, y)
int edgeCount = 0;             // Number of vertices in the polygon

// OpenGL initialization
void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0); // White background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480); // 2D coordinate system
}

// Draw polygon and clipping window
void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw polygon in blue
    glColor3f(0.2, 0.2, 1.0);
    glBegin(GL_POLYGON);
    for (auto &p : points)
        glVertex2i(p.first, p.second);
    glEnd();

    // Draw clipping rectangle in green
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(wxmin, wymin);
    glVertex2i(wxmax, wymin);
    glVertex2i(wxmax, wymax);
    glVertex2i(wxmin, wymax);
    glEnd();

    glFlush();
}

// ----------- Clipping Helper Function ------------
void clipPolygon(char edge)
{
    vector<pair<int, int>> newPoints;

    for (int i = 0; i < points.size(); ++i)
    {
        // Current and next point
        auto [x1, y1] = points[i];
        auto [x2, y2] = points[(i + 1) % points.size()];

        bool inside1, inside2;
        float xi, yi;

        switch (edge)
        {
        case 'l': // Left
            inside1 = x1 >= wxmin;
            inside2 = x2 >= wxmin;
            if (inside1 && inside2)
            {
                newPoints.push_back({x2, y2});
            }
            else if (inside1 && !inside2)
            {
                yi = y1 + (y2 - y1) * (wxmin - x1) / (x2 - x1);
                newPoints.push_back({wxmin, yi});
            }
            else if (!inside1 && inside2)
            {
                yi = y1 + (y2 - y1) * (wxmin - x1) / (x2 - x1);
                newPoints.push_back({wxmin, yi});
                newPoints.push_back({x2, y2});
            }
            break;
        case 'r': // Right
            inside1 = x1 <= wxmax;
            inside2 = x2 <= wxmax;
            if (inside1 && inside2)
            {
                newPoints.push_back({x2, y2});
            }
            else if (inside1 && !inside2)
            {
                yi = y1 + (y2 - y1) * (wxmax - x1) / (x2 - x1);
                newPoints.push_back({wxmax, yi});
            }
            else if (!inside1 && inside2)
            {
                yi = y1 + (y2 - y1) * (wxmax - x1) / (x2 - x1);
                newPoints.push_back({wxmax, yi});
                newPoints.push_back({x2, y2});
            }
            break;
        case 'b': // Bottom
            inside1 = y1 >= wymin;
            inside2 = y2 >= wymin;
            if (inside1 && inside2)
            {
                newPoints.push_back({x2, y2});
            }
            else if (inside1 && !inside2)
            {
                xi = x1 + (x2 - x1) * (wymin - y1) / (y2 - y1);
                newPoints.push_back({xi, wymin});
            }
            else if (!inside1 && inside2)
            {
                xi = x1 + (x2 - x1) * (wymin - y1) / (y2 - y1);
                newPoints.push_back({xi, wymin});
                newPoints.push_back({x2, y2});
            }
            break;
        case 't': // Top
            inside1 = y1 <= wymax;
            inside2 = y2 <= wymax;
            if (inside1 && inside2)
            {
                newPoints.push_back({x2, y2});
            }
            else if (inside1 && !inside2)
            {
                xi = x1 + (x2 - x1) * (wymax - y1) / (y2 - y1);
                newPoints.push_back({xi, wymax});
            }
            else if (!inside1 && inside2)
            {
                xi = x1 + (x2 - x1) * (wymax - y1) / (y2 - y1);
                newPoints.push_back({xi, wymax});
                newPoints.push_back({x2, y2});
            }
            break;
        }
    }

    points = newPoints;        // Update clipped points
    edgeCount = points.size(); // Update edge count
    draw();                    // Redraw polygon
    glutPostRedisplay();       // Request display update
}

// ----------- GLUT Callbacks ------------------

// Mouse click: Add point to polygon
void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        y = 480 - y; // Invert y for OpenGL
        points.push_back({x, y});
        edgeCount = points.size();
        draw();
    }
}

// Keyboard press: Enter = finalize polygon
void keyboard(unsigned char key, int x, int y)
{
    if (key == 13)
    { // Enter key
        cout << "Polygon finalized!\n";
    }
}

// Menu selections: call corresponding clip
void menu(int value)
{
    switch (value)
    {
    case 1:
        clipPolygon('l');
        break; // Left
    case 2:
        clipPolygon('r');
        break; // Right
    case 3:
        clipPolygon('t');
        break; // Top
    case 4:
        clipPolygon('b');
        break; // Bottom
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Simple Polygon Clipping");

    init();
    glutDisplayFunc(draw);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);

    // Right-click menu for clipping
    glutCreateMenu(menu);
    glutAddMenuEntry("Clip Left", 1);
    glutAddMenuEntry("Clip Right", 2);
    glutAddMenuEntry("Clip Top", 3);
    glutAddMenuEntry("Clip Bottom", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}

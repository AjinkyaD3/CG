#include <GL/glut.h>
#include <vector>
using namespace std;

int xmin = 200, ymin = 100, xmax = 500, ymax = 350;
vector<pair<int, int>> poly;

void init()
{
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, 640, 0, 480);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw polygon (blue)
    glColor3f(0, 0, 1);
    glBegin(GL_POLYGON);
    for (auto p : poly)
        glVertex2i(p.first, p.second);
    glEnd();

    // Draw clipping window (green)
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glEnd();

    glFlush();
}

pair<int, int> intersect(int x1, int y1, int x2, int y2, char edge)
{
    float x = 0, y = 0;
    if (edge == 'l' || edge == 'r')
    {
        int xEdge = (edge == 'l') ? xmin : xmax;
        y = y1 + (y2 - y1) * (xEdge - x1) / float(x2 - x1);
        x = xEdge;
    }
    else
    {
        int yEdge = (edge == 'b') ? ymin : ymax;
        x = x1 + (x2 - x1) * (yEdge - y1) / float(y2 - y1);
        y = yEdge;
    }
    return {int(x), int(y)};
}

void clip(char edge)
{
    vector<pair<int, int>> newPoly;
    for (int i = 0; i < poly.size(); i++)
    {
        auto [x1, y1] = poly[i];
        auto [x2, y2] = poly[(i + 1) % poly.size()];
        bool in1 = true, in2 = true;

        if (edge == 'l')
        {
            in1 = x1 >= xmin;
            in2 = x2 >= xmin;
        }
        if (edge == 'r')
        {
            in1 = x1 <= xmax;
            in2 = x2 <= xmax;
        }
        if (edge == 'b')
        {
            in1 = y1 >= ymin;
            in2 = y2 >= ymin;
        }
        if (edge == 't')
        {
            in1 = y1 <= ymax;
            in2 = y2 <= ymax;
        }

        if (in1 && in2)
            newPoly.push_back({x2, y2});
        else if (in1 && !in2)
            newPoly.push_back(intersect(x1, y1, x2, y2, edge));
        else if (!in1 && in2)
        {
            newPoly.push_back(intersect(x1, y1, x2, y2, edge));
            newPoly.push_back({x2, y2});
        }
    }
    poly = newPoly;
    draw();
    glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y)
{
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        poly.push_back({x, 480 - y});
        draw();
    }
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 'c' || key == 'C')
    {
        clip('l');
        clip('r');
        clip('t');
        clip('b');
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Polygon Clipping");
    init();
    glutDisplayFunc(draw);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

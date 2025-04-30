#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

int maxDepth;

struct Point
{
    float x, y;
};

Point getMid(Point a, Point b)
{
    return {(a.x + b.x) / 2, (a.y + b.y) / 2};
}

Point rotate(Point origin, Point p, float angleDeg)
{
    float angleRad = angleDeg * M_PI / 180;
    float s = sin(angleRad), c = cos(angleRad);

    p.x -= origin.x;
    p.y -= origin.y;

    float xnew = p.x * c - p.y * s;
    float ynew = p.x * s + p.y * c;

    return {xnew + origin.x, ynew + origin.y};
}

void drawBezier(Point p0, Point p1, Point p2, Point p3)
{
    glBegin(GL_LINE_STRIP);
    for (float t = 0; t <= 1.0; t += 0.01)
    {
        float x = pow(1 - t, 3) * p0.x +
                  3 * pow(1 - t, 2) * t * p1.x +
                  3 * (1 - t) * t * t * p2.x +
                  t * t * t * p3.x;

        float y = pow(1 - t, 3) * p0.y +
                  3 * pow(1 - t, 2) * t * p1.y +
                  3 * (1 - t) * t * t * p2.y +
                  t * t * t * p3.y;

        glVertex2f(x, y);
    }
    glEnd();
}

void bezierTree(Point start, Point ctrl1, Point ctrl2, Point end, int depth)
{
    if (depth == 0)
    {
        drawBezier(start, ctrl1, ctrl2, end);
        return;
    }

    drawBezier(start, ctrl1, ctrl2, end);

    // Create two new branches from the endpoint
    Point mid = getMid(ctrl1, ctrl2);
    Point newCtrl1 = rotate(end, mid, 30);
    Point newCtrl2 = rotate(end, mid, -30);

    Point next = {
        end.x + (end.x - start.x) * 0.5f,
        end.y + (end.y - start.y) * 0.5f};

    bezierTree(end, newCtrl1, getMid(newCtrl1, next), next, depth - 1);
    bezierTree(end, newCtrl2, getMid(newCtrl2, next), next, depth - 1);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.1, 0.2, 0.8);

    // Root bezier segment
    Point p0 = {250, 50};
    Point p1 = {260, 200};
    Point p2 = {240, 200};
    Point p3 = {250, 300};

    bezierTree(p0, p1, p2, p3, maxDepth);

    glFlush();
}

void init()
{
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char **argv)
{
    cout << "Enter recursion depth (suggest 4-6): ";
    cin >> maxDepth;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bezier Fractal Tree");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

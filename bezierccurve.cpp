#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

// Max recursion depth
int maxDepth;

// 2D Point structure
struct Point
{
    float x, y;
};

// Get midpoint between two points
Point midpoint(Point a, Point b)
{
    return {(a.x + b.x) / 2, (a.y + b.y) / 2};
}

// Rotate point 'p' around origin by 'angle' degrees
Point rotateAround(Point origin, Point p, float angleDeg)
{
    float rad = angleDeg * M_PI / 180;
    float s = sin(rad), c = cos(rad);

    // Translate point to origin
    float x = p.x - origin.x;
    float y = p.y - origin.y;

    // Rotate
    float newX = x * c - y * s;
    float newY = x * s + y * c;

    // Translate back
    return {newX + origin.x, newY + origin.y};
}

// Draw Bezier curve with 4 control points
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

// Recursive function to draw tree branches
void drawTree(Point p0, Point p1, Point p2, Point p3, int depth)
{
    drawBezier(p0, p1, p2, p3); // Draw current branch

    if (depth == 0)
        return; // Base case

    // Rotate control points to create left and right branches
    Point newP1 = rotateAround(p3, p1, 30);
    Point newP2 = rotateAround(p3, p2, -30);

    // Extend the branch upwards
    Point newP3 = {
        p3.x + (p3.x - p0.x) * 0.5f,
        p3.y + (p3.y - p0.y) * 0.5f};

    // Recursive call for two branches
    drawTree(p3, newP1, midpoint(newP1, newP3), newP3, depth - 1);
    drawTree(p3, newP2, midpoint(newP2, newP3), newP3, depth - 1);
}

// OpenGL display callback
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0.5f, 0); // Tree color

    // Initial trunk control points
    Point p0 = {250, 50};
    Point p1 = {260, 200};
    Point p2 = {240, 200};
    Point p3 = {250, 300};

    drawTree(p0, p1, p2, p3, maxDepth);

    glFlush();
}

// OpenGL initialization
void init()
{
    glClearColor(1, 1, 1, 1); // White background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500); // 2D canvas
}

int main(int argc, char **argv)
{
    cout << "Enter recursion depth (2 to 5 recommended): ";
    cin >> maxDepth;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Simplified Bezier Fractal Tree");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

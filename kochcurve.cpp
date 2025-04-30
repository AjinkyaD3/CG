#include <GL/glut.h>
#include <iostream>
#include <cmath>

using namespace std;

// Define a point structure
struct Point
{
    float x, y;
};

// Global variable to hold the number of iterations
int iterations;

// Function to interpolate between two points
Point interpolate(Point p1, Point p2, float t)
{
    return {p1.x + (p2.x - p1.x) * t, p1.y + (p2.y - p1.y) * t};
}

// Function to calculate the point that forms the peak of the triangle
Point peak(Point p1, Point p2)
{
    // The peak is calculated by finding the point that is at a 60 degree angle
    float angle = M_PI / 3;                                           // 60 degrees in radians
    float dist = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)) / 3; // 1/3rd of the distance
    float x = (p1.x + p2.x) / 2 - dist * sin(angle);
    float y = (p1.y + p2.y) / 2 + dist * cos(angle);
    return {x, y};
}

// Function to draw a Koch curve recursively
void drawKochCurve(Point p1, Point p2, int depth)
{
    if (depth == 0)
    {
        // Base case: Draw the line segment
        glBegin(GL_LINES);
        glVertex2f(p1.x, p1.y);
        glVertex2f(p2.x, p2.y);
        glEnd();
        return;
    }

    // Divide the line segment into 3 parts
    Point pA = interpolate(p1, p2, 1.0 / 3.0);
    Point pB = interpolate(p1, p2, 2.0 / 3.0);

    // Find the peak of the triangle
    Point pC = peak(pA, pB);

    // Recursively draw the smaller segments
    drawKochCurve(p1, pA, depth - 1);
    drawKochCurve(pA, pC, depth - 1);
    drawKochCurve(pC, pB, depth - 1);
    drawKochCurve(pB, p2, depth - 1);
}

// OpenGL display function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0); // Set curve color to blue

    // Define the starting points of the Koch curve (initial triangle)
    Point p1 = {100.0, 100.0};
    Point p2 = {500.0, 100.0};
    Point p3 = {300.0, 400.0};

    // Draw the three sides of the initial triangle using Koch curve
    drawKochCurve(p1, p2, iterations); // Depth of recursion (this is the default depth)
    drawKochCurve(p2, p3, iterations);
    drawKochCurve(p3, p1, iterations);

    glFlush(); // Ensure that everything is drawn
}

// OpenGL initialization
void init()
{
    glClearColor(1, 1, 1, 1); // Set background color to white
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 600, 0, 600); // Set 2D orthographic view
}

// Main function to initialize OpenGL and get iteration input
int main(int argc, char **argv)
{
    cout << "Enter the number of iterations (depth) for the Koch Curve: ";
    cin >> iterations;

    // Input validation
    if (iterations < 0)
    {
        cout << "Number of iterations must be a positive integer." << endl;
        return -1;
    }

    // Initialize OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);     // Window size
    glutInitWindowPosition(100, 100); // Window position
    glutCreateWindow("Koch Curve Fractal");

    // Initialize OpenGL settings
    init();

    // Register the display function
    glutDisplayFunc(display);

    // Start the GLUT main loop
    glutMainLoop();

    return 0;
}

#include <GL/glut.h>
#include <iostream>
#include <cmath> // For math functions like sin, cos, sqrt

using namespace std;

// Define a point in 2D space
struct Point
{
    float x, y;
};

// Global variable to store user input for depth of recursion
int iterations;

// 🟡 Function to find a point between two points (based on ratio 't')
Point interpolate(Point p1, Point p2, float t)
{
    return {
        p1.x + (p2.x - p1.x) * t, // Linear interpolation on X
        p1.y + (p2.y - p1.y) * t  // Linear interpolation on Y
    };
}

// 🔺 Function to find the top peak of the triangle (forms the Koch 'bump')
Point peak(Point p1, Point p2)
{
    float angle = M_PI / 3;                                           // 60 degrees in radians
    float dist = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)) / 3; // One-third length
    float midX = (p1.x + p2.x) / 2;
    float midY = (p1.y + p2.y) / 2;

    // Shift the midpoint upward to form the triangle peak
    float x = midX - dist * sin(angle);
    float y = midY + dist * cos(angle);

    return {x, y};
}

// 🔁 Recursive function to draw the Koch curve
void drawKochCurve(Point p1, Point p2, int depth)
{
    if (depth == 0)
    {
        // Draw a straight line between p1 and p2
        glBegin(GL_LINES);
        glVertex2f(p1.x, p1.y);
        glVertex2f(p2.x, p2.y);
        glEnd();
        return;
    }

    // 🔹 Break the line into 4 parts:
    Point A = interpolate(p1, p2, 1.0 / 3.0); // First 1/3rd
    Point B = interpolate(p1, p2, 2.0 / 3.0); // Second 2/3rd
    Point C = peak(A, B);                     // Peak point to form triangle bump

    // 🔁 Recursively draw the 4 smaller segments
    drawKochCurve(p1, A, depth - 1); // First segment
    drawKochCurve(A, C, depth - 1);  // Left triangle side
    drawKochCurve(C, B, depth - 1);  // Right triangle side
    drawKochCurve(B, p2, depth - 1); // Last segment
}

// 🎨 Display function to draw the full Koch Snowflake
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear screen
    glColor3f(0.0, 0.0, 1.0);     // Set drawing color to blue

    // 🔺 Initial triangle points (equilateral)
    Point p1 = {100.0, 100.0};
    Point p2 = {500.0, 100.0};
    Point p3 = {300.0, 400.0};

    // Draw Koch curve on each side of the triangle
    drawKochCurve(p1, p2, iterations);
    drawKochCurve(p2, p3, iterations);
    drawKochCurve(p3, p1, iterations);

    glFlush(); // Show everything on screen
}

// 🔧 OpenGL setup function
void init()
{
    glClearColor(1, 1, 1, 1);    // Set background color to white
    glMatrixMode(GL_PROJECTION); // Set projection mode
    gluOrtho2D(0, 600, 0, 600);  // Define the coordinate system (2D)
}

// 🚀 Main function
int main(int argc, char **argv)
{
    // Ask user how many levels deep they want to draw
    cout << "Enter the number of iterations (depth) for the Koch Curve: ";
    cin >> iterations;

    // If the input is invalid (negative), exit
    if (iterations < 0)
    {
        cout << "Please enter a non-negative number.\n";
        return -1;
    }

    // Initialize GLUT (OpenGL Utility Toolkit)
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Single buffer + RGB color mode
    glutInitWindowSize(600, 600);                // Window size
    glutInitWindowPosition(100, 100);            // Window position on screen
    glutCreateWindow("Koch Curve Fractal");      // Window title

    init();                   // Call OpenGL init function
    glutDisplayFunc(display); // Register display callback
    glutMainLoop();           // Enter OpenGL event loop (runs until window closes)

    return 0;
}

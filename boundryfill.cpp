#include <iostream>
#include <math.h>
#include <GL/glut.h>

using namespace std;

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0); // White background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480); // Set up orthogonal projection
}

// Helper function for floating-point comparison with tolerance
bool isColorEqual(float* color1, float* color2, float tolerance = 0.1f) {
    return (abs(color1[0] - color2[0]) < tolerance &&
            abs(color1[1] - color2[1]) < tolerance &&
            abs(color1[2] - color2[2]) < tolerance);
}

// Boundary Fill Algorithm (Recursive)
void bound_it(int x, int y, float* fillColor, float* bc) {
    float color[3];

    // Read the color of the current pixel at (x, y)
    glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, color);
    
    // Check if the pixel is not part of the boundary or already filled with fill color
    if (!isColorEqual(color, bc) && !isColorEqual(color, fillColor)) {
        // Fill the pixel with the desired color
        glColor3f(fillColor[0], fillColor[1], fillColor[2]);
        glBegin(GL_POINTS);
            glVertex2i(x, y);
        glEnd();
        glFlush();
        
        // Recursive calls to neighboring pixels (smaller step size)
        bound_it(x + 1, y, fillColor, bc);
        bound_it(x - 1, y, fillColor, bc);
        bound_it(x, y + 1, fillColor, bc);
        bound_it(x, y - 1, fillColor, bc);
    }
}

// Mouse click handler
void mouse(int btn, int state, int x, int y) {
    // Convert from window coordinates to OpenGL coordinates (flip y-axis)
    int invertedY = 480 - y;
    
    // Define the boundary color (blue) and fill color (green)
    float bCol[] = {0, 0, 1}; // Boundary color (blue)
    float color[] = {0, 1, 0}; // Fill color (green)
    
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        cout << "Mouse clicked at x: " << x << ", y: " << invertedY << endl;
        bound_it(x, invertedY, color, bCol); // Start filling from the clicked point
    }
}

// Function to draw a triangle
void world() {
    glLineWidth(3);
    glPointSize(2);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 1); // Boundary color (blue)
    
    glBegin(GL_LINE_LOOP);
        glVertex2i(150, 100);  // Vertex 1
        glVertex2i(300, 300);  // Vertex 2
        glVertex2i(450, 100);  // Vertex 3
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Boundary Fill Algorithm");
    init();
    
    glutDisplayFunc(world);  // Draw the triangle
    glutMouseFunc(mouse);    // Mouse click to start filling
    
    glutMainLoop();
    return 0;
}

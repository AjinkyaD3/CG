#include <GL/glut.h>
#include <iostream>
#include <queue>
using namespace std;

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);  // Set background to white
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480);  // Set the coordinate system
}

// Function to set the pixel color at (x, y)
void setPixel(int x, int y, float color[3]) {
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

// Function to check if the point is inside the triangle using a simple area method
bool pointInTriangle(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3) {
    float A = abs(x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2));
    float A1 = abs(px*(y2 - y3) + x2*(y3 - py) + x3*(py - y2));
    float A2 = abs(x1*(py - y3) + px*(y3 - y1) + x3*(y1 - py));
    float A3 = abs(x1*(y2 - py) + x2*(py - y1) + px*(y1 - y2));
    
    return (A == A1 + A2 + A3);
}

// Iterative flood fill using a queue
void floodFill(int startX, int startY, float* newColor) {
    queue<pair<int, int>> q;
    bool visited[640][480] = {false};  // To avoid re-visiting pixels
    
    // Start the flood fill from the clicked point
    q.push({startX, startY});
    visited[startX][startY] = true;

    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();

        int x = current.first;
        int y = current.second;

        // If the point is inside the triangle, fill it
        if (pointInTriangle(x, y, 300, 100, 300, 300, 450, 100)) {
            setPixel(x, y, newColor);
            
            // Push the neighboring pixels to the queue
            if (x + 1 < 640 && !visited[x + 1][y]) {
                q.push({x + 1, y});
                visited[x + 1][y] = true;
            }
            if (x - 1 >= 0 && !visited[x - 1][y]) {
                q.push({x - 1, y});
                visited[x - 1][y] = true;
            }
            if (y + 1 < 480 && !visited[x][y + 1]) {
                q.push({x, y + 1});
                visited[x][y + 1] = true;
            }
            if (y - 1 >= 0 && !visited[x][y - 1]) {
                q.push({x, y - 1});
                visited[x][y - 1] = true;
            }
        }
    }
}

// Mouse click handler
void onMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float newColor[] = {0, 1, 0};  // Green color to fill
        cout << "x is : " << x << " y is : " << y << endl;
        floodFill(x, 480 - y, newColor);  // Convert y from OpenGL's origin (bottom-left)
    }
}

// Draw a simple triangle
void draw_triangle() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);  // Black color for the triangle outline
    glBegin(GL_LINE_LOOP);
    glVertex2i(300, 100);  // Vertex 1
    glVertex2i(300, 300);  // Vertex 2
    glVertex2i(450, 100);  // Vertex 3
    glEnd();
    glFlush();
}

// Display function
void display(void) {
    draw_triangle();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Flood Fill Algorithm");
    init();
    
    glutDisplayFunc(display);
    glutMouseFunc(onMouseClick);
    
    glutMainLoop();
    return 0;
}

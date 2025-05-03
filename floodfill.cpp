#include <GL/glut.h> // For OpenGL functions
#include <queue>     // For queue used in flood fill
using namespace std;

bool visited[640][480] = {false}; // Keeps track of filled pixels

// Initializes OpenGL window
void init()
{
    glClearColor(1, 1, 1, 1); // Set white background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480); // Set 2D coordinate system
}

// Draws a triangle boundary
void drawTriangle()
{
    glColor3f(0, 0, 0);    // Black triangle
    glBegin(GL_LINE_LOOP); // Draw outline of triangle
    glVertex2i(300, 100);
    glVertex2i(300, 300);
    glVertex2i(450, 100);
    glEnd();
    glFlush(); // Display immediately
}

// Sets pixel color at (x, y)
void setPixel(int x, int y, float r, float g, float b)
{
    glColor3f(r, g, b); // Set desired fill color
    glBegin(GL_POINTS);
    glVertex2i(x, y); // Plot the point
    glEnd();
    glFlush();
}

// Simple check if (x,y) lies inside a fixed triangle using bounding box (faster than area check)
bool insideTriangle(int x, int y)
{
    return x >= 300 && x <= 450 && y >= 100 && y <= 300 &&
           (x + y) <= 600; // Rough diagonal check to shape triangle
}

// Basic 4-directional queue-based Flood Fill
void floodFill(int x, int y, float r, float g, float b)
{
    queue<pair<int, int>> q;
    q.push({x, y});
    visited[x][y] = true;

    while (!q.empty())
    {
        auto [cx, cy] = q.front();
        q.pop();
        if (!insideTriangle(cx, cy))
            continue;

        setPixel(cx, cy, r, g, b);

        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};

        for (int i = 0; i < 4; i++)
        {
            int nx = cx + dx[i], ny = cy + dy[i];
            if (nx >= 0 && nx < 640 && ny >= 0 && ny < 480 && !visited[nx][ny])
            {
                q.push({nx, ny});
                visited[nx][ny] = true;
            }
        }
    }
}

// Handles mouse click to start flood fill
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        int fy = 480 - y;                // Invert y-axis for OpenGL
        floodFill(x, fy, 0.0, 1.0, 0.0); // Fill with green
    }
}

// Display function to draw triangle
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawTriangle();
}

// Main function: sets up window and loop
int main(int argc, char **argv)
{
    glutInit(&argc, argv);                       // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode
    glutInitWindowSize(640, 480);                // Window size
    glutCreateWindow("Flood Fill Simple");       // Window title
    init();                                      // Set up OpenGL
    glutDisplayFunc(display);                    // Draw callback
    glutMouseFunc(mouse);                        // Mouse click callback
    glutMainLoop();                              // Enter loop
    return 0;
}

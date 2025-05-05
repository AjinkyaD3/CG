#include <GL/glut.h>
#include <queue> // For flood fill using queue
using namespace std;

bool visited[640][480] = {false}; // To track filled pixels

// OpenGL initialization
void init()
{
    glClearColor(1, 1, 1, 1);   // Set background to white
    gluOrtho2D(0, 640, 0, 480); // Define 2D drawing area
}

// Draws a triangle using lines
void drawTriangle()
{
    glColor3f(0, 0, 0);    // Black color for outline
    glBegin(GL_LINE_LOOP); // Start drawing a closed shape
    glVertex2i(300, 100);  // Bottom left
    glVertex2i(300, 300);  // Top left
    glVertex2i(500, 100);  // Bottom right
    glEnd();
    glFlush(); // Show immediately
}

// Plot a single point at (x, y) with given color
void setPixel(int x, int y, float r, float g, float b)
{
    glColor3f(r, g, b); // Set color
    glBegin(GL_POINTS); // Draw a point
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

// Check if (x, y) is inside the triangle's bounding box
bool isInsideTriangle(int x, int y)
{
    // Rough bounds and diagonal line check
    return x >= 300 && x <= 500 && y >= 100 && y <= 300 && (x + y <= 600);
}

// Flood fill using a queue (BFS approach)
void floodFill(int x, int y, float r, float g, float b)
{
    queue<pair<int, int>> q;
    q.push({x, y});
    visited[x][y] = true;

    while (!q.empty())
    {
        int cx = q.front().first;
        int cy = q.front().second;
        q.pop();

        // Only fill if inside triangle shape
        if (!isInsideTriangle(cx, cy))
            continue;

        setPixel(cx, cy, r, g, b); // Fill the pixel with color

        // Check 4 directions (up, down, left, right)
        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};

        for (int i = 0; i < 4; i++)
        {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            // Only process if not visited and inside screen
            if (nx >= 0 && nx < 640 && ny >= 0 && ny < 480 && !visited[nx][ny])
            {
                q.push({nx, ny});
                visited[nx][ny] = true;
            }
        }
    }
}

// Handles mouse clicks
void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        int fy = 480 - y;                // Convert to OpenGL coordinate (invert y)
        floodFill(x, fy, 0.0, 1.0, 0.0); // Start filling with green color
    }
}

// Display function for drawing shapes
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
    drawTriangle();               // Draw triangle outline
}

// Main function to set up OpenGL
int main(int argc, char **argv)
{
    glutInit(&argc, argv);                         // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // Use single buffer and RGB mode
    glutInitWindowSize(640, 480);                  // Set window size
    glutCreateWindow("Flood Fill - Simple Logic"); // Window title
    init();                                        // Set background and projection
    glutDisplayFunc(display);                      // Call display() on startup
    glutMouseFunc(mouseClick);                     // Handle mouse click
    glutMainLoop();                                // Run the OpenGL loop
    return 0;
}

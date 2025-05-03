#include <GL/glut.h>  // OpenGL header
#include <cmath>      // For abs and round

int x1_in, y1_in, x2_in, y2_in, clicks = 0;  // Mouse click positions

// Draw one point
void plot(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// DDA line drawing
void DDA(int x1, int y1, int x2, int y2) {
    float dx = x2 - x1, dy = y2 - y1;
    int steps = std::max(abs(dx), abs(dy));
    float x = x1, y = y1, xInc = dx / steps, yInc = dy / steps;
    for (int i = 0; i <= steps; i++, x += xInc, y += yInc)
        plot(round(x), round(y));
}

// Display the coordinate axes and the line
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);  // White axes

    glBegin(GL_LINES);
    glVertex2i(-500, 0); glVertex2i(500, 0);   // X-axis
    glVertex2i(0, -500); glVertex2i(0, 500);   // Y-axis
    glEnd();

    if (clicks == 2) {
        glColor3f(1, 0, 0);                   // Red line
        DDA(x1_in, y1_in, x2_in, y2_in);
    }

    glFlush();
}

// Get two mouse clicks to define the line
void mouse(int btn, int state, int x, int y) {
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (clicks == 0) {
            x1_in = x - 400; y1_in = 400 - y;
        } else {
            x2_in = x - 400; y2_in = 400 - y;
            glutPostRedisplay();  // Trigger display
        }
        clicks++;
    }
}

// Setup background and projection
void init() {
    glClearColor(0, 0, 0, 1);                 // Black background
    gluOrtho2D(-500, 500, -500, 500);         // 2D projection
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);                   // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("DDA Line Drawing");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}

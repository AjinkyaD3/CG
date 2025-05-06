// 5-Step Quick Code Memory Trick
// 🧱 1. Region Code Generator → getCode(x, y)
// 🧠 Trick: Use |= for:

// Left x < xmin → 1

// Right x > xmax → 2

// Bottom y < ymin → 4

// Top y > ymax → 8

// 📌 Order = L R B T → (1 2 4 8)

// 🔄 2. Clipping Loop → clipLine()
// 🧠 Trick: Use OR and AND:

// If (code1 | code2) == 0 → Accept

// If (code1 & code2) != 0 → Reject

// Else → Clip at boundary

// 📌 Order of clipping:
// Top → Bottom → Right → Left
// (TBRL with if (outCode & …))

// ✂️ 3. Intersect at Boundaries
// 🧠 Trick:

// For Top/Bottom: change y = ymax/ymin, solve for x

// For Left/Right: change x = xmin/xmax, solve for y

// 📌 H → x, V → y

// 🖱️ 4. Mouse Handler → mouse()
// First click = start point

// Second click = end point → Draw line

// Y is flipped: y = 480 - y

// ⌨️ 5. Keyboard Control → keyboard()
// Press 'c' → clip

// Press 'r' → reset

// getCode() → generate region code using L, R, B, T
// clipLine() → apply OR/AND logic, intersect if needed
// mouse() → get 2 points on click
// keyboard() → 'c' to clip, 'r' to reset
// init() → white bg, black draw, draw rect
// main() → GLUT init, set handlers, loop


#include <GL/glut.h>

// Clipping window boundaries
int xmin = 100, xmax = 300, ymin = 100, ymax = 300;

// Line endpoints
float x_start, y_start, x_end, y_end;
int clickCount = 0;

// Region code generator
int getCode(float x, float y)
{
    int code = 0;
    if (x < xmin) code |= 1;  // Left
    if (x > xmax) code |= 2;  // Right
    if (y < ymin) code |= 4;  // Bottom
    if (y > ymax) code |= 8;  // Top
    return code;
}

// Draw clipping rectangle
void drawClippingRectangle()
{
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glEnd();
}

// Cohen-Sutherland Line Clipping
void clipLine()
{
    int code1 = getCode(x_start, y_start);
    int code2 = getCode(x_end, y_end);
    bool accept = false;

    while (true)
    {
        if ((code1 | code2) == 0) // Trivially accept
        {
            accept = true;
            break;
        }
        else if (code1 & code2) // Trivially reject
        {
            break;
        }
        else
        {
            float x, y;
            int outCode = code1 ? code1 : code2;

            if (outCode & 8) // Top
            {
                x = x_start + (x_end - x_start) * (ymax - y_start) / (y_end - y_start);
                y = ymax;
            }
            else if (outCode & 4) // Bottom
            {
                x = x_start + (x_end - x_start) * (ymin - y_start) / (y_end - y_start);
                y = ymin;
            }
            else if (outCode & 2) // Right
            {
                y = y_start + (y_end - y_start) * (xmax - x_start) / (x_end - x_start);
                x = xmax;
            }
            else // Left
            {
                y = y_start + (y_end - y_start) * (xmin - x_start) / (x_end - x_start);
                x = xmin;
            }

            if (outCode == code1)
            {
                x_start = x;
                y_start = y;
                code1 = getCode(x_start, y_start);
            }
            else
            {
                x_end = x;
                y_end = y;
                code2 = getCode(x_end, y_end);
            }
        }
    }

    glClear(GL_COLOR_BUFFER_BIT);
    drawClippingRectangle();

    if (accept)
    {
        glBegin(GL_LINES);
        glVertex2f(x_start, y_start);
        glVertex2f(x_end, y_end);
        glEnd();
    }

    glFlush();
}

// Mouse click to get line endpoints
void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        if (clickCount == 0)
        {
            x_start = x;
            y_start = 480 - y; // OpenGL Y-axis inversion
            clickCount = 1;
        }
        else if (clickCount == 1)
        {
            x_end = x;
            y_end = 480 - y;
            clickCount = 2;

            glClear(GL_COLOR_BUFFER_BIT);
            drawClippingRectangle();

            // Draw original line
            glBegin(GL_LINES);
            glVertex2f(x_start, y_start);
            glVertex2f(x_end, y_end);
            glEnd();

            glFlush();
        }
    }
}

// Keyboard controls: 'c' = clip, 'r' = reset
void keyboard(unsigned char key, int x, int y)
{
    if (key == 'c' && clickCount == 2)
    {
        clipLine();
    }
    else if (key == 'r')
    {
        clickCount = 0;
        glClear(GL_COLOR_BUFFER_BIT);
        drawClippingRectangle();
        glFlush();
    }
}

// OpenGL initialization
void init()
{
    glClearColor(1, 1, 1, 1); // White background
    gluOrtho2D(0, 640, 0, 480);
    glColor3f(0, 0, 0);       // Black color
    glClear(GL_COLOR_BUFFER_BIT);
    drawClippingRectangle();
    glFlush();
}

// Main function
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Cohen-Sutherland Line Clipping");

    init();

    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}

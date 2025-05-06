#include <GL/glut.h>

int xmin = 100, xmax = 300, ymin = 100, ymax = 300;
float x1, y1, x2, y2;
int click = 0;

int getCode(float x, float y)
{
    int code = 0;
    if (x < xmin)
        code |= 1;
    if (x > xmax)
        code |= 2;
    if (y < ymin)
        code |= 4;
    if (y > ymax)
        code |= 8;
    return code;
}

void drawRect()
{
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void clipLine()
{
    int code1 = getCode(x1, y1), code2 = getCode(x2, y2);
    bool accept = false;

    while (true)
    {
        if ((code1 | code2) == 0)
        {
            accept = true;
            break;
        }
        else if (code1 & code2)
            break;
        else
        {
            float x, y;
            int out = code1 ? code1 : code2;

            if (out & 8)
            {
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            }
            else if (out & 4)
            {
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            }
            else if (out & 2)
            {
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            }
            else
            {
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (out == code1)
            {
                x1 = x;
                y1 = y;
                code1 = getCode(x1, y1);
            }
            else
            {
                x2 = x;
                y2 = y;
                code2 = getCode(x2, y2);
            }
        }
    }

    glClear(GL_COLOR_BUFFER_BIT);
    drawRect();
    if (accept)
        drawLine(x1, y1, x2, y2);
    glFlush();
}

void mouse(int btn, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        if (click == 0)
        {
            x1 = x;
            y1 = 480 - y;
            click = 1;
        }
        else
        {
            x2 = x;
            y2 = 480 - y;
            click = 2;
            glClear(GL_COLOR_BUFFER_BIT);
            drawRect();
            drawLine(x1, y1, x2, y2);
            glFlush();
        }
    }
}

void keyboard(unsigned char key, int, int)
{
    if (key == 'c' && click == 2)
        clipLine();
    else if (key == 'r')
    {
        click = 0;
        glClear(GL_COLOR_BUFFER_BIT);
        drawRect();
        glFlush();
    }
}

void init()
{
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 640, 0, 480);
    glColor3f(0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    drawRect();
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Cohen-Sutherland Clipping");
    init();
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

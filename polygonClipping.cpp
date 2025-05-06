#include <GL/glut.h>
#include <vector>
using namespace std;

// Clipping window
int wxmin = 200, wymin = 100, wxmax = 500, wymax = 350;
vector<pair<int, int>> points;

void init() {
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, 640, 0, 480);
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw polygon
    glColor3f(0, 0, 1);
    glBegin(GL_POLYGON);
    for (auto p : points) glVertex2i(p.first, p.second);
    glEnd();

    // Draw clipping rectangle
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(wxmin, wymin); glVertex2i(wxmax, wymin);
    glVertex2i(wxmax, wymax); glVertex2i(wxmin, wymax);
    glEnd();

    glFlush();
}

// Generic edge clipping
void clip(char edge) {
    vector<pair<int, int>> newPts;
    for (int i = 0; i < points.size(); ++i) {
        int x1 = points[i].first, y1 = points[i].second;
        int x2 = points[(i+1)%points.size()].first, y2 = points[(i+1)%points.size()].second;
        bool in1, in2;
        float xi, yi;

        switch(edge) {
            case 'l': in1 = x1 >= wxmin; in2 = x2 >= wxmin; break;
            case 'r': in1 = x1 <= wxmax; in2 = x2 <= wxmax; break;
            case 'b': in1 = y1 >= wymin; in2 = y2 >= wymin; break;
            case 't': in1 = y1 <= wymax; in2 = y2 <= wymax; break;
        }

        if (in1 && in2)
            newPts.push_back({x2, y2});
        else if (in1 && !in2) {
            if (edge == 'l' || edge == 'r')
                yi = y1 + (y2 - y1) * ((edge=='l'?wxmin:wxmax) - x1) / (x2 - x1),
                newPts.push_back({(edge=='l'?wxmin:wxmax), yi});
            else
                xi = x1 + (x2 - x1) * ((edge=='b'?wymin:wymax) - y1) / (y2 - y1),
                newPts.push_back({xi, (edge=='b'?wymin:wymax)});
        }
        else if (!in1 && in2) {
            if (edge == 'l' || edge == 'r')
                yi = y1 + (y2 - y1) * ((edge=='l'?wxmin:wxmax) - x1) / (x2 - x1),
                newPts.push_back({(edge=='l'?wxmin:wxmax), yi});
            else
                xi = x1 + (x2 - x1) * ((edge=='b'?wymin:wymax) - y1) / (y2 - y1),
                newPts.push_back({xi, (edge=='b'?wymin:wymax)});
            newPts.push_back({x2, y2});
        }
    }

    points = newPts;
    draw();
    glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y) {
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        points.push_back({x, 480 - y});
        draw();
    }
}

void menu(int opt) {
    if (opt == 1) clip('l');
    else if (opt == 2) clip('r');
    else if (opt == 3) clip('t');
    else if (opt == 4) clip('b');
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Simple Clipping");
    init();
    glutDisplayFunc(draw);
    glutMouseFunc(mouse);
    glutCreateMenu(menu);
    glutAddMenuEntry("Left", 1);
    glutAddMenuEntry("Right", 2);
    glutAddMenuEntry("Top", 3);
    glutAddMenuEntry("Bottom", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
    return 0;
}

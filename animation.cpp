#include <GL/glut.h> // OpenGL Utility Toolkit
#include <iostream>  // Console I/O
#include <cmath>     // Math functions

using namespace std;

// Window size
const int WIDTH = 640;
const int HEIGHT = 480;

// Ball properties
float ballX = WIDTH / 2;  // Ball's X position
float ballY = HEIGHT / 2; // Ball's Y position
float ballRadius = 20;    // Ball's radius
float speedX = 2.0;       // Horizontal velocity
float speedY = 0.0;       // Vertical velocity
float gravity = 0.2;      // Gravity force
float friction = 0.99;    // Friction coefficient (slows down X movement)
float bounce = 0.8;       // Elasticity on bounce

// Initialize OpenGL environment
void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0); // Set white background
    glMatrixMode(GL_PROJECTION);      // Switch to projection matrix
    gluOrtho2D(0, WIDTH, 0, HEIGHT);  // Set 2D coordinate system
}

// Draw the ball at (ballX, ballY)
void drawBall()
{
    glColor3f(1.0, 0.0, 0.0); // Ball color: red
    glBegin(GL_TRIANGLE_FAN); // Circle drawing using triangle fan
    glVertex2f(ballX, ballY); // Center of the circle

    for (int angle = 0; angle <= 360; angle += 10)
    {
        float rad = angle * M_PI / 180.0; // Convert to radians
        float x = ballX + ballRadius * cos(rad);
        float y = ballY + ballRadius * sin(rad);
        glVertex2f(x, y);
    }

    glEnd();
}

// Update ball position and handle physics
void updateBall()
{
    // Apply gravity
    speedY -= gravity;

    // Apply friction to horizontal motion
    speedX *= friction;

    // Update position
    ballX += speedX;
    ballY += speedY;

    // Check collisions with window boundaries and bounce
    if (ballX - ballRadius < 0)
    {
        ballX = ballRadius;
        speedX = -speedX * bounce;
    }
    else if (ballX + ballRadius > WIDTH)
    {
        ballX = WIDTH - ballRadius;
        speedX = -speedX * bounce;
    }

    if (ballY - ballRadius < 0)
    {
        ballY = ballRadius;
        speedY = -speedY * bounce;
    }
    else if (ballY + ballRadius > HEIGHT)
    {
        ballY = HEIGHT - ballRadius;
        speedY = -speedY * bounce;
    }
}

// Timer callback for animation (~60 FPS)
void timer(int value)
{
    updateBall();                // Update position and check collisions
    glutPostRedisplay();         // Redraw the screen
    glutTimerFunc(16, timer, 0); // Call timer again after 16ms (~60 FPS)
}

// Display function - renders the frame
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear screen
    drawBall();                   // Draw ball
    glFlush();                    // Render now
}

// Mouse click to set ball's position and launch
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        ballX = x;
        ballY = HEIGHT - y; // Invert Y since OpenGL origin is bottom-left
        speedX = 5.0;
        speedY = 5.0;
    }
}

// Press 'R' to reset the ball
void keyboard(unsigned char key, int x, int y)
{
    if (key == 'r' || key == 'R')
    {
        ballX = WIDTH / 2;
        ballY = HEIGHT / 2;
        speedX = 0;
        speedY = 0;
    }
}

// Main program entry point
int main(int argc, char **argv)
{
    glutInit(&argc, argv);                       // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode
    glutInitWindowSize(WIDTH, HEIGHT);           // Window size
    glutInitWindowPosition(200, 200);            // Window position
    glutCreateWindow("Simple Bouncing Ball");    // Create window

    init(); // Set up OpenGL environment

    // Register callback functions
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0); // Start the timer

    glutMainLoop(); // Start event loop

    return 0;
}

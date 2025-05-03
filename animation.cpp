/**
 * Animation Implementation
 * This program demonstrates basic animation techniques using OpenGL.
 * It shows a bouncing ball with physics-based motion and collision detection.
 */

#include <GL/glut.h>  // OpenGL Utility Toolkit
#include <iostream>   // For console I/O
#include <cmath>      // For mathematical functions

using namespace std;

// Window dimensions
const int WIDTH = 640;
const int HEIGHT = 480;

// Ball properties
float ballX = WIDTH / 2;      // Initial x position
float ballY = HEIGHT / 2;     // Initial y position
float ballRadius = 20;        // Ball radius
float ballSpeedX = 2;         // Horizontal speed
float ballSpeedY = 0;         // Vertical speed
float gravity = 0.2;          // Gravity acceleration
float friction = 0.99;        // Friction coefficient
float elasticity = 0.8;       // Bounce elasticity

/**
 * Initialize OpenGL settings
 */
void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);  // White background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
}

/**
 * Draw the ball
 */
void drawBall() {
    glColor3f(1, 0, 0);  // Red color
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ballX, ballY);  // Center point
    
    // Draw circle using triangles
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * M_PI / 180;
        float x = ballX + ballRadius * cos(angle);
        float y = ballY + ballRadius * sin(angle);
        glVertex2f(x, y);
    }
    
    glEnd();
}

/**
 * Update ball position and handle collisions
 */
void updateBall() {
    // Update position
    ballX += ballSpeedX;
    ballY += ballSpeedY;
    
    // Apply gravity
    ballSpeedY -= gravity;
    
    // Apply friction
    ballSpeedX *= friction;
    
    // Check for collisions with walls
    if (ballX - ballRadius < 0) {
        ballX = ballRadius;
        ballSpeedX = -ballSpeedX * elasticity;
    }
    else if (ballX + ballRadius > WIDTH) {
        ballX = WIDTH - ballRadius;
        ballSpeedX = -ballSpeedX * elasticity;
    }
    
    if (ballY - ballRadius < 0) {
        ballY = ballRadius;
        ballSpeedY = -ballSpeedY * elasticity;
    }
    else if (ballY + ballRadius > HEIGHT) {
        ballY = HEIGHT - ballRadius;
        ballSpeedY = -ballSpeedY * elasticity;
    }
}

/**
 * Timer function for animation
 * @param value: Timer value
 */
void timer(int value) {
    updateBall();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);  // 60 FPS (1000ms/60 ≈ 16ms)
}

/**
 * Display function
 * Called whenever the window needs to be redrawn
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw the ball
    drawBall();
    
    glFlush();
}

/**
 * Mouse click handler
 * Launches the ball when left mouse button is clicked
 * @param btn: Mouse button pressed
 * @param state: Button state (up/down)
 * @param x, y: Mouse coordinates
 */
void mouse(int btn, int state, int x, int y) {
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Convert from window coordinates to OpenGL coordinates
        int invertedY = HEIGHT - y;
        
        // Set ball position to click location
        ballX = x;
        ballY = invertedY;
        
        // Set initial velocity
        ballSpeedX = 5;
        ballSpeedY = 5;
    }
}

/**
 * Keyboard function
 * Handles keyboard input for resetting the ball
 * @param key: Key pressed
 * @param x, y: Mouse coordinates
 */
void keyboard(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        // Reset ball position and velocity
        ballX = WIDTH / 2;
        ballY = HEIGHT / 2;
        ballSpeedX = 0;
        ballSpeedY = 0;
    }
}

/**
 * Main function
 * Entry point of the program
 */
int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    
    // Set display mode (single buffer and RGB mode)
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    // Set window size and position
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(200, 200);
    
    // Create window with title
    glutCreateWindow("Bouncing Ball Animation");
    
    // Initialize OpenGL settings
    init();
    
    // Register callback functions
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    
    // Enter the GLUT event processing loop
    glutMainLoop();
    
    return 0;
}

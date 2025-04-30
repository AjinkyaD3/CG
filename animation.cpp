#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Global variables
float ballX = 320.0f;      // Ball's initial X position
float ballY = 240.0f;      // Ball's initial Y position
float ballRadius = 20.0f;  // Ball's radius
float velocityY = 0.0f;    // Ball's velocity in Y direction
float gravity = -0.2f;     // Gravity effect on the ball
float bounceFactor = 0.8f; // Damping factor for bounce (simulating energy loss)
float maxVelocity = 15.0f; // Maximum speed of the ball (ease in/ease out)
bool movingDown = true;    // Flag to check if the ball is moving down

// Function to initialize OpenGL
void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color (white)
    glMatrixMode(GL_PROJECTION);          // Set projection mode
    gluOrtho2D(0, 640, 0, 480);           // Set 2D orthographic view
}

// Function to display the ball (draw a circle)
void drawBall(float x, float y, float radius)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        float angle = i * 3.14159f / 180.0f; // Convert angle to radians
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}

// Function to update the ball's position (animation logic)
void update()
{
    // Apply gravity (simulating acceleration downwards)
    if (movingDown)
    {
        velocityY += gravity; // Increasing velocity downwards
    }
    else
    {
        velocityY -= gravity; // Decreasing velocity upwards (bounce effect)
    }

    // Update ball's Y position
    ballY += velocityY;

    // Implement squash and stretch: Increase radius when ball hits the ground
    if (ballY - ballRadius <= 0)
    {
        ballY = ballRadius;        // Prevent the ball from going below the ground
        movingDown = false;        // Ball is moving upwards after bounce
        velocityY *= bounceFactor; // Apply bounce effect (less energy)
    }

    // If the ball goes up and slows down, it should stop and reverse direction
    if (ballY >= 240.0f && !movingDown)
    {
        movingDown = true; // Ball starts moving down again after peak
    }

    // If the ball's velocity becomes too slow, stop the animation (optional)
    if (fabs(velocityY) < 0.1f && !movingDown)
    {
        velocityY = 0.0f;
    }

    glutPostRedisplay(); // Request to update the display again (for animation)
}

// Display function to draw the ball and update the screen
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

    // Draw the ball with its current position
    glColor3f(0.0f, 0.0f, 1.0f);        // Set ball color (blue)
    drawBall(ballX, ballY, ballRadius); // Draw ball

    glFlush();         // Render the drawing
    glutSwapBuffers(); // Swap buffers for double buffering
}

// Timer function to manage frame rate and animation speed
void timer(int value)
{
    update();                    // Update ball position
    glutTimerFunc(16, timer, 0); // Call timer every 16 milliseconds (60 FPS)
}

// Main function to set up the window and start the animation
int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Double buffering for smooth animation
    glutInitWindowSize(640, 480);                // Set window size
    glutCreateWindow("Animation Principles - Bouncing Ball");

    // Initialize OpenGL settings
    init();

    // Register display and timer functions
    glutDisplayFunc(display);
    glutTimerFunc(16, timer, 0); // Call timer to update animation

    // Start the GLUT main loop (this keeps the window open)
    glutMainLoop();

    return 0;
}

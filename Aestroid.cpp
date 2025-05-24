#include <string>
#include <iostream>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.14159265
#define MAX_ASTEROIDS 25
#define MIN_ASTEROIDS 4
#define MAX_BULLETS 25
#define NUM_OF_LEVELS 10
#define WIDTH 800
#define HEIGHT 600
#define SHIPSPEED 5
#define MAX_BULLETS_PER_LEVEL 20

using namespace std;

static int level = 0;
static int score = 0;
static int levelAsteroids = MIN_ASTEROIDS;
GLboolean isAnimating = true;
GLboolean initial = true;
GLboolean change = false;
GLint nOfAsteroids = 0;
GLboolean outOfBullets = false;

GLboolean grid = true;

GLubyte fly[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1f, 0x80, 0x1f, 0xc0, 0x0f, 0xc0, 0x3f, 0x80,
    0x03, 0xf5, 0xff, 0x00, 0x07, 0xfd, 0xff, 0xf8,
    0x1f, 0xfc, 0xff, 0x08, 0xff, 0x03, 0xbf, 0x70,
    0x03, 0x10, 0x40, 0x40, 0x08, 0x88, 0x8c, 0x20,
    0x05, 0x05, 0x04, 0x40, 0x02, 0x82, 0x14, 0x40,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
};

GLint angle = 0;
int bulletsCount = 0;
int bulletsLeft = MAX_BULLETS_PER_LEVEL;

//==================================Classes==================================//

class Asteroid {
public:
    GLdouble centerX, centerY, radius;
    GLint red, green, blue;
    GLint numOfSides;
    GLdouble velocityX, velocityY;

    Asteroid() {}
    Asteroid(double radius, double centerX, double centerY) {
        this->radius = radius;
        this->centerX = centerX;
        this->centerY = centerY;
        this->red = rand() % 255;
        this->green = rand() % 255;
        this->blue = rand() % 255;
        this->numOfSides = rand() % 3 + 5;
        // Assign random velocity
        double angle = (rand() % 360) * PI / 180.0;
        double speed = ((rand() % 30) / 100.0) + 0.1; // 0.1 to 0.4
        velocityX = cos(angle) * speed;
        velocityY = sin(angle) * speed;
    }
};

class Bullet {
public:
    GLdouble movementX, movementY, x, y;
    Bullet() {}
    Bullet(double angle) {
        this->x = 0;
        this->y = 0;
        this->movementX = cos(angle * PI / 180);
        this->movementY = sin(angle * PI / 180);
    }
};

Bullet* bullets[MAX_BULLETS] = { nullptr };
Asteroid* asteroids[MAX_ASTEROIDS] = { nullptr };

// Function declarations
void shoot();
void drawScore(void* font);
void drawShip();
void drawAsteroid(int i);
void drawBullet(int i);
void drawLevel(void* font);
void drawBulletsLeft(void* font);
void drawGameOver();
void updateAsteroids();
void randomizeAsteroid();

//-------------------Draw Functions--------------------//
void drawAsteroid(int i) {
    glLoadIdentity();
    glTranslated(asteroids[i]->centerX, asteroids[i]->centerY, 0.0);
    glColor3ub(asteroids[i]->red, asteroids[i]->green, asteroids[i]->blue);
    glBegin(GL_POLYGON);
    for (int j = 0; j < asteroids[i]->numOfSides; j++) {
        double theta = 2.0 * PI * double(j) / double(asteroids[i]->numOfSides);
        glVertex2d(asteroids[i]->radius * cos(theta), asteroids[i]->radius * sin(theta));
    }
    glEnd();
}

void drawBullet(int i) {
    glLoadIdentity();
    glColor3ub(100, 255, 140);
    glLineWidth(4.0);
    glBegin(GL_LINES);
    glVertex2d(bullets[i]->x, bullets[i]->y);
    glVertex2d(bullets[i]->x + bullets[i]->movementX, bullets[i]->y + bullets[i]->movementY);
    glEnd();
    bullets[i]->x += bullets[i]->movementX;
    bullets[i]->y += bullets[i]->movementY;
    // Remove bullet if out of bounds
    if (bullets[i]->x < -55 || bullets[i]->x > 55 || bullets[i]->y < -55 || bullets[i]->y > 55) {
        delete bullets[i];
        bullets[i] = nullptr;
    }
}

void drawShip() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glRotatef(angle, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glColor3ub(0, 255, 25);
    glVertex2f(0.0, 0.0);
    glColor3ub(50, 150, 75);
    glVertex2f(-2.5, -2.5);
    glColor3ub(50, 255, 100);
    glVertex2f(2.5, 0.0);
    glVertex2f(-2.5, 2.5);
    glEnd();
    glPopMatrix();
}

void drawScore(void* font) {
    string scoreString = "Score : " + to_string(score);
    glColor3ub(0, 150, 255);
    glRasterPos2f(-47, 45);
    for (int i = 0; i < scoreString.length(); i++) {
        glutBitmapCharacter(font, scoreString[i]);
    }
}

void drawLevel(void* font) {
    string levelString = "Level : " + to_string(level);
    glColor3ub(0, 150, 255);
    glRasterPos2f(35, 45);
    for (int i = 0; i < levelString.length(); i++) {
        glutBitmapCharacter(font, levelString[i]);
    }
}

void drawBulletsLeft(void* font) {
    string bulletsString = "Bullets : " + to_string(bulletsLeft);
    glColor3ub(255, 100, 100);
    glRasterPos2f(-47, 40);
    for (int i = 0; i < bulletsString.length(); i++) {
        glutBitmapCharacter(font, bulletsString[i]);
    }
}

//Asteroid Movement
void updateAsteroids() {
    for (int i = 0; i < levelAsteroids; i++) {
        if (asteroids[i] != nullptr) {
            asteroids[i]->centerX += asteroids[i]->velocityX;
            asteroids[i]->centerY += asteroids[i]->velocityY;
            // Screen wrapping
            if (asteroids[i]->centerX > 50) asteroids[i]->centerX = -50;
            if (asteroids[i]->centerX < -50) asteroids[i]->centerX = 50;
            if (asteroids[i]->centerY > 50) asteroids[i]->centerY = -50;
            if (asteroids[i]->centerY < -50) asteroids[i]->centerY = 50;
        }
    }
}

//-------------------Collision Detection--------------------//
void detectCollision() {
    for (int j = 0; j < levelAsteroids; j++) {
        if (asteroids[j] != nullptr) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i] != nullptr) {
                    if ((asteroids[j]->centerX + asteroids[j]->radius > bullets[i]->x && asteroids[j]->centerX - asteroids[j]->radius < bullets[i]->x) &&
                        (asteroids[j]->centerY + asteroids[j]->radius > bullets[i]->y && asteroids[j]->centerY - asteroids[j]->radius < bullets[i]->y)) {
                        delete asteroids[j];
                        asteroids[j] = nullptr;
                        delete bullets[i];
                        bullets[i] = nullptr;
                        score++;
                        nOfAsteroids--;
                        break;
                    }
                }
            }
        }
    }
}

//Randomize Asteroids
void randomizeAsteroid() {
    int i = 0;
    while (i < levelAsteroids) {
        int radius = (rand() % 3 + 6);
        int centerX = rand() % (100 - (2 * radius)) + (-50 + radius);
        int centerY = rand() % (100 - (2 * radius)) + (-50 + radius);
        bool collides = false;
        for (int k = 0; k < i; k++) {
            if (asteroids[k] != nullptr) {
                if ((asteroids[k]->centerX + asteroids[k]->radius > centerX && asteroids[k]->centerX - asteroids[k]->radius < centerX) &&
                    (asteroids[k]->centerY + asteroids[k]->radius > centerY && asteroids[k]->centerY - asteroids[k]->radius < centerY)) {
                    collides = true;
                    break;
                }
            }
        }
        if ((centerX + radius + 5 > 0 && centerX - radius - 5 < 0) && (centerY + radius + 5 > 0 && centerY - radius - 5 < 0)) {
            collides = true;
        }
        if (collides) {
            continue;
        } else {
            Asteroid* obj = new Asteroid(radius, centerX, centerY);
            asteroids[i] = obj;
            nOfAsteroids++;
            i++;
        }
    }
    // Reset bullets for new level
    bulletsLeft = MAX_BULLETS_PER_LEVEL;
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i] != nullptr) {
            delete bullets[i];
            bullets[i] = nullptr;
        }
    }
}

//-------------------Game Over--------------------//
void drawGameOver() {
    string gameOverStr;
    if (outOfBullets)
        gameOverStr = "Game Over - Out of Bullets!";
    else
        gameOverStr = "Game Over - All Levels Completed";
    glColor3ub(0, 231, 136);
    glRasterPos2f(-25, 10.0);
    for (int i = 0; i < gameOverStr.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, gameOverStr[i]);
    }
    string scoreStr = "Your Score is: " + to_string(score);
    glColor3ub(200, 50, 50);
    glRasterPos2f(-25, -10.0);
    for (int i = 0; i < scoreStr.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreStr[i]);
    }
    string quitStr = "Press Esc to quit";
    glColor3ub(55, 250, 100);
    glRasterPos2f(-25, -25.0);
    for (int i = 0; i < quitStr.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, quitStr[i]);
    }
}

//-------------------Input & Timer--------------------//
void TimerFunc(int value) {
    if (isAnimating)
        glutPostRedisplay();
    glutTimerFunc(10, TimerFunc, 1);
}

void special(int key, int, int) {
    switch (key) {
    case GLUT_KEY_LEFT:
        angle += SHIPSPEED;
        break;
    case GLUT_KEY_RIGHT:
        angle -= SHIPSPEED;
        break;
    }
}

void keyInput(unsigned char key, int x, int y) {
    if (isAnimating && !outOfBullets) {
        switch (key) {
        case ' ':
            shoot();
            break;
        case 27:
            exit(0);
            break;
        }
    } else if (key == 27) {
        exit(0);
    }
}

void pattern(int item) {
    switch (item) {
    case 1: grid = true; break;
    case 2: grid = false; break;
    default: break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 50.0, -50.0, 50.0, -50.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    change = (w < 600);
}

//-------------------Bullet Shooting--------------------//
void shoot() {
    if (bulletsLeft > 0) {
        bullets[bulletsCount] = new Bullet(angle);
        if (bulletsCount == MAX_BULLETS - 1)
            bulletsCount = 0;
        else
            bulletsCount++;
        bulletsLeft--;
    }
    if (bulletsLeft == 0) {
        isAnimating = false;
        outOfBullets = true;
        glutPostRedisplay();
    }
}

//-------------------Main Display--------------------//
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw background
    if (grid) {
        for (float x = -50; x < 50; x += 10) {
            glColor3ub(100, 100, 100);
            glLineWidth(1.0);
            glBegin(GL_LINES);
            glVertex2f(x, 50);
            glVertex2f(x, -50);
            glEnd();
        }
        for (float y = -50; y < 50; y += 10) {
            glColor3ub(100, 100, 100);
            glLineWidth(1.0);
            glBegin(GL_LINES);
            glVertex2f(50, y);
            glVertex2f(-50, y);
            glEnd();
        }
    } else {
        glColor3ub(250, 255, 40);
        glEnable(GL_POLYGON_STIPPLE);
        glPolygonStipple(fly);
        glRectf(-50, -50, 50, 50);
        glDisable(GL_POLYGON_STIPPLE);
    }

    if (!isAnimating) {
        drawGameOver();
        glutSwapBuffers();
        return;
    }

    if (level <= NUM_OF_LEVELS && !outOfBullets) {
        // Move asteroids
        updateAsteroids();

        // Draw Bullets
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i] != nullptr) {
                drawBullet(i);
            }
        }

        // Draw Asteroids
        if (nOfAsteroids == 0) {
            level++;
            if (level > NUM_OF_LEVELS) {
                isAnimating = false;
                glutSwapBuffers();
                return;
            }
            levelAsteroids++;
            randomizeAsteroid();
        }
        for (int i = 0; i < levelAsteroids; i++) {
            if (asteroids[i] != nullptr)
                drawAsteroid(i);
        }

        // Collision
        detectCollision();

        // Draw Ship
        drawShip();

        // Draw UI
        if (change) {
            drawScore(GLUT_BITMAP_HELVETICA_12);
            drawLevel(GLUT_BITMAP_HELVETICA_12);
            drawBulletsLeft(GLUT_BITMAP_HELVETICA_12);
        } else {
            drawScore(GLUT_BITMAP_9_BY_15);
            drawLevel(GLUT_BITMAP_9_BY_15);
            drawBulletsLeft(GLUT_BITMAP_9_BY_15);
        }
    } else {
        isAnimating = false;
        drawGameOver();
    }

    glutSwapBuffers();
}

//-------------------Main--------------------//
int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Multi Level Asteroid Game");

    glutCreateMenu(pattern);
    glutAddMenuEntry("Grid On", 1);
    glutAddMenuEntry("Pattern On", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyInput);
    glutTimerFunc(25, TimerFunc, 1);

    glutMainLoop();
    return 0;
}

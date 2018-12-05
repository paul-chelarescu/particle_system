////////////////////////////////////////////////////////////////
// School of Computer Science
// The University of Manchester
//
// This code is licensed under the terms of the Creative Commons 
// Attribution 2.0 Generic (CC BY 3.0) License.
//
// Skeleton code for COMP37111 coursework, 2013-14
//
// Authors: Arturs Bekasovs and Toby Howard
//
/////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>
#include "frames.h"

#define TIME_STEP 0.3
#define NUM_PARTICLES 10000000
#define DEG_TO_RAD 0.017453293

// Display list for coordinate axis 
GLuint axisList;

int AXIS_SIZE= 200;
int axisEnabled= 0;
float rotX, rotY, rotZ;
float pVeloc, pAcc = 0.09, pPos, pGravPos, pYPos;
float date;
float gravity = -9.8;
double spray_factor = 0.7;
int particle_count = 3000;
int start_particle_count = 3000;
int sources_count = 3;
int color_mode = 1;
int mute = 1;

typedef struct {
	double px, py, pz;
	double dx, dy, dz;
	double speed, size, scale;
	// double rx, ry, rz, ra;
	double r, g, b;
	int age;
} particle;
particle particles[NUM_PARTICLES];

///////////////////////////////////////////////

double myRandom()
//Return random double within range [0,1]
{
  return (rand()/(double)RAND_MAX);
}

///////////////////////////////////////////////
void updatePositions() {
    for (int i = 0; i < particle_count; i++) {
        if (particles[i].py > 0 && particles[i].py < particles[i].scale
                && particles[i].pz < 300 && particles[i].px < 300
                && particles[i].pz > -300 && particles[i].px > -300
                && !(particles[i].pz > 100 && particles[i].px < -100)) {
            particles[i].dx *= 0.95;
            particles[i].dy *= -0.95;
            particles[i].dz *= 0.95;
            if (particles[i].dx < 0.005 && particles[i].dy < 0.005 && particles[i].dz < 0.005) {
                particles[i].speed = 0;
            }
        } else if (particles[i].py <= 0 && particles[i].py < particles[i].scale
                && particles[i].pz < 300 && particles[i].px < 300
                && particles[i].pz > -300 && particles[i].px > -300
                && !(particles[i].pz > 100 && particles[i].px < -100)) {
            particles[i].py = 0;
            particles[i].dy *= -0.7;
            if (!mute) system("aplay sound.wav&"); // from http://soundbible.com/1742-Anvil-Impact-1x.html
        }
        if (abs(particles[i].py) > particles[i].scale) particles[i].dy -= 0.05;


        particles[i].px += particles[i].speed * particles[i].dx;
        particles[i].py += particles[i].speed * particles[i].dy;
        particles[i].pz += particles[i].speed * particles[i].dz;


        if (particles[i].scale >= 5) {
            double current_position = 360.0 / sources_count * ((double) particles[i].scale / 5);
            particles[i].px = 50.0 * sin(date / 2 + current_position * DEG_TO_RAD);
            particles[i].py = 10.0 + 100;
            particles[i].pz = 50.0 * cos(date / 2 + current_position * DEG_TO_RAD);
        }

        particles[i].age += 1;
    }
}

void drawParticle(void) {
    glPointSize(20.0f);
    glBegin(GL_POINTS);
        for (int i = 0; i < particle_count; i++) {
            glColor3f(particles[i].r, particles[i].g, particles[i].b);
            glVertex3f (particles[i].px, particles[i].py, particles[i].pz);
        }
    glEnd ();
    updatePositions();
}

void cleanParticles() {
    for (int i = 0; i < particle_count; i++) {
        if (particles[i].age > 600 || (particles[i].speed == 0 && particles[i].scale < 5)) {
            particles[i] = particles[particle_count - 1];
            particle_count -= 1;
        }
    }
}

void drawGround() {
	glColor3f(0.7, 0.7, 0.7);
	glBegin(GL_QUADS);
	glVertex3f(-300.0, 0.0, -300.0);
	glVertex3f(-300.0, 0.0, 100.0);
	glVertex3f(300.0, 0.0, 100.0);
	glVertex3f(300.0, 0.0, -300.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(300.0, 0.0, 300.0);
	glVertex3f(-100.0, 0.0, 300.0);
	glVertex3f(-100.0, 0.0, 100.0);
	glVertex3f(300.0, 0.0, 100.0);
	glEnd();
}

void writeString(void *font, GLclampf r, GLclampf g, GLclampf b,
              GLfloat x, GLfloat y, char* str) {
  char *ch;
  GLint matrixMode;

 glGetIntegerv(GL_MATRIX_MODE, &matrixMode);

 glMatrixMode(GL_PROJECTION);
 glPushMatrix();
   glLoadIdentity();
   gluOrtho2D(0.0, 1.0, 0.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
     glLoadIdentity();
     glPushAttrib(GL_COLOR_BUFFER_BIT);
       glColor3f(r, g, b);
       glRasterPos3f(x, y, 0.0);
       for(ch= str; *ch; ch++) {
          glutBitmapCharacter(font, (int)*ch);
       }
     glPopAttrib();
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
 glPopMatrix();
 glMatrixMode(matrixMode);
}

void display() {
    frameStart();
    glLoadIdentity();
    gluLookAt(100.0 + rotX, 100.0 + rotY, 300.0 + rotZ,
                0.0, 0.0, 0.0,
                0.0, 1.0, 0.0);
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // If enabled, draw coordinate axis
    if (rotY < 0) {
        drawParticle();
        drawGround();
    } else {
        drawGround();
        drawParticle();
    }
    if(axisEnabled) glCallList(axisList);

    cleanParticles();

    frameEnd(GLUT_BITMAP_HELVETICA_10, 1.0, 0.0, 0.0, 0.003, 0.99);
    char text[30];
    sprintf(text, "Particles: %d", particle_count);
    writeString(GLUT_BITMAP_HELVETICA_10, 1.0, 0.0, 0.0, 0.003, 0.98, text);
    sprintf(text, "Explosion particles: %d", start_particle_count);
    writeString(GLUT_BITMAP_HELVETICA_10, 1.0, 0.0, 0.0, 0.003, 0.97, text);
    sprintf(text, "Sources: %d", sources_count);
    writeString(GLUT_BITMAP_HELVETICA_10, 1.0, 0.0, 0.0, 0.003, 0.96, text);
    glutSwapBuffers();
}

///////////////////////////////////////////////

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:  /* Escape key */
            exit(0);
            break;
        case 'a':
            if (axisEnabled) axisEnabled = 0;
            else axisEnabled = 1;
            break;
        case 's':
            sources_count += 1;
            particles[particle_count].px = 50.0 * sin((date / 2) +  DEG_TO_RAD);
            particles[particle_count].py = 10.0 + 100;
            particles[particle_count].pz = 50.0 * cos((date / 2) +  DEG_TO_RAD);
            particles[particle_count].speed = 0;
            particles[particle_count].scale = 5 * sources_count;
            particles[particle_count].age = 300;
            particles[particle_count].r = 0;
            particles[particle_count].g = 0;
            particles[particle_count].b = 0;

            particle_count += 1;
            break;
        case 'S':
            sources_count -= 1;
            particles[particle_count].px = 50.0 * sin((date / 2) +  DEG_TO_RAD);
            particles[particle_count].py = 10.0 + 100;
            particles[particle_count].pz = 50.0 * cos((date / 2) +  DEG_TO_RAD);
            particles[particle_count].speed = 0;
            particles[particle_count].scale = 0;
            particles[particle_count].age = 300;
            particles[particle_count].r = 0;
            particles[particle_count].g = 0;
            particles[particle_count].b = 0;

            particle_count -= 1;
            break;
        case 'X':
          rotX -= 100.0;
          break;
        case 'x':
          rotX += 100.0;
          break;
        case 'Y':
          rotY -= 100.0;
          break;
        case 'y':
          rotY += 100.0;
          break;
        case 'Z':
          rotZ -= 100.0;
          break;
        case 'z':
          rotZ += 100.0;
          break;
        case 'c':
          if (color_mode == 1) color_mode = 0;
          else color_mode = 1;
          break;
        case 'm':
          mute = !mute;
          break;
        case 'w':
          start_particle_count += 1000;
          break;
        case 'W':
          start_particle_count -= 1000;
          break;
        case 'p':
          for (int a = 1; a <= sources_count; a++) {
            double current_position = 360.0 / (double) sources_count * a;
            particles[particle_count].px = 50.0 * sin((date / 2) + current_position * DEG_TO_RAD);
            particles[particle_count].py = 10.0 + 100;
            particles[particle_count].pz = 50.0 * cos((date / 2) + current_position * DEG_TO_RAD);
            // Direction
            particles[particle_count].dx = /*(myRandom() - 0.5) */ spray_factor * sin(date / 2 + current_position * DEG_TO_RAD);
            particles[particle_count].dy = myRandom() * 1.5;
            particles[particle_count].dz = /*(myRandom() - 0.5) */ spray_factor * cos(date / 2 + current_position * DEG_TO_RAD);

            particles[particle_count].speed = 5;
            particles[particle_count].scale = 0.5;
            particles[particle_count].age = 50 * myRandom();
            if (color_mode) {
                particles[particle_count].r = myRandom();
                particles[particle_count].g = myRandom();
                particles[particle_count].b = myRandom();
            }
            particle_count += 1;

            particles[particle_count].px = 50.0 * sin((date / 2) + current_position * DEG_TO_RAD);
            particles[particle_count].py = 10.0 + 100;
            particles[particle_count].pz = 50.0 * cos((date / 2) + current_position * DEG_TO_RAD);
            particles[particle_count].speed = 0;
            particles[particle_count].scale = 5 * a;
            particles[particle_count].age = 300;
            particles[particle_count].r = 0;
            particles[particle_count].g = 0;
            particles[particle_count].b = 0;

            particle_count += 1;
          }
          break;
        case ' ':
          date = 0.0;
          pVeloc = 0.0;
          pPos = 0.0;
          pGravPos = 0.0;
          pYPos = 0.0;
          particle_count = start_particle_count;
          for (int i = 0; i < start_particle_count; i++) {
              particles[i].px = 10.0 * (myRandom() - 0.5);
              particles[i].py = 10.0 * (myRandom() - 0.5) + 100;
              particles[i].pz = 10.0 * (myRandom() - 0.5);

              particles[i].dx = (myRandom() - 0.5) * spray_factor;
              particles[i].dy = myRandom();
              particles[i].dz = (myRandom() - 0.5) * spray_factor;

              particles[i].speed = 5;
              particles[i].scale = 0.5;
              particles[i].age = 50 * myRandom();
              if (color_mode) {
                  particles[i].r = myRandom();
                  particles[i].g = myRandom();
                  particles[i].b = myRandom();
              }
          }
          break;
    }
    glutPostRedisplay();
}

///////////////////////////////////////////////

void reshape(int width, int height)
{
  glClearColor(0.9, 0.9, 0.9, 1.0);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
  glMatrixMode(GL_MODELVIEW);
}

///////////////////////////////////////////////

void makeAxes() {
// Create a display list for drawing coord axis
  axisList = glGenLists(1);
  glNewList(axisList, GL_COMPILE);
      glLineWidth(2.0);
      glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);       // X axis - red
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(AXIS_SIZE, 0.0, 0.0);
      glColor3f(0.0, 1.0, 0.0);       // Y axis - green
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, AXIS_SIZE, 0.0);
      glColor3f(0.0, 0.0, 1.0);       // Z axis - blue
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, 0.0, AXIS_SIZE);
    glEnd();
  glEndList();
}

void initParticles() {
    for (int i = 0; i < particle_count; i++) {
        // Particle position
        particles[i].px = 10.0 * (myRandom() - 0.5);
        particles[i].py = 10.0 * (myRandom() - 0.5) + 100;
        particles[i].pz = 10.0 * (myRandom() - 0.5);
        // Direction
        particles[i].dx = (myRandom() - 0.5) * spray_factor;
        particles[i].dy = myRandom();
        particles[i].dz = (myRandom() - 0.5) * spray_factor;

        particles[i].speed = 5;
        particles[i].scale = 0.5;
        particles[i].age = 50 * myRandom();
        if (color_mode) {
            particles[i].r = myRandom();
            particles[i].g = myRandom();
            particles[i].b = myRandom();
        }
    }
}

///////////////////////////////////////////////
void initGraphics(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("COMP37111 Particles");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  makeAxes();
  initParticles();
  glEnable(GL_DEPTH_TEST);
}

/////////////////////////////////////////////////
void animate(void) {
    date += TIME_STEP;
    pVeloc += pAcc;
    pPos += pVeloc;
    pGravPos += gravity * date / 40;
    pYPos += pVeloc / 90;
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    // double f;
    srand(time(NULL));
    initGraphics(argc, argv);
    glutFullScreen();
    glEnable(GL_POINT_SMOOTH);
    glutDisplayFunc (display);
    glutIdleFunc (animate);
    glutMainLoop();
    return 0;
}

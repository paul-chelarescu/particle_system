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

// Display list for coordinate axis 
GLuint axisList;

int AXIS_SIZE= 200;
int axisEnabled= 1;
float rotX, rotY, rotZ;

///////////////////////////////////////////////

double myRandom()
//Return random double within range [0,1]
{
  return (rand()/(double)RAND_MAX);
}

///////////////////////////////////////////////
void drawParticle(void) {
    glPointSize(20.0f);
    glBegin(GL_POINTS);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f (0.0, 60.0, 40.0);
        glVertex3f (0.0, 80.0, 0.0);
        glVertex3f (80.0, 60.0, 0.0);
        glVertex3f (80.0, 30.0, 0.0);
        glVertex3f (60.0, 0.0, 50.0);
        glVertex3f (20.0, 0.0, 50.0);
    glEnd ();
}


void display() {
    glLoadIdentity();
    gluLookAt(100.0 + rotX, 100.0 + rotY, 300.0 + rotZ,
                0.0, 0.0, 0.0,
                0.0, 1.0, 0.0);
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    // If enabled, draw coordinate axis
    if(axisEnabled) glCallList(axisList);
    drawParticle();

    glutSwapBuffers();
}

///////////////////////////////////////////////

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:  /* Escape key */
            exit(0);
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

///////////////////////////////////////////////
void initGraphics(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("COMP37111 Particles");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  makeAxes();
}

/////////////////////////////////////////////////

int main(int argc, char *argv[]) {
    // double f;
    srand(time(NULL));
    initGraphics(argc, argv);
    glEnable(GL_POINT_SMOOTH);
    glutDisplayFunc (display);
    glutMainLoop();
    return 0;
}

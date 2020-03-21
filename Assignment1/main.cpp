//  ========================================================================
//  COSC363: Computer Graphics (2020); CSSE  University of Canterbury.
//
//  FILE NAME: main.cpp
//  The main function and glutMainLoop() for the museum.
//  Contains main() initialize() display()
//  ========================================================================

#include <iostream>
#include <cmath>
#include <stdio.h>
#include <GL/glut.h>

#include "loadTGA.h"
#include "keyboard.h"
#include "main.h"
using namespace std;

float angle=0, look_x, look_z=-1., eye_x, eye_z;  //Camera parameters

const char* sky_files[6] = {"right.tga", "left.tga", "top.tga", "bottom.tga", "back.tga", "front.tga"};
enum TextureIndex {SKY_RIGHT, SKY_LEFT, SKY_TOP, SKY_BOTTOM, SKY_BACK, SKY_FRONT, WALL_ID, FLOOR_ID, NUM_TEXTURES};
GLuint txId[NUM_TEXTURES];


//--------------------------------------------------------------------------------

void initialise()
{
    loadTexture();
    glEnable(GL_TEXTURE_2D);
    glClearColor(0., 1., 1., 1.);    //Background colour
}

//--------------------------------------------------------------------------------

void walls()
{
    glColor3f(0.8, 0.7, 0.3);   //replace with a texture
    glBindTexture(GL_TEXTURE_2D, txId[WALL_ID]);
    
    glBegin(GL_QUADS);
    
    ////////////////////// BACK WALL ///////////////////////
    
    glTexCoord2f(0, 2);     glVertex3f(-15, 1, -15);
    glTexCoord2f(0, 0);     glVertex3f(-15, -1, -15);
    glTexCoord2f(12, 0);     glVertex3f(15, -1, -15);
    glTexCoord2f(12, 2);     glVertex3f(15, 1, -15);
    
    ////////////////////// FRONT WALL ///////////////////////
    
    glTexCoord2f(0, 2);     glVertex3f(-15, 1, 15);
    glTexCoord2f(0, 0);     glVertex3f(-15, -1, 15);
    glTexCoord2f(12, 0);     glVertex3f(15, -1, 15);
    glTexCoord2f(12, 2);     glVertex3f(15, 1, 15);
    
    ////////////////////// LEFT WALL ///////////////////////
    
    glTexCoord2f(0, 2);     glVertex3f(-15, 1, -15);
  	 glTexCoord2f(0, 0);     glVertex3f(-15, -1, -15);
    glTexCoord2f(12, 0);     glVertex3f(-15, -1, 15);
    glTexCoord2f(12, 2);     glVertex3f(-15, 1, 15);
    
    
    ////////////////////// RIGHT WALL ///////////////////////
    
    glTexCoord2f(0, 2);     glVertex3f(15, 1, -15);
    glTexCoord2f(0, 0);     glVertex3f(15, -1, -15);
    glTexCoord2f(12, 0);     glVertex3f(15, -1, 15);
    glTexCoord2f(12, 2);     glVertex3f(15, 1, 15);
    
    glEnd();
}

//--------------------------------------------------------------------------------
void floor()
{
    glColor3d(0.6, 0.25, 0.25);  //replace with a texture
    glBindTexture(GL_TEXTURE_2D, txId[FLOOR_ID]);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 16);     glVertex3f(-15, -1, -15);
    glTexCoord2f(0, 0);     glVertex3f(-15, -1, 15);
    glTexCoord2f(16, 0);     glVertex3f(15, -1, 15);
    glTexCoord2f(16, 16);     glVertex3f(15, -1, -15);
    glEnd();
}

//--------------------------------------------------------------------------------
// CAUTION
void skybox() {
    float x = 0;
    float y = 0;
    float z = 0;
    float width  = 100;
    float height = 100;
    float length = 100;
    
    x = x - width  / 2 + eye_x;
    y = y - height / 2;
    z = z - length / 2 + eye_z;
    
    glBindTexture(GL_TEXTURE_2D, txId[SKY_RIGHT]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,  z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,  z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[SKY_LEFT]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y,  z + length);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y,  z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[SKY_TOP]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[SKY_BOTTOM]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y,  z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y,  z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,  z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,  z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[SKY_BACK]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,  z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y,  z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[SKY_FRONT]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y,  z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,  z + length);
    glEnd();
    
    
}


//--------------------------------------------------------------------------------
void loadTexture()
{
    glGenTextures(NUM_TEXTURES, txId); 	// Create texture ids
    
    glBindTexture(GL_TEXTURE_2D, txId[WALL_ID]);  //Use this texture
    loadTGA("Wall.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[FLOOR_ID]);  //Use this texture
    loadTGA("Floor.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    for (int i=0; i < 6; i++) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, txId[SKY_RIGHT + i]);
        loadTGA(sky_files[i]);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}


//--------------------------------------------------------------------------------

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45., 1., 1., 100.);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_x, 0, eye_z,  look_x, 0, look_z,   0, 1, 0);
    
    skybox();
    walls();
    floor();
    
    glutSwapBuffers();
}

//--------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
    glutInitWindowSize (600, 600); 
    glutInitWindowPosition (10, 10);
    glutCreateWindow ("The Museum");
    initialise();
    
    
    glutDisplayFunc(display);
    glutSpecialFunc(spec_key_event);
    glutKeyboardFunc(reg_key_event);
    
    glutMainLoop();
    return 0;
}

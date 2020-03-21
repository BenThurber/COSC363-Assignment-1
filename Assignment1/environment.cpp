//  ========================================================================
//  COSC363: Computer Graphics (2020); CSSE  University of Canterbury.
//
//  FILE NAME: environment.cpp
//  Contains the skybox and base/floor
//  ========================================================================

#include "environment.h"
#include <GL/glut.h>
#include <stdio.h>
#include "textures.h"
#include "main.h"

//--------------------------------------------------------------------------------
void floor()
{
    glColor3d(0.6, 0.25, 0.25);  //replace with a texture
    printf("%d\n", FLOOR_ID);
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


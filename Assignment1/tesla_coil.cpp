//
//  tesla_coil.cpp
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 8/04/20.
//
//  Model of a tesla coil
//

#include <GL/freeglut.h>
#include <cmath>
#include "main.h"
#include "building.h"
#include "tesla_coil.h"



void secondary_coil(float radius, float height, GLuint texId)
{
    GLUquadric *quad;
    quad = gluNewQuadric();
    
    glBindTexture(GL_TEXTURE_2D, texId);
    gluQuadricTexture(quad, GLU_TRUE);
    gluQuadricOrientation(quad, GLU_OUTSIDE);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    
    
    // scale the texture of the coils
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1, 2, 1);
    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    
        glRotatef(-90, 1, 0, 0);
        gluCylinder(quad, radius, radius, height, 20, 5);
    
    glPopMatrix();
    
    // sets texture scaling back to normal (identity matrix)
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    
}


void tesla_coil(GLuint* textures)
{
    secondary_coil(3, 25, textures[COPPER_COIL]);
    
}

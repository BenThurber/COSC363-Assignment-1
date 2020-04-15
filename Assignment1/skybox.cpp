//
//  skybox.cpp
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 7/04/20.
//
//  A skybox
//

#include <GL/freeglut.h>
#include "skybox.h"
#include "main.h"

// These should not be changed (duh)
#define CUBE_SIDES 6
#define SQR_VERTI 4

void draw_side(float l);



void skybox(int size, GLuint* textures)
{
    glDisable(GL_LIGHTING);
    glColor3d(1, 1, 1);
    
    float l = (float)size / 2.0;
    for (int i=0; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[SKY_RIGHT + i]);
        
        glPushMatrix();
            glRotated((i * 90), 0, 1, 0);
            draw_side(l);
        glPopMatrix();
    }
    
    glBindTexture(GL_TEXTURE_2D, textures[SKY_TOP]);
    glPushMatrix();
        glRotated(90, 0, 1, 0);
        glRotated(90, 0, 0, 1);
        draw_side(l);
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D, textures[SKY_BOTTOM]);
    glPushMatrix();
        glRotated(90, 0, 1, 0);
        glRotated(-90, 0, 0, 1);
        draw_side(l);
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
}


void draw_side(float l) {
    float n = 0.002 * l;
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);    glVertex3f(l-n, -l, -l);
    glTexCoord2f(1.0f, 0.0f);    glVertex3f(l-n, -l,  l);
    glTexCoord2f(1.0f, 1.0f);    glVertex3f(l-n,  l,  l);
    glTexCoord2f(0.0f, 1.0f);    glVertex3f(l-n,  l, -l);
    glEnd();
}



























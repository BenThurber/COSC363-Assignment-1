//
//  building.cpp
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 6/04/20.
//
//  A model of an empty museum
//
//

#include <GL/freeglut.h>
#include <cmath>
#include "main.h"
#include "building.h"
#include "loadOFF.h"

#define INC_ANGLE (360/num_sides)

//--------------------------------------------------------------------------------
// Make a roof

// Helper function
void set_roof_vertex(float angle, float radius, float height, float height_offset) {
    glVertex3f(radius * cos(RAD(angle)), height + height_offset, radius * sin(RAD(angle)));
}
// Function to build the roof
void roof(float radius, float height, float angle, float thickness, int num_sides)
{
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat);
//    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);
    glColor3f(1.0, 0.75, 0.5);  // Mat
    
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);  // Unbind Last bound texture
    
    float smaller_rad = radius - thickness*(1.0/tan(RAD(angle)));
    
    // Lower Base
    glBegin(GL_POLYGON);
    // Create verticies in a circle with 60 degree increments
    glNormal3f(0, -1, 0);
    for (int angle=0; angle <= 360; angle += INC_ANGLE) {
        set_roof_vertex(angle, radius, height, 0);
    }
    glEnd();
    
    // Upper Base
    glBegin(GL_POLYGON);
    // Create the same as the lower base but smaller based on ROOF_SLANT and thickness
    glNormal3f(0, 1, 0);
    for (int angle=0; angle <= 360; angle += INC_ANGLE) {
        set_roof_vertex(angle, smaller_rad, height, thickness);
    }
    glEnd();
    
    // Strip connecting the upper and lower base
    glBegin(GL_QUAD_STRIP);
    for (int angle=0; angle <= 360; angle += INC_ANGLE) {
        if (angle > 0) normal(
                              radius * cos(RAD(angle)), height, radius * sin(RAD(angle)),
                              smaller_rad * cos(RAD(angle)), height+thickness, smaller_rad * sin(RAD(angle)),
                              smaller_rad * cos(RAD(angle - INC_ANGLE)), height+thickness, smaller_rad * sin(RAD(angle - INC_ANGLE)));
        set_roof_vertex(angle, radius, height, 0);
        set_roof_vertex(angle, smaller_rad, height, thickness);
    }
    glEnd();
}



void vertical_pillar(float height, float width, GLuint* textures)
{
    GLUquadric *quad;
    quad = gluNewQuadric();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[WOOD1]);
    gluQuadricTexture(quad, GLU_TRUE);
    gluQuadricOrientation(quad, GLU_OUTSIDE);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    
    
    // scale the texture of the coils
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(0.75, 2, 1);
    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quad, width, width, height, 20, 1);

    
    glPopMatrix();
    
    // sets texture scaling back to normal (identity matrix)
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}



//--------------------------------------------------------------------------------
// Make walls

void walls(float wall_radius, float wall_height, int num_sides, GLuint texId)
{
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
//    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);
    glColor3f(1, 1, 1);  // White
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);
    
    glBegin(GL_QUAD_STRIP);
    
        float vx, vy, vz;
        float wx, wy, wz;
        vy = wall_height;
        wy = 0;
    
    
        for (int angle=120, i=0; angle <= 420; angle += INC_ANGLE, i++) {
            vx = wx = wall_radius * cos(RAD(angle));
            vz = wz = wall_radius * sin(RAD(angle));
            
            // Left side entrance wall
            if (angle <= 120) {
                glNormal3f(0, 0, 1);
                glTexCoord2f(i-1, 1);
                glVertex3f(vx + wall_radius/4, vy, vz);
                glTexCoord2f(i-1, 0);
                glVertex3f(wx + wall_radius/4, wy, wz);
            }
            
            // All other walls
            normal(vx, vy, vz, wx, wy, wz, (wall_radius * cos(RAD(angle - INC_ANGLE))), 0, (wall_radius * sin(RAD(angle - INC_ANGLE))));
            glTexCoord2f(i, 1);
            glVertex3f(vx, vy, vz);
            glTexCoord2f(i, 0);
            glVertex3f(wx, wy, wz);
            
            // Right side entrance wall
            if (angle >= 420) {
                glNormal3f(0, 0, 1);
                glTexCoord2f(i+1, 1);
                glVertex3f(vx - wall_radius/4, vy, vz);
                glTexCoord2f(i+1, 0);
                glVertex3f(wx - wall_radius/4, wy, wz);
            }
        }
    glEnd();
}



// Draw a floor inside the museum
void floor(float size, Model* model)
{
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.784, 0.784, 0.784);  // Grey
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);  // Disable specular reflections
    
    glPushMatrix();
        glScalef(size, 1, size);
        drawModel(model);
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);   //Re-enable specular reflections to white
}



// Create a 2D texture on a quad  pxl_w is pixel width
void flat_image(float height, float pxl_w, float pxl_h, GLuint texId)
{
    glColor3f(1, 1, 1);  // White
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);
    
    float h = height;
    float w = (pxl_w/pxl_h) * height;
    
    glBegin(GL_QUADS);   //A simple quad
        glNormal3f(0, 0, 1);
        glTexCoord2f(1, 0);  glVertex3f(w/2, 0, 0);
        glTexCoord2f(0, 0);  glVertex3f(-w/2, 0, 0);
        glTexCoord2f(0, 1);  glVertex3f(-w/2, h, 0);
        glTexCoord2f(1, 1);  glVertex3f(w/2, h, 0);
    glEnd();
    
}




//--------------------------------------------------------------------------------
// Make the whole building

// Note textures is an array on length [???] that holds texture IDs
void building(float wall_radius, float wall_height, float roof_radius, float roof_angle, float roof_thickness, int num_sides, GLuint* textures, Model **models)
{
    floor(wall_radius, models[FLOOR]);
    
    roof(roof_radius, wall_height, roof_angle, roof_thickness, num_sides);
    walls(wall_radius, wall_height, num_sides, textures[OUTER_WALL]);
    
    // Create a slightly smaller inner wall.  Wall normals should now point inward.  Easier than creating a seperate function.
    glPushMatrix();
        glScalef(-1, 1, 1);
        walls(0.99 * wall_radius, wall_height, num_sides, textures[INNER_WALL]);
    glPopMatrix();
    
    // Create a door frame
    const int pillar_width = 2;
    glPushMatrix();
        glTranslatef(wall_radius/4, 0, wall_radius * cos(RAD(30)));
        vertical_pillar(wall_height, pillar_width, textures);
        glPushMatrix();
            glTranslatef(-wall_radius/2, 0, 0);
            vertical_pillar(wall_height, pillar_width, textures);
        glPopMatrix();
    glPopMatrix();
    
    // Add a wall portrait of Nikola Tesla (80% size of wall hight)
    glPushMatrix();
        glTranslatef(0, 0.05*wall_height, -(wall_radius * cos(RAD(30)) - 1));
        flat_image(wall_height * 0.9, 1536, 2068, textures[PORTRAIT]);
    glPopMatrix();
    
    // Add a title sign to the outside of the museum "Nikola Tesla Museum"
    const float title_height = 5;
    glPushMatrix();
        glTranslatef(0, wall_height + roof_thickness/2 - title_height/2, roof_radius * cos(RAD(30)));
        glRotatef(roof_angle - 90, 1, 0, 0);
        flat_image(title_height, 879, 147, textures[TITLE]);
    glPopMatrix();
}




















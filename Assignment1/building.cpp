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
    glBindTexture(GL_TEXTURE_2D, 0);  // Unbind Last bound texture
    glColor3f(1.0, 0.498, 0.0);
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




//--------------------------------------------------------------------------------
// Make the walls

void walls(float wall_radius, float wall_height, int num_sides, GLuint texId)
{
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
                glVertex3f(vx + 5, vy, vz);
                glTexCoord2f(i-1, 0);
                glVertex3f(wx + 5, wy, wz);
            }
            
            // All other walls
            normal(vx, vy, vz, wx, wy, wz, (wall_radius * cos(RAD(angle - INC_ANGLE))), 0, (wall_radius * sin(RAD(angle - INC_ANGLE))));
            glTexCoord2f(i, 1);
            glVertex3f(vx, vy, vz);
            glTexCoord2f(i, 0);
            glVertex3f(wx, wy, wz);
            
            // Left side entrance wall
            if (angle >= 420) {
                glNormal3f(0, 0, 1);
                glTexCoord2f(i+1, 1);
                glVertex3f(vx - 5, vy, vz);
                glTexCoord2f(i+1, 0);
                glVertex3f(wx - 5, wy, wz);
            }
        }
    glEnd();
}







//--------------------------------------------------------------------------------
// Make the whole building

// Note textures is an array on length [???] that holds texture IDs
void building(float wall_radius, float wall_height, float roof_radius, float roof_angle, float roof_thickness, int num_sides, GLuint* textures)
{
    roof(roof_radius, wall_height, roof_angle, roof_thickness, num_sides);
    walls(wall_radius, wall_height, num_sides, textures[0]);
    
    // Create a slightly smaller inner wall.  Wall normals should now point inward.  Easier than creating a seperate function.
    glPushMatrix();
        glRotatef(180, 0, 1, 0);
        glScalef(-1, 1, -1);
        walls(0.95 * wall_radius, wall_height, num_sides, textures[1]);
    glPopMatrix();
}




















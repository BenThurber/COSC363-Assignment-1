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
#include "loadOFF.h"
#include "building.h"
#include "tesla_coil.h"


//----- Draws the top part fo the tesla coil from a model file that is loaded in "main.cpp" ------
void coil_top(float height, Model* top)
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.2, 0.2, 0.2);
    
    glPushMatrix();
        glTranslatef(0, height, 0);
        glScalef(6, 6, 6);
        drawModel(top);
    glPopMatrix();
    
    glEnable(GL_TEXTURE_2D);
}

// Generate x, y, z coordinates of a ring that will we sweeped to create a wire.
// segments is the number of elements in each coord array.
void wire_ring(float radius, int segments, float *x, float *y, float *z) {
    for (int i=0; i < segments; i++) {
        x[i] = 0;
        y[i] = radius * sin(RAD(i * 360.0/(float)segments));
        z[i] = radius * cos(RAD(i * 360.0/(float)segments));
        printf("x=%.3f, y=%.3f, z=%.3f\n", x[i], y[i], z[i]);
    }
}

//----- Draws the primary coil (the flat coil at the bottom) using a sweeped surface
void coil_primary(float start_radius, float wire_dia, int num_turns)
{
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    // For solid change GL_LINE to GL_FILL
    
    glDisable(GL_TEXTURE_2D);
    
    float copper[4] = {1.0, 0.49803, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, copper);
    
    float hoff = 0;  //Horizontal Offset
    float voff = 0;
    
    const int N = 20;
    float vx_init[N];
    float vy_init[N];
    float vz_init[N];
    
    wire_ring(wire_dia/2, N, vx_init, vy_init, vz_init);
    
    // Translate the wire ring to start at the radius
    for (int i=0; i < N; i++) {
        vz_init[i] += start_radius;
    }
    
    float vx[N];
    float vy[N];
    float vz[N];
    
    float wx[N];
    float wy[N];
    float wz[N];
    
    for (int i=0; i < N; i++) {
        vx[i] = vx_init[i];
        vy[i] = vy_init[i];
        vz[i] = vz_init[i];
    }
    
    for (int angle=0, j=0; angle <= 360 * num_turns; angle += 5, j++) {
        
        
        hoff = 0.01 * j;
        voff = 0.0035 * j;
        
        for (int i=0; i < N; i++) {
            wx[i] = (vx_init[i] + hoff) * cos(RAD(angle)) + (vz_init[i] + hoff) * sin(RAD(angle));
            wy[i] = vy_init[i] + voff;
            wz[i] = -(vx_init[i] + hoff) * sin(RAD(angle)) + (vz_init[i] + hoff) * cos(RAD(angle));
            
        }
        
        
        glBegin(GL_QUAD_STRIP);
        
            // Use seperate index k to wrap arround to the start of the array on the last itteration.
            for (int i=0, k=0; i < (N + 1); i++, k=(i % N)) {
//                k = (N - 1) - (i % N);  // This wraps around N and also itterates in reverse order
                
                if (i > 0) normal(wx[i-1], wy[i-1], wz[i-1], vx[i], vy[i], vz[i], vx[i-1], vy[i-1], vz[i-1]);
                glVertex3f(vx[k], vy[k], vz[k]);
                glVertex3f(wx[k], wy[k], wz[k]);
            }
        
        glEnd();
        
        for (int i=0; i < N; i++) {
            vx[i] = wx[i];
            vy[i] = wy[i];
            vz[i] = wz[i];
        }
        

    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // For solid change GL_LINE to GL_FILL
    
    float white[4] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
    glEnable(GL_TEXTURE_2D);
    
}


//----- Draws the secondary coil using a cylinder and a texture
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


void tesla_coil(GLuint texId, Model** models)
{
    float height = 21;
    glScalef(2, 2, 2);  // Temp
    glPushMatrix();
        glTranslatef(0, 1, 0);
        coil_primary(5, 0.4, 9);
    glPopMatrix();
    coil_top(height, models[COIL_TOP]);
    secondary_coil(3, height, texId);
    
}
